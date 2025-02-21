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

#include "FPSDemo/FPSDemoLevel.hpp"
#include "Core/EditorCommon.hpp"
#include "Rendering/RenderEngine.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/FreeLookComponent.hpp"
#include "Core/Application.hpp"
#include "ECS/Components/MeshRendererComponent.hpp"
#include "ECS/Components/SpriteRendererComponent.hpp"
#include "ECS/Components/RigidbodyComponent.hpp"
#include "FPSDemo/HeadbobComponent.hpp"
#include "FPSDemo/PlayerMotionComponent.hpp"
#include "Drawers/ComponentDrawer.hpp"
#include "ECS/Systems/CameraSystem.hpp"
#include "Utility/Math/Matrix.hpp"
#include "Helpers/DrawParameterHelper.hpp"
#include "glad/glad.h"
#include "glm/gtc/matrix_access.hpp"

namespace LinaEngine
{
	using namespace LinaEngine::ECS;
	using namespace LinaEngine::Graphics;

	bool FPSDemoLevel::Install(bool loadFromFile, const std::string& path, const std::string& levelName)
	{
		Level::Install(loadFromFile, path, levelName);
		return true;
	}


	void FPSDemoLevel::Initialize()
	{
		m_registry = &Application::GetECSRegistry();

		Application::GetApp().GetPlayModeStack().PushLayer(m_player);

		if (!m_processOnce)
		{
			m_processOnce = true;
			RenderEngine& renderEngine = Application::GetRenderEngine();
			RenderDevice& rd = renderEngine.GetRenderDevice();
			Vector2 viewportSize = renderEngine.GetViewportSize();

			// Setup portal rt
			m_portalRTParams.m_textureParams.m_pixelFormat = PixelFormat::FORMAT_RGB;
			m_portalRTParams.m_textureParams.m_internalPixelFormat = PixelFormat::FORMAT_RGBA16F;
			m_portalRTParams.m_textureParams.m_minFilter = m_portalRTParams.m_textureParams.m_magFilter = SamplerFilter::FILTER_LINEAR;
			m_portalRTParams.m_textureParams.m_wrapS = m_portalRTParams.m_textureParams.m_wrapT = SamplerWrapMode::WRAP_REPEAT;
			m_portalTexture.ConstructRTTexture(rd, viewportSize, m_portalRTParams, false);
			m_portalBuffer.Construct(rd, RenderBufferStorage::STORAGE_DEPTH, viewportSize);
			m_portalRT.Construct(rd, m_portalTexture, viewportSize, TextureBindMode::BINDTEXTURE_TEXTURE2D, FrameBufferAttachment::ATTACHMENT_COLOR, FrameBufferAttachment::ATTACHMENT_DEPTH, m_portalBuffer.GetID());

		}
		

		// Component drawer.
		m_componentDrawer.AddComponentDrawFunctions();

	}

	void FPSDemoLevel::Tick(bool isInPlayMode, float delta)
	{
		m_isInPlayMode = isInPlayMode;
	}

	void FPSDemoLevel::Uninstall()
	{
		LinaEngine::Application::GetEngineDispatcher().UnsubscribeAction("fpsdemo_preDraw", LinaEngine::Action::ActionType::PreDraw);
		LinaEngine::Application::GetApp().GetPlayModeStack().PopLayer(m_player);
	}

}