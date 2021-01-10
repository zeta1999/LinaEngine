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
Class: RenderEngineFilament

Render engine based on Google Filament backend.

Timestamp: 1/7/2021 1:32:01 PM
*/

#pragma once

#ifndef RenderEngineFilament_HPP
#define RenderEngineFilament_HPP

// Headers here.
#include "Core/Common.hpp"
#include "ECS/ECS.hpp"
#include "EventSystem/Events.hpp"
#include "WindowFilament.hpp"
#include <unordered_map>
#include <set>
#include <filameshio/MeshReader.h>


namespace Lina
{
	namespace Event
	{
		class EventSystem;
	}

	namespace Resources
	{
		class ResourceManager;
	}
}

namespace filament
{
	class Engine;
	class SwapChain;
	class Renderer;
	class Camera;
	class View;
	class Scene;
	class TransformManager;
	class RenderableManager;
	class Material;
};


namespace utils
{
	class EntityManager;
};

namespace Lina::Graphics
{
	class RenderEngineFilament
	{
		
	public:
		
		WindowFilament& GetWindow() { return m_window; }
		inline filament::Engine* GetEngine() { return m_engine; }
		inline filament::View* GetGameView() { return m_gameView; }
		inline filament::View* GetUIView() { return m_uiView; }

	private:

		friend class Application;

		RenderEngineFilament& operator=(const RenderEngineFilament&) = delete;
		RenderEngineFilament(const RenderEngineFilament&) = delete;
		RenderEngineFilament() {};
		~RenderEngineFilament();

		void SetReferences(Event::EventSystem* eventSys, ECS::Registry* ecs, Resources::ResourceManager* m_resources);
	
	private:

		void OnAppLoad(Event::EAppLoad& e);
		void OnPreMainLoop(Event::EPreMainLoop& e);
		void OnPostMainLoop(Event::EPostMainLoop& e);
		void OnWindowResize(Event::EWindowResized& e);
		void OnMeshResourceLoaded(Event::EMeshResourceLoaded& e);
		void OnMaterialResourceLoaded(Event::EMaterialResourceLoaded& e);
		void Tick();
		void Render();
		void DisconnectEvents();
		void AddMeshData();
		void AddMaterialData();
	
	private:

		bool m_initialized = false;
		Event::EventSystem* m_eventSys = nullptr;
		ECS::Registry* m_ecs = nullptr;
		Resources::ResourceManager* m_resources = nullptr;
		WindowFilament m_window;
		filament::Engine* m_engine = nullptr;
		filament::SwapChain* m_swapchain = nullptr;
		filament::Renderer* m_renderer = nullptr;
		filament::Camera* m_gameCamera = nullptr;
		filament::Camera* m_uiCamera = nullptr;
		filament::View* m_gameView = nullptr;
		filament::View* m_uiView = nullptr;
		filament::Scene* m_gameScene = nullptr;
		filament::TransformManager* m_transformManager = nullptr;
		filament::RenderableManager* m_renderableManager = nullptr;
		filament::Material* m_defaultMaterial = nullptr;
		filament::MaterialInstance* m_defaultMaterialInstance = nullptr;
		utils::EntityManager* m_entityManager = nullptr;
		std::unordered_map < StringIDType, std::vector<unsigned char>> m_meshesToLoadStandalone;
		std::unordered_map < StringIDType, std::vector<unsigned char>> m_standaloneMeshBuffer;
		std::unordered_map < StringIDType, std::string> m_meshesToLoadEditor;
		std::unordered_map < StringIDType, std::vector<unsigned char>> m_materialBuffer;
		std::unordered_map<StringIDType, filamesh::MeshReader::Mesh> m_loadedMeshes;
		std::unordered_map<StringIDType, filament::Material*> m_loadedMaterials;
		ApplicationInfo* m_appInfo = nullptr;
	};
}

#endif
