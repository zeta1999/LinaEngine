/*
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-2020] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Panels/ResourcesPanel.hpp"
#include "Core/GUILayer.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Core/Application.hpp"
#include "Core/EditorApplication.hpp"
#include "Rendering/RenderEngine.hpp"
#include "Input/InputMappings.hpp"
#include "Core/EditorCommon.hpp"
#include "Widgets/WidgetsUtility.hpp"
#include "Utility/EditorUtility.hpp"
#include "IconsFontAwesome5.h"
#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialogue/ImGuiFileDialog.h"
#include <filesystem>

#define ROOT_NAME "###Resources"

namespace LinaEditor
{

	static int s_itemIDCounter = 0;
	static int s_selectedItem = -1;
	static EditorFolder* s_hoveredFolder;
	static EditorFile* s_selectedFile;
	static EditorFolder* s_selectedFolder;


	static ImVec4 s_highlightColor;
	static ImVec4 s_fileNameColor;
	static ImVec4 s_usedFileNameColor;;
	static bool s_highlightColorSet;
	static float s_colorLerpTimestamp;
	static float s_colorLerpDuration = 1.0f;
	static float s_colorLerpItemID;

	void ResourcesPanel::Setup()
	{
		LinaEditor::EditorApplication::GetEditorDispatcher().SubscribeAction<std::pair<LinaEngine::Graphics::Texture*, LinaEngine::Graphics::Texture*>>("##mrsr_textureReimport", LinaEngine::Action::ActionType::TextureReimported,
			std::bind(&ResourcesPanel::TextureReimported, this, std::placeholders::_1));

		LinaEditor::EditorApplication::GetEditorDispatcher().SubscribeAction<LinaEngine::Graphics::Texture*>("##mrsr_matTextureSelected", LinaEngine::Action::ActionType::MaterialTextureSelected,
			std::bind(&ResourcesPanel::MaterialTextureSelected, this, std::placeholders::_1));

		ScanRoot();

		s_highlightColor = ImGui::GetStyleColorVec4(ImGuiCol_Header);
		s_fileNameColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		s_usedFileNameColor = s_fileNameColor;
	}


	void ResourcesPanel::ScanRoot()
	{
		// Create root.
		EditorFolder root;
		root.m_name = ROOT_NAME;
		root.m_path = "resources";
		m_resourceFolders.push_back(root);

		// Recursively fill in root.
		s_itemIDCounter = -1;
		std::string path = "resources";
		ScanFolder(m_resourceFolders[0]);

		// Load resources	
		LoadFolderResources(m_resourceFolders[0]);
		LoadFolderDependencies(m_resourceFolders[0]);
	}

	void ResourcesPanel::ScanFolder(EditorFolder& root)
	{
		for (const auto& entry : std::filesystem::directory_iterator(root.m_path))
		{
			if (entry.path().has_extension())
			{
				// Is a file
				EditorFile file;
				file.m_name = entry.path().filename().string();
				file.m_pathToFolder = entry.path().parent_path().string() + "/";
				std::string replacedPath = entry.path().relative_path().string();
				std::replace(replacedPath.begin(), replacedPath.end(), '\\', '/');
				file.m_path = replacedPath;
				file.m_extension = file.m_name.substr(file.m_name.find(".") + 1);
				file.m_type = GetFileType(file.m_extension);
				file.m_id = ++s_itemIDCounter;

				// Add to the folder data.
				root.m_files[file.m_id] = file;
			}
			else
			{
				// Is a folder
				EditorFolder folder;
				folder.m_name = entry.path().filename().string();
				std::string replacedPath = entry.path().relative_path().string();
				std::replace(replacedPath.begin(), replacedPath.end(), '\\', '/');
				folder.m_path = replacedPath;
				folder.m_id = ++s_itemIDCounter;
				folder.m_parent = &root;

				// Add to the sub folders.
				root.m_subFolders[folder.m_id] = folder;

				// Iterate recursively.
				ScanFolder(root.m_subFolders[folder.m_id]);
			}
		}
	}

	void ResourcesPanel::Draw()
	{
		if (m_show)
		{
			// Set window properties.
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_area_pos = viewport->GetWorkPos();
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::Begin(RESOURCES_ID, &m_show, flags);
			WidgetsUtility::DrawShadowedLine(5);
			DrawContextMenu();
			DrawFolder(m_resourceFolders[0], true);

			ImGui::End();
		}
	}

	void ResourcesPanel::DrawContextMenu()
	{
		std::string rootPath = s_hoveredFolder == nullptr ? "resources" : s_hoveredFolder->m_path;

		// Handle Right click popup.
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("Create"))
			{
				// Create a folder.
				if (ImGui::MenuItem("Folder"))
				{
					std::string folderPath = rootPath + "/NewFolder" + std::to_string(++s_itemIDCounter);
					EditorUtility::CreateFolderInPath(folderPath);
					EditorFolder folder;
					folder.m_path = folderPath;
					folder.m_name = "NewFolder" + std::to_string(s_itemIDCounter);
					folder.m_id = s_itemIDCounter;

					if (s_hoveredFolder != nullptr)
						s_hoveredFolder->m_subFolders[folder.m_id] = folder;
					else
						m_resourceFolders[0].m_subFolders[folder.m_id] = folder;

				}

				ImGui::Separator();

				// Create a material.
				if (ImGui::MenuItem("Material"))
				{
					std::string name = "NewMaterial" + std::to_string(++s_itemIDCounter) + ".mat";
					std::string materialPath = rootPath + "/" + name;

					EditorFile file;
					file.m_path = materialPath;
					file.m_pathToFolder = rootPath + "/";
					file.m_name = name;
					file.m_extension = "mat";
					file.m_type = FileType::Material;
					file.m_id = ++s_itemIDCounter;

					Graphics::Material& m = LinaEngine::Graphics::Material::CreateMaterial(Graphics::RenderEngine::GetDefaultShader(), file.m_path);
					Graphics::Material::SaveMaterialData(m, materialPath);

					if (s_hoveredFolder != nullptr)
						s_hoveredFolder->m_files[file.m_id] = file;
					else
						m_resourceFolders[0].m_files[file.m_id] = file;
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

	void ResourcesPanel::DrawFolder(EditorFolder& folder, bool isRoot)
	{
		static ImGuiTreeNodeFlags folderFlagsNotSelected = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static ImGuiTreeNodeFlags folderFlagsSelected = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected;
		static ImGuiTreeNodeFlags fileNodeFlagsNotSelected = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
		static ImGuiTreeNodeFlags fileNodeFlagsSelected = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected;


		WidgetsUtility::IncrementCursorPosY(11);
		WidgetsUtility::ItemSpacingX(0);

		if (!isRoot)
			WidgetsUtility::IncrementCursorPosY(-11);


		// Draw folders.
		for (std::map<int, EditorFolder>::iterator it = folder.m_subFolders.begin(); it != folder.m_subFolders.end();)
		{
			// Skip drawing if internal folders.
			if (it->second.m_parent != nullptr && it->second.m_parent->m_name.compare(ROOT_NAME) == 0)
			{
				if (it->second.m_name.compare("engine") == 0 || it->second.m_name.compare("editor") == 0)
				{
					it++;
					continue;
				}
			}

			WidgetsUtility::IncrementCursorPosX(4);

			if (it->second.m_markedForErase)
			{
				// Delete directory.
				EditorUtility::DeleteDirectory(it->second.m_path);

				// Nullout reference.
				if (s_hoveredFolder == &it->second)
					s_hoveredFolder = nullptr;

				// Unload the contained resources & erase.
				UnloadFileResourcesInFolder(it->second);
				folder.m_subFolders.erase(it++);
				continue;
			}

			ImGuiTreeNodeFlags folderFlags = (it->second).m_id == s_selectedItem ? folderFlagsSelected : folderFlagsNotSelected;

			if (it->second.m_markedForForceOpen)
			{
				ImGui::SetNextItemOpen(true);
				it->second.m_markedForForceOpen = false;
			}

			std::string id = "##" + (it->second).m_name;
			bool nodeOpen = ImGui::TreeNodeEx(id.c_str(), folderFlags);
			ImGui::SameLine();  WidgetsUtility::IncrementCursorPosY(5);
			WidgetsUtility::Icon(ICON_FA_FOLDER, 0.7f, ImVec4(0.9f, 0.83f, 0.0f, 1.0f));
			ImGui::SameLine(); WidgetsUtility::IncrementCursorPosX(3); WidgetsUtility::IncrementCursorPosY(-5);
			ImGui::Text((it->second).m_name.c_str());

			// Click
			if (ImGui::IsItemClicked())
			{
				s_selectedItem = (it->second).m_id;
				s_selectedFile = nullptr;
				s_selectedFolder = &(it->second);
			}

			// Hover.
			if (ImGui::IsWindowHovered() && ImGui::IsItemHovered())
				s_hoveredFolder = &(it->second);

			if (nodeOpen)
			{
				DrawFolder(it->second);
				ImGui::TreePop();
			}
			++it;

		}
		WidgetsUtility::PopStyleVar();


		// Draw files.
		for (std::map<int, EditorFile>::iterator it = folder.m_files.begin(); it != folder.m_files.end();)
		{
			WidgetsUtility::IncrementCursorPosX(-9);
			if (it->second.m_markedForErase)
			{
				// Delete directory.
				EditorUtility::DeleteDirectory(it->second.m_path);

				// Unload the resources & erase.
				UnloadFileResource(it->second);
				folder.m_files.erase(it++);
				continue;
			}

			ImGuiTreeNodeFlags fileFlags = it->second.m_id == s_selectedItem ? fileNodeFlagsSelected : fileNodeFlagsNotSelected;

			// Highlight.
			if (it->second.m_markedForHighlight)
			{
				it->second.m_markedForHighlight = false;
				s_usedFileNameColor = s_highlightColor;
				s_highlightColorSet = true;
				s_colorLerpItemID = it->second.m_id;
				s_colorLerpTimestamp = ImGui::GetTime();
			}

			if (s_highlightColorSet)
			{
				float t = ImGui::GetTime() - s_colorLerpTimestamp;
				float remapped = Math::Remap(t, 0.0f, s_colorLerpDuration, 0.0f, 1.0f);

				s_usedFileNameColor.x = Math::Lerp(s_highlightColor.x, s_fileNameColor.x, remapped);
				s_usedFileNameColor.y = Math::Lerp(s_highlightColor.y, s_fileNameColor.y, remapped);
				s_usedFileNameColor.z = Math::Lerp(s_highlightColor.z, s_fileNameColor.z, remapped);
				s_usedFileNameColor.w = Math::Lerp(s_highlightColor.w, s_fileNameColor.w, remapped);

				if (t > s_colorLerpDuration - 0.1f)
				{
					s_highlightColorSet = false;
					s_usedFileNameColor = s_fileNameColor;
				}
			}

			if (it->second.m_id == s_colorLerpItemID)
				ImGui::PushStyleColor(ImGuiCol_Text, s_usedFileNameColor);
			else
				ImGui::PushStyleColor(ImGuiCol_Text, s_fileNameColor);

			// Node
			bool nodeOpen = ImGui::TreeNodeEx(it->second.m_name.c_str(), fileFlags);

			// Drag drop
			if (it->second.m_type == FileType::Texture2D)
			{
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					// Set payload to carry the texture;
					uint32 id = LinaEngine::Graphics::Texture::GetTexture(it->second.m_path).GetID();
					ImGui::SetDragDropPayload(RESOURCES_MOVETEXTURE_ID, &id, sizeof(uint32));

					// Display preview 
					ImGui::Text("Assign ");
					ImGui::EndDragDropSource();
				}
			}

			ImGui::PopStyleColor();

			// Click.
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				s_selectedItem = it->second.m_id;
				s_selectedFolder = nullptr;
				s_selectedFile = &it->second;

				// Notify properties panel of file selection.
				if (it->second.m_type == FileType::Texture2D)
					EditorApplication::GetEditorDispatcher().DispatchAction<LinaEngine::Graphics::Texture*>(LinaEngine::Action::ActionType::TextureSelected, &LinaEngine::Graphics::Texture::GetTexture(it->second.m_path));
				else if (it->second.m_type == FileType::Mesh)
					EditorApplication::GetEditorDispatcher().DispatchAction<LinaEngine::Graphics::Mesh*>(LinaEngine::Action::ActionType::MeshSelected, &LinaEngine::Graphics::Mesh::GetMesh(it->second.m_path));
				else if (it->second.m_type == FileType::Material)
					EditorApplication::GetEditorDispatcher().DispatchAction<std::pair<EditorFile*, LinaEngine::Graphics::Material*>>(LinaEngine::Action::ActionType::MaterialSelected, std::make_pair(&it->second, &LinaEngine::Graphics::Material::GetMaterial(it->second.m_path)));
			}

			if (nodeOpen)
				ImGui::TreePop();
			++it;

		}


		// Deselect.
		if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			EditorApplication::GetEditorDispatcher().DispatchAction<void*>(LinaEngine::Action::ActionType::Unselect, 0);
			s_selectedItem = -1;
		}

		// Delete item.
		if (ImGui::IsKeyPressed(Input::InputCode::Delete) && s_selectedItem != -1)
		{
			if (s_selectedFolder != nullptr)
				s_selectedFolder->m_markedForErase = true;
			if (s_selectedFile != nullptr)
				s_selectedFile->m_markedForErase = true;

			// Deselect
			EditorApplication::GetEditorDispatcher().DispatchAction<void*>(LinaEngine::Action::ActionType::Unselect, 0);
			s_selectedItem = -1;
		}

		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
			s_hoveredFolder = nullptr;
	}

	void ResourcesPanel::LoadFolderResources(EditorFolder& folder)
	{
		LinaEngine::Graphics::RenderEngine& renderEngine = LinaEngine::Application::GetRenderEngine();

		// Load files.
		for (std::map<int, EditorFile>::iterator it = folder.m_files.begin(); it != folder.m_files.end(); ++it)
		{
			EditorFile& file = it->second;

			// SKIP FOR NOW BC WE NEED TO MAKE SURE WE HANDLE BOTH ENGINE CREATION & EDITOR CREATION
			if (file.m_type == FileType::Texture2D)
			{
				bool textureExists = LinaEngine::Graphics::Texture::TextureExists(file.m_path);

				if (!textureExists)
				{
					LinaEngine::Graphics::SamplerParameters samplerParams;
					std::string samplerParamsPath = file.m_pathToFolder + EditorUtility::RemoveExtensionFromFilename(file.m_name) + ".samplerparams";

					if (LinaEngine::Utility::FileExists(samplerParamsPath))
						samplerParams = LinaEngine::Graphics::Texture::LoadParameters(samplerParamsPath);

					LinaEngine::Graphics::Texture::CreateTexture2D(file.m_path, samplerParams, false, false, samplerParamsPath);

					LinaEngine::Graphics::Texture::SaveParameters(samplerParamsPath, samplerParams);
				}

			}
			if (file.m_type == FileType::Material)
			{
				bool materialExists = LinaEngine::Graphics::Material::MaterialExists(file.m_path);
				if (!materialExists)
					LinaEngine::Graphics::Material::LoadMaterialFromFile(file.m_path);

			}
			else if (file.m_type == FileType::Mesh)
			{
				bool meshExists = LinaEngine::Graphics::Mesh::MeshExists(file.m_path);

				if (!meshExists)
				{
					LinaEngine::Graphics::MeshParameters meshParams;
					std::string meshParamsPath = file.m_pathToFolder + EditorUtility::RemoveExtensionFromFilename(file.m_name) + ".meshparams";

					if (LinaEngine::Utility::FileExists(meshParamsPath))
						meshParams = LinaEngine::Graphics::Mesh::LoadParameters(meshParamsPath);

					LinaEngine::Graphics::Mesh::CreateMesh(file.m_path, meshParams, -1, meshParamsPath);

					LinaEngine::Graphics::Mesh::SaveParameters(meshParamsPath, meshParams);
				}
			}
		}

		// Recursively load subfolders.
		for (std::map<int, EditorFolder>::iterator it = folder.m_subFolders.begin(); it != folder.m_subFolders.end(); ++it)
			LoadFolderResources(it->second);
	}

	void ResourcesPanel::LoadFolderDependencies(EditorFolder& folder)
	{
		LinaEngine::Graphics::RenderEngine& renderEngine = LinaEngine::Application::GetRenderEngine();

		// Load files.
		for (std::map<int, EditorFile>::iterator it = folder.m_files.begin(); it != folder.m_files.end(); ++it)
		{
			EditorFile& file = it->second;

			// SKIP FOR NOW BC WE NEED TO MAKE SURE WE HANDLE BOTH ENGINE CREATION & EDITOR CREATION
			if (file.m_type == FileType::Texture2D)
			{

			}
			if (file.m_type == FileType::Material)
			{
				LinaEngine::Graphics::Material& mat = LinaEngine::Graphics::Material::GetMaterial(file.m_path);
				mat.PostLoadMaterialData(renderEngine);
			}
			else if (file.m_type == FileType::Mesh)
			{

			}
		}

		// Recursively load subfolders.
		for (std::map<int, EditorFolder>::iterator it = folder.m_subFolders.begin(); it != folder.m_subFolders.end(); ++it)
			LoadFolderDependencies(it->second);
	}

	void ResourcesPanel::UnloadFileResource(EditorFile& file)
	{
		LinaEngine::Graphics::RenderEngine& renderEngine = LinaEngine::Application::GetRenderEngine();
		if (file.m_type == FileType::Texture2D)
			LinaEngine::Graphics::Texture::UnloadTextureResource(LinaEngine::Graphics::Texture::GetTexture(file.m_path).GetID());
		else if (file.m_type == FileType::Mesh)
			LinaEngine::Graphics::Mesh::UnloadMeshResource(LinaEngine::Graphics::Mesh::GetMesh(file.m_path).GetID());
		else if (file.m_type == FileType::Material)
			LinaEngine::Graphics::Material::UnloadMaterialResource(LinaEngine::Graphics::Material::GetMaterial(file.m_path).GetID());
	}

	void ResourcesPanel::UnloadFileResourcesInFolder(EditorFolder& folder)
	{
		for (std::map<int, EditorFile>::iterator it = folder.m_files.begin(); it != folder.m_files.end(); ++it)
			UnloadFileResource(it->second);
	}

	bool ResourcesPanel::ExpandFileResource(EditorFolder& folder, const std::string& path, FileType type)
	{
		for (auto& file : folder.m_files)
		{
			if (type == FileType::Unknown || (type != FileType::Unknown && file.second.m_type == type))
			{
				if (file.second.m_path.compare(path) == 0)
				{
					file.second.m_markedForHighlight = true;
					folder.m_markedForForceOpen = true;

					EditorFolder* parent = folder.m_parent;
					while (parent != nullptr)
					{
						parent->m_markedForForceOpen = true;

						parent = parent->m_parent;
					}

					return true;
				}
			}
		}

		for (auto& subFolder : folder.m_subFolders)
		{
			if (ExpandFileResource(subFolder.second, path, type))
				break;
		}
	}

	FileType ResourcesPanel::GetFileType(std::string& extension)
	{
		if (extension.compare("jpg") == 0 || extension.compare("jpeg") == 0 || extension.compare("png") == 0 || extension.compare("tga") == 0)
			return FileType::Texture2D;
		else if (extension.compare("ttf") == 0)
			return FileType::Font;
		else if (extension.compare("obj") == 0 || extension.compare("fbx") == 0 || extension.compare("3ds") == 0)
			return FileType::Mesh;
		else if (extension.compare("mat") == 0)
			return FileType::Material;
		else
			return FileType::Unknown;
	}

	void ResourcesPanel::MaterialTextureSelected(LinaEngine::Graphics::Texture* texture)
	{
		ExpandFileResource(m_resourceFolders[0], texture->GetPath(), FileType::Texture2D);
	}

	void ResourcesPanel::TextureReimported(std::pair<LinaEngine::Graphics::Texture*, LinaEngine::Graphics::Texture*> texturePair)
	{
		VerifyMaterialFiles(m_resourceFolders[0], texturePair);
	}

	bool ResourcesPanel::VerifyMaterialFiles(EditorFolder& folder, std::pair<LinaEngine::Graphics::Texture*, LinaEngine::Graphics::Texture*> textures)
	{
		// Iterate the files in this folder first & see if there is a match.
		for (auto file : folder.m_files)
		{
			if (file.second.m_type == FileType::Material)
			{
				LinaEngine::Graphics::Material& mat = LinaEngine::Graphics::Material::GetMaterial(file.second.m_path);
				for (auto sampler : mat.m_sampler2Ds)
				{
					if (sampler.second.m_boundTexture == textures.first)
					{
						mat.SetTexture(sampler.first, textures.second, sampler.second.m_bindMode);
						return true;
					}
				}

			}
		}

		// Iterate subfolders.
		for (auto folder : folder.m_subFolders)
		{
			if (VerifyMaterialFiles(folder.second, textures))
				return true;
		}

		return false;
	}
}