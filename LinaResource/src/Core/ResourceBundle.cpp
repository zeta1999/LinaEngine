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

#include "Core/ResourceBundle.hpp"
#include "Resources/ImageResource.hpp"
#include "Resources/MeshResource.hpp"
#include "Resources/AudioResource.hpp"
#include "Resources/MaterialResource.hpp"
#include "Resources/ShaderResource.hpp"
#include "Resources/MetadataResource.hpp"
#include "Core/Log.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Utility/FileUtility.hpp"

namespace Lina::Resources
{
	void ResourceBundle::UnloadRawPackages()
	{
		for (auto& p : m_rawPackages)
			p.second.clear();

		m_rawPackages.clear();
			
		LINA_TRACE("[Resource Bundle] -> Raw bundle unloaded.");
	}

	void ResourceBundle::UnloadProcessedPackages()
	{
		// Clear in-active packages only. (those whose buffers are immediately send to hardware.)
		for (auto& r : m_imagePackage)
			delete r.second;

		for (auto& r : m_audioPackage)
			delete r.second;

		for (auto& r : m_meshPackage)
			delete r.second;

		for (auto& r : m_materialPackage)
			delete r.second;

		for (auto& r : m_shaderPackage)
			delete r.second;

		for (auto& r : m_metaPackage)
			delete r.second;

		m_imagePackage.clear();
		m_audioPackage.clear();
		m_meshPackage.clear();
		m_materialPackage.clear();
		m_shaderPackage.clear();
		m_metaPackage.clear();
		/* DO NOT delete material & shader resources, as they are considered active resources and need to stay in-memory.*/
	}

	// Packages that are loaded in runtime.
	void ResourceBundle::ProcessRawPackages(Event::EventSystem* eventSys)
	{
		for (auto& package : m_rawPackages)
		{
			RawPackage& rawPackage = package.second;

			// For every resource in this package, 
			for (auto& resource : rawPackage)
			{
				// Initialize the corresponding package class from memory.
				if (package.first == ResourceType::Image)
				{
					ImageResource* img = new ImageResource();
					if (img->LoadFromMemory(resource.first, &resource.second[0], resource.second.size(), eventSys))
						m_imagePackage[resource.first] = img;
					else
						delete img;
				}
				else if (package.first == ResourceType::Mesh)
				{
					MeshResource* mesh = new MeshResource();
					if(mesh->LoadFromMemory(resource.first, &resource.second[0], resource.second.size(), eventSys))
						m_meshPackage[resource.first] = mesh;
					else
						delete mesh;
				}
				else if (package.first == ResourceType::Audio)
				{
					AudioResource* aud = new AudioResource();
					if (aud->LoadFromMemory(resource.first, &resource.second[0], resource.second.size(), eventSys))
						m_audioPackage[resource.first] = aud;
					else 
						delete aud;
				}
				else if (package.first == ResourceType::Material)
				{
					MaterialResource* mat = new MaterialResource();
					if (mat->LoadFromMemory(resource.first, &resource.second[0], resource.second.size(), eventSys))
						m_materialPackage[resource.first] = mat;
					else
						delete mat;
				}
				else if (package.first == ResourceType::ImageMeta || package.first == ResourceType::MeshMeta || package.first == ResourceType::MaterialMeta)
				{
					MetadataResource* meta = new MetadataResource();
					if (meta->LoadFromMemory(package.first, resource.first, &resource.second[0], resource.second.size(), eventSys))
						m_metaPackage[resource.first] = meta;
					else
						delete meta;
				}
			
			}
		}

		LINA_TRACE("[Resource Bundle] -> Raw bundle processed into packages.");
		UnloadRawPackages();
	}

	// Packages that are loaded in editor.
	void ResourceBundle::FillProcessedPackage(const std::string& path, ResourceType type, ResourceProgressData* progData, Event::EventSystem* eventSys)
	{
		progData->m_currentResourceName = path;
		// Initialize the corresponding package class from memory.
		if (type == ResourceType::Image)
		{
			ImageResource* img = new ImageResource();
			if (img->LoadFromFile(path, eventSys))
			{
				StringIDType imgSid = StringID(path.c_str()).value();
				m_imagePackage[imgSid] = img;

				std::string metaPath = FileUtility::RemoveExtensionFromFileName(path) + std::string(LINAIMAGE_EXT);
				if (FileUtility::FileExists(metaPath))
				{
					MetadataResource* meta = new MetadataResource();
					meta->LoadFromFile(ResourceType::ImageMeta, imgSid, metaPath, eventSys);
					m_metaPackage[StringID(metaPath.c_str()).value()] = meta;
				}
			}
			else
				delete img;
		}
		else if (type == ResourceType::Mesh)
		{
			MeshResource* mesh = new MeshResource();
			if (mesh->LoadFromFile(path, eventSys))
			{
				StringIDType meshSid = StringID(path.c_str()).value();
				m_meshPackage[meshSid] = mesh;

				std::string metaPath = FileUtility::RemoveExtensionFromFileName(path) + std::string(LINAMESH_EXT);
				if (FileUtility::FileExists(metaPath))
				{
					MetadataResource* meta = new MetadataResource();
					meta->LoadFromFile(ResourceType::MeshMeta, meshSid, metaPath, eventSys);
					m_metaPackage[StringID(metaPath.c_str()).value()] = meta;
				}
			}
			else
				delete mesh;
		}
		else if (type == ResourceType::Material)
		{
			MaterialResource* mat = new MaterialResource();
			if (mat->LoadFromFile(path, eventSys))
			{
				StringIDType matSid = StringID(path.c_str()).value();
				m_materialPackage[matSid] = mat;

				std::string metaPath = FileUtility::RemoveExtensionFromFileName(path) + std::string(LINAMAT_EXT);
				if (FileUtility::FileExists(metaPath))
				{
					MetadataResource* meta = new MetadataResource();
					meta->LoadFromFile(ResourceType::MaterialMeta, matSid, metaPath, eventSys);
					m_metaPackage[StringID(metaPath.c_str()).value()] = meta;
				}
			}
			else
				delete mat;
		}	
		else if (type == ResourceType::Audio)
		{
			AudioResource* aud = new AudioResource();
			if (aud->LoadFromFile(path, eventSys))
				m_audioPackage[StringID(path.c_str()).value()] = aud;
			else
				delete aud;
		}
	}
}