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

#include "Core/Backend/Filament/RenderEngineFilament.hpp"
#include "Profiling/Profiler.hpp"
#include "Core/Log.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Backend/Filament/FilaImage.hpp"
#include <filament/FilamentAPI.h>
#include <filament/Engine.h>
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/RenderableManager.h>
#include <filament/Scene.h>
#include <filament/Skybox.h>
#include <filament/Renderer.h>
#include <filament/TransformManager.h>
#include <filament/VertexBuffer.h>
#include <filament/View.h>
#include <filament/Texture.h>
#include <utils/EntityManager.h>
#include <filament/Material.h>
#include <cmath>
#include <filament/Viewport.h>
#include <utils/Path.h>
#include <filament/LightManager.h>
#include <filament/RenderTarget.h>
#include "Core/Backend/Filament/MaterialResources/materialresources.h"
#include "Core/Backend/Filament/MaterialResources/monkey.h"
#include "Math/Math.hpp"
#include <utils/Path.h>


using namespace filament;
using utils::Entity;
using utils::EntityManager;

struct Vertex {
	filament::math::float2 position;
	uint32_t color;
};

#define M_PI 3.14159265358979323846
static const Vertex TRIANGLE_VERTICES[3] = {
	{{1, 0}, 0xffff0000u},
	{{cos(M_PI * 2 / 3), sin(M_PI * 2 / 3)}, 0xff00ff00u},
	{{cos(M_PI * 4 / 3), sin(M_PI * 4 / 3)}, 0xff0000ffu},
};
static constexpr uint16_t TRIANGLE_INDICES[3] = { 0, 1, 2 };

namespace Lina::Graphics
{
	
	TransformManager::Instance ti2;


	RenderEngineFilament::~RenderEngineFilament()
	{
		if (m_initialized)
			DisconnectEvents();

	}

	void RenderEngineFilament::DisconnectEvents()
	{
		m_eventSys->Disconnect<Event::EPreMainLoop>(this);
		m_eventSys->Disconnect<Event::EPostMainLoop>(this);
		m_eventSys->Disconnect<Event::ERender>(this);
		m_eventSys->Disconnect<Event::EAppLoad>(this);
		m_eventSys->Disconnect<Event::ETick>(this);
		m_eventSys->Disconnect<Event::EWindowResized>(this);
		m_eventSys->Disconnect<Event::EMeshResourceLoaded>(this);
		m_eventSys->Disconnect<Event::EMaterialResourceLoaded>(this);
		m_eventSys->Disconnect<Event::EImageResourceLoaded>(this);
	}


	void RenderEngineFilament::SetReferences(Event::EventSystem* eventSys, ECS::Registry* ecs, Resources::ResourceManager* resources)
	{
		m_eventSys = eventSys;
		m_ecs = ecs;
		m_resources = resources;
		m_eventSys->Connect<Event::EPreMainLoop, &RenderEngineFilament::OnPreMainLoop>(this);
		m_eventSys->Connect<Event::EPostMainLoop, &RenderEngineFilament::OnPostMainLoop>(this);
		m_eventSys->Connect<Event::ERender, &RenderEngineFilament::Render>(this);
		m_eventSys->Connect<Event::EAppLoad, &RenderEngineFilament::OnAppLoad>(this);
		m_eventSys->Connect<Event::ETick, &RenderEngineFilament::Tick>(this);
		m_eventSys->Connect<Event::EWindowResized, &RenderEngineFilament::OnWindowResize>(this);
		m_eventSys->Connect<Event::EMeshResourceLoaded, &RenderEngineFilament::OnMeshResourceLoaded>(this);
		m_eventSys->Connect<Event::EMaterialResourceLoaded, &RenderEngineFilament::OnMaterialResourceLoaded>(this);
		m_eventSys->Connect<Event::EImageResourceLoaded, &RenderEngineFilament::OnImageResourceLoaded>(this);
	}

	void RenderEngineFilament::OnAppLoad(Event::EAppLoad& e)
	{
		m_appInfo = e.m_appInfo;
		
		// First create empty window context.
		m_window.SetReferences(m_eventSys, e.m_appInfo->m_windowProperties);
		m_window.CreateContext();

		// Now create engine, swapchaind and a renderer.
		m_engine = filament::Engine::create();
		m_swapchain = m_engine->createSwapChain(m_window.GetNativeWindow());
		m_renderer = m_engine->createRenderer();
	
		// Create the primary camera, game view & scene, & get manager references.
		m_gameCamera = m_engine->createCamera(utils::EntityManager::get().create());
		m_uiCamera = m_engine->createCamera(utils::EntityManager::get().create());
		m_gameView = m_engine->createView();
		m_uiView = m_engine->createView();
		m_gameScene = m_engine->createScene();
		m_transformManager = &m_engine->getTransformManager();
		m_entityManager = &utils::EntityManager::get();
		m_renderableManager = &m_engine->getRenderableManager();

		// Configure game scene, camera & view.
		auto sb = Skybox::Builder().color(math::float4{ 0.0735f, 0.035f, 0.035f, 1.0f }).showSun(true).build(*m_engine);	
		e.m_appInfo->m_windowProperties.m_width = 360;
		e.m_appInfo->m_windowProperties.m_height = 450;
		double aspect = ((double)e.m_appInfo->m_windowProperties.m_width / (double)e.m_appInfo->m_windowProperties.m_height);
		m_gameScene->setSkybox(sb);

		m_gameCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
		m_gameCamera->setExposure(100.0f);
		m_gameCamera->setProjection(90.0f, aspect, 0.1f, 1000.0f);	
		m_gameCamera->lookAt({ 0, 0, -10 }, { 0, 0, 0 }, { 0, 1, 0 });
		m_gameView->setViewport({ 360, 0, (uint32_t)e.m_appInfo->m_windowProperties.m_width, (uint32_t)e.m_appInfo->m_windowProperties.m_height });
		m_gameView->setScene(m_gameScene);
		m_gameView->setCamera(m_gameCamera); 
		m_gameView->setName("game-view");


		m_uiCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
		m_uiCamera->setProjection(Camera::Projection::ORTHO, 0.0, (uint32_t)e.m_appInfo->m_windowProperties.m_width, (uint32_t)e.m_appInfo->m_windowProperties.m_height, 0.0, 0.0, 1.0);
		m_uiView->setCamera(m_uiCamera);
		m_uiView->setName("ui-view");
		m_uiView->setViewport({ 0, 0, (uint32_t)e.m_appInfo->m_windowProperties.m_width, (uint32_t)e.m_appInfo->m_windowProperties.m_height });

		m_initialized = true;

		m_defaultMaterial = Material::Builder().package(MATERIALRESOURCES_AIDEFAULTMAT_DATA, MATERIALRESOURCES_AIDEFAULTMAT_SIZE).build(*m_engine);
		m_defaultMaterialInstance = m_defaultMaterial->createInstance();
		m_defaultMaterialInstance->setParameter("baseColor", RgbType::LINEAR, filament::math::float3{ 0.5, 0.9, 0.1 });
		m_defaultMaterialInstance->setParameter("metallic", 1.0f);
		m_defaultMaterialInstance->setParameter("roughness", 1.0f);
		m_defaultMaterialInstance->setParameter("reflectance", 1.0f);

		utils::Entity light;

		light = m_entityManager->create();
		LightManager::Builder(LightManager::Type::SUN)
			.color(Color::toLinear<ACCURATE>(sRGBColor(0.98f, 0.92f, 0.89f)))
			.intensity(110)
			.direction({ 0.7, 1, -0.8 })
			.sunAngularRadius(1.9f)
			.castShadows(true)
			.build(*m_engine, light);
		m_gameScene->addEntity(light);
		
	}
	void RenderEngineFilament::OnPreMainLoop(Event::EPreMainLoop& e)
	{
		LINA_TRACE("[Render Engine Filament] -> Startup");

	}
	void RenderEngineFilament::OnPostMainLoop(Event::EPostMainLoop& e)
	{
		LINA_TRACE("[Render Engine Filament] -> Shutdown");

		
	}
	void RenderEngineFilament::OnWindowResize(Event::EWindowResized& e)
	{
		m_gameView->setViewport({ 0, 0, (uint32_t)e.m_windowProps.m_width, (uint32_t)e.m_windowProps.m_height });
		double aspect = (double)((double)e.m_windowProps.m_width / (double)e.m_windowProps.m_height);
		m_gameCamera->setProjection(90.0f, aspect, 0.1f, 1000.0f);
	}

	void RenderEngineFilament::OnMeshResourceLoaded(Event::EMeshResourceLoaded& e)
	{
		if (m_appInfo->m_appMode == ApplicationMode::Standalone)
			m_meshesToLoadStandalone[e.m_sid] = e.m_data;
		else
			m_meshesToLoadEditor[e.m_sid] = e.m_path;
	}

	void RenderEngineFilament::OnMaterialResourceLoaded(Event::EMaterialResourceLoaded& e)
	{
		m_materialBuffer[e.m_sid] = e.m_data;
	}

	void RenderEngineFilament::OnImageResourceLoaded(Event::EImageResourceLoaded& e)
	{
		FilaImage* image = new FilaImage(e);
		Texture::PixelBufferDescriptor buffer(e.m_data, size_t(e.m_width * e.m_height * 4), Texture::Format::RGBA, Texture::Type::UBYTE, (Texture::PixelBufferDescriptor::Callback)&stbi_image_free);
		Texture* tex = Texture::Builder().width(uint32_t(e.m_width)).height(uint32_t(e.m_height)).levels(1).sampler(Texture::Sampler::SAMPLER_2D).format(Texture::InternalFormat::RGBA8).build(*m_engine);
		
	}

	static float t = 0.0f;
	static bool once = false;
	void RenderEngineFilament::Tick()
	{
		PROFILER_FUNC();

		m_materialBuffer.clear();
		// Check if any resource is requested to be loaded & load them if so.
		AddMeshData();
		AddMaterialData();

		t += 0.0002f;		
		auto& tcm = m_engine->getTransformManager();
		// math::mat4f transform2 = filament::math::mat4f{ filament::math::mat3f(1), filament::math::float3(-1, Math::Sin(t) * 2, 0) };
		m_gameCamera->lookAt({ 0, 0, 3 }, { 0, 0, 0 }, { 0, 1, 0 });
	}
	void RenderEngineFilament::Render()
	{
		PROFILER_FUNC();


		m_eventSys->Trigger<Event::EPreRender>();

		if (m_renderer->beginFrame(m_swapchain)) 
		{
			// for each View

			m_renderer->render(m_gameView);
			m_renderer->render(m_uiView);

			m_renderer->endFrame();
		}

		m_eventSys->Trigger<Event::EPostRender>();

	}

	void RenderEngineFilament::AddMeshData()
	{
		if (m_appInfo->m_appMode == ApplicationMode::Standalone)
		{
			if (m_meshesToLoadStandalone.size() == 0) return;

			auto it = m_meshesToLoadStandalone.begin();

			while (it != m_meshesToLoadStandalone.end())
			{
				m_standaloneMeshBuffer[it->first] = it->second;
				m_loadedMeshes.emplace(it->first, filamesh::MeshReader::loadMeshFromBuffer(m_engine, m_standaloneMeshBuffer[it->first].data(), nullptr, nullptr, m_defaultMaterialInstance));
				filamesh::MeshReader::Mesh* mesh = &m_loadedMeshes[it->first];

				auto ti = m_transformManager->getInstance(mesh->renderable);
				math::mat4f transform = filament::math::mat4f{ filament::math::mat3f(1), filament::math::float3(0, 0, 0) };
				math::mat4f transform2 = filament::math::mat4f{ filament::math::mat3f(1), filament::math::float3(-1, -1.1f, 0) };
				m_transformManager->setTransform(ti, transform);
				m_transformManager->setTransform(ti2, transform2);
				m_renderableManager->setCastShadows(m_renderableManager->getInstance(mesh->renderable), true);

				m_gameScene->addEntity(mesh->renderable);

				it = m_meshesToLoadStandalone.erase(it);
			}
		}
		else
		{
			if (m_meshesToLoadEditor.size() == 0) return;

			auto it = m_meshesToLoadEditor.begin();

			while (it != m_meshesToLoadEditor.end())
			{
				utils::Path path = it->second;
				filamesh::MeshReader::MaterialRegistry reg;
				reg.registerMaterialInstance(utils::CString("DefaultMaterial"), m_defaultMaterialInstance);
				m_loadedMeshes.emplace(it->first, filamesh::MeshReader::loadMeshFromFile(m_engine, path, reg));
				filamesh::MeshReader::Mesh* mesh = &m_loadedMeshes[it->first];

				auto ti = m_transformManager->getInstance(mesh->renderable);
				math::mat4f transform = filament::math::mat4f{ filament::math::mat3f(1), filament::math::float3(0, 0, 0) };
				math::mat4f transform2 = filament::math::mat4f{ filament::math::mat3f(1), filament::math::float3(-1, -1.1f, 0) };
				m_transformManager->setTransform(ti, transform);
				m_transformManager->setTransform(ti2, transform2);
				m_renderableManager->setCastShadows(m_renderableManager->getInstance(mesh->renderable), true);

				m_gameScene->addEntity(mesh->renderable);
				it = m_meshesToLoadEditor.erase(it);
			}
		}
	}

	void RenderEngineFilament::AddMaterialData()
	{
		if (m_materialBuffer.size() == 0) return;

		auto it = m_materialBuffer.begin();
		while (it != m_materialBuffer.end())
		{
			Material* mat = m_loadedMaterials[it->first] = Material::Builder().package(it->second.data(), it->second.size()).build(*m_engine);
			it = m_materialBuffer.erase(it);

			std::string s = "Resources/Meshes/monkey.filamesh";
			StringIDType sid = StringID(s.c_str()).value();

			auto mins = mat->createInstance();
			mins->setParameter("baseColor", RgbType::LINEAR, filament::math::float3{ 0, 0.0, 0.5 });
			mins->setParameter("metallic", 1.0f);
			mins->setParameter("roughness", 1.0f);
			mins->setParameter("reflectance", 1.0f);
			m_renderableManager->setMaterialInstanceAt(m_renderableManager->getInstance(m_loadedMeshes[sid].renderable), 0, mins);
			it = m_materialBuffer.erase(it);

		}

	}
}