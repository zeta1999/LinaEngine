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

/*
Class: ResourcesPanel

Displays the project folders & files, creates functionality to edit, add, remove them.

Timestamp: 6/5/2020 12:54:52 AM
*/

#pragma once

#ifndef ResourcesPanel_HPP
#define ResourcesPanel_HPP

#include "Panels/EditorPanel.hpp"
#include "Utility/EditorUtility.hpp"
#include <map>
#include <vector>

namespace LinaEngine
{
	namespace Graphics
	{
		class RenderEngine;
		class Texture;
	}
}

namespace LinaEditor 
{

	class PropertiesPanel;

	class ResourcesPanel : public EditorPanel
	{

	public:

		
		ResourcesPanel() {};
		virtual ~ResourcesPanel() {};
	
		virtual void Setup() override;
		virtual void Draw() override;

	private:
	
		void ScanRoot();
		void DrawContextMenu();
		void ScanFolder(EditorFolder& folder);
		void DrawFolder(EditorFolder& folder, bool isRoot = false);
		void LoadFolderResources(EditorFolder& folder);
		void LoadFolderDependencies(EditorFolder& folder);
		void UnloadFileResource(EditorFile& file);
		void UnloadFileResourcesInFolder(EditorFolder& folder);
		bool ExpandFileResource(EditorFolder& folder, const std::string& path, FileType type = FileType::Unknown);
		FileType GetFileType(std::string& extension);
		void MaterialTextureSelected(LinaEngine::Graphics::Texture* texture);
		void TextureReimported(std::pair<LinaEngine::Graphics::Texture*, LinaEngine::Graphics::Texture*> textures);
		bool VerifyMaterialFiles(EditorFolder& folder, std::pair<LinaEngine::Graphics::Texture*, LinaEngine::Graphics::Texture*> textures);

	private:

		std::vector<EditorFolder> m_resourceFolders;
	};
}

#endif
