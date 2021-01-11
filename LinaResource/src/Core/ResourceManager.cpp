/*
This file is a part of: Lina Engine
https://github.com/inanevin/Lina

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


#include "Core/ResourceManager.hpp"  
#include "Core/Log.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Core/ResourcesCommon.hpp"
#include "Resources/MaterialResource.hpp"
#include "Utility/FileUtility.hpp"

namespace Lina::Resources
{
	static float t = 0.0f;
	static bool loaded = false;
	void ResourceManager::DebugLevelLoad(Event::ETick& e)
	{
		t += e.m_deltaTime;
		if (!loaded && t > 3.0f)
		{
			loaded = true;
			ExportLevel("", "default");
		}
	}
	ResourceManager::~ResourceManager()
	{
		m_eventSys->Disconnect<Event::EPreMainLoop>(this);
		m_eventSys->Disconnect<Event::EPostMainLoop>(this);
		m_eventSys->Disconnect<Event::ETick>(this);
		m_eventSys->Disconnect<Event::EAppLoad>(this);
	}

	void ResourceManager::SetReferences(Event::EventSystem* eventSys, ECS::Registry* ecs)
	{
		m_eventSys = eventSys;
		m_ecs = ecs;
		m_eventSys->Connect<Event::EPreMainLoop, &ResourceManager::OnPreMainLoop>(this);
		m_eventSys->Connect<Event::EAppLoad, &ResourceManager::OnAppLoad>(this);
		m_eventSys->Connect<Event::EPostMainLoop, &ResourceManager::OnPostMainLoop>(this);
		m_eventSys->Connect<Event::ETick, &ResourceManager::DebugLevelLoad>(this);
	}

	void ResourceManager::OnPreMainLoop(Event::EPreMainLoop& e)
	{
		LINA_TRACE("[Resource Manager] -> Startup");

		
	}

	void ResourceManager::OnPostMainLoop(Event::EPostMainLoop& e)
	{
		// Make sure we don't have any packing/unpacking going on.
		if (m_future.valid())
		{
			m_future.cancel();
			m_future.get();
		}

		LINA_TRACE("[Resource Manager] -> Shutdown");
	}

	void ResourceManager::OnAppLoad(Event::EAppLoad& e)
	{
		m_appMode = e.m_appInfo->m_appMode;

		m_activeLevel.AddUsedResource("Resources/test.wav");
		m_activeLevel.AddUsedResource("Resources/Shaders/frag.spv");
		m_activeLevel.AddUsedResource("Resources/Shaders/vert.spv");
		m_activeLevel.AddUsedResource("Resources/monkey.filamesh");

		// If we are in editor, fill our resource bundle with all the files imported in the project's Resources directory.
		if (m_appMode == ApplicationMode::Editor || m_appMode == ApplicationMode::EditorGame)
		{
			m_currentProgressData.m_state = ResourceProgressState::Pending;

			m_taskflow.emplace([=]()
				{
					// Find resources.
					FileUtility::Folder root;
					root.m_fullPath = "Resources/";
					FileUtility::ScanFolder(root, true);

					// Set progress & fill.
					m_currentProgressData.m_progressTitle = "Loading resources...";
					m_currentProgressData.m_state = ResourceProgressState::InProgress;
					m_currentProgressData.m_currentProgress = 0.0f;

					// Start filling preceeded & followed by an event dispatch.
					m_eventSys->Trigger<Event::EResourceProgressStarted>();
					FillBundleForEditor(root, &m_currentProgressData);

					// Unload all processed packages since the listeners of resource loaders already uploaded the necessary buffers to cpu.
					// From the packages to necessary memory blocks.
					m_bundle.UnloadProcessedPackages();

					// Notify listeners that unpacking has finished.
					m_currentProgressData.m_state = ResourceProgressState::None;
					m_eventSys->Trigger<Event::EResourceProgressEnded>();
				});

			m_future = m_executor.run(m_taskflow);
		}
		else
			ImportLevel("", "default");
	}

	void ResourceManager::ImportLevel(const std::string& path, const std::string& levelName)
	{
		// TODO: Unload current level.

		// Make sure we set state before multithreading.
		std::string fullLevelPath = path + levelName + ".linalevel";
		m_currentProgressData.m_state = ResourceProgressState::InProgress;
		m_currentProgressData.m_progressTitle = "Importing level...";
		m_currentProgressData.m_currentResourceName = fullLevelPath;

		// Make sure we don't have any packing/unpacking going on.
		m_taskflow.clear();
		m_executor.wait_for_all();

		// LoadFromFile the level.
		m_activeLevel.LoadFromFile(fullLevelPath, *m_ecs);

		// If not in editor, load & unpack the bundle associated with this level, on a seperate thread.
		if (m_appMode == ApplicationMode::Standalone)
		{
			std::string fullBundlePath = path + levelName + ".linabundle";
			m_currentProgressData.m_progressTitle = "Unpacking level resources...";
			m_currentProgressData.m_currentResourceName = fullBundlePath;

			m_taskflow.emplace([=]()
				{
					// Start unpacking process, preceeded & followed by an event dispatch.
					m_eventSys->Trigger<Event::EResourceProgressStarted>();

					// Start unpacking.
					std::unordered_map<std::string, ResourceType> unpackedResources;
					m_packager.Unpack(fullBundlePath, PACKAGE_PASS, &m_bundle, &m_currentProgressData, unpackedResources);

					// Create processed resource instances from raw data.
					m_bundle.ProcessRawPackages(m_eventSys);

					// Unload all processed packages since the listeners of AddResourceReference already uploaded the buffers
					// From the packages to necessary memory blocks.
					m_bundle.UnloadProcessedPackages();

					// Set progress end.
					m_currentProgressData.m_state = ResourceProgressState::None;
					m_currentProgressData.m_progressTitle = "";
					m_currentProgressData.m_currentResourceName = "";
					m_currentProgressData.m_currentProgress = 0;

					// Notify listeners that unpacking has finished.
					m_eventSys->Trigger<Event::EResourceProgressEnded>();

					// Notify listeners of level load.
					m_eventSys->Trigger<Event::ELevelLoaded>();
				});

			m_future = m_executor.run(m_taskflow);
		}
		else
			m_eventSys->Trigger<Event::ELevelLoaded>();

	}

	void ResourceManager::ExportLevel(const std::string& path, const std::string& levelName)
	{
		// Make sure we don't have any packing/unpacking going on.
		m_taskflow.clear();
		m_executor.wait_for_all();

		// Pack the bundle associated with this level, on a seperate thread.
		m_taskflow.emplace([=]()
			{
				m_eventSys->Trigger<Event::EResourceProgressStarted>();

				// Export level first.
				m_currentProgressData.m_state = ResourceProgressState::InProgress;
				m_currentProgressData.m_progressTitle = "Exporting level...";
				m_currentProgressData.m_currentResourceName = levelName + ".linalevel";
				m_activeLevel.Export(path + levelName + ".linalevel", *m_ecs);

				// Find out which resources to pack.
				m_currentProgressData.m_progressTitle = "Packing resources...";
				std::vector<std::string> filesToPack;
				for (auto& resource : m_activeLevel.m_usedResources)
					filesToPack.push_back(resource.data());

				m_packager.PackageFileset(filesToPack, path + levelName + ".linabundle", PACKAGE_PASS, &m_currentProgressData);
				m_eventSys->Trigger<Event::EResourceProgressEnded>();
			});

		m_future = m_executor.run(m_taskflow);

	}

	void ResourceManager::AddResourceReference(const std::string& path, ResourceType type)
	{
		m_activeLevel.AddUsedResource(path);
	}

	void ResourceManager::RemoveResourceReference(const std::string& path, ResourceType type)
	{
		m_activeLevel.RemoveUsedResource(path);
	}

	void ResourceManager::FillBundleForEditor(FileUtility::Folder& root,ResourceProgressData* progData)
	{
		for (auto& folder : root.m_folders)
		{
			FillBundleForEditor(folder, progData);
		}

		// InitializeVulkan each file into memory where they will persist during the editor lifetime.
		for (auto& file : root.m_files)
		{
			ResourceType resType = GetResourceType(file.m_extension);
			m_bundle.FillProcessedPackage(file.m_fullPath, resType, progData, m_eventSys);
		}
	}

}
