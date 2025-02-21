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
Class: LightingSystem

Responsible for processing lights and light information to be used by rendering engine
to update the shaders that receive lighting.

Timestamp: 5/13/2019 12:49:19 AM
*/

#pragma once

#ifndef LightingSystem_HPP
#define LightingSystem_HPP

#include "ECS/ECS.hpp"
#include "Utility/Math/Color.hpp"
#include "Utility/Math/Vector.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/LightComponent.hpp"
#include "PackageManager/PAMRenderDevice.hpp"


namespace LinaEngine
{
	namespace Graphics
	{
		class RenderEngine;
	}	
}

namespace LinaEngine::ECS
{
	class LightingSystem : public BaseECSSystem
	{
	public:

		LightingSystem() {};

		void Construct(ECSRegistry& registry, RenderDevice& rdIn, Graphics::RenderEngine& renderEngineIn)
		{
			BaseECSSystem::Construct(registry);
			s_renderDevice = &rdIn;
			m_renderEngine = &renderEngineIn;
		}

		DirectionalLightComponent* GetDirLight() { return std::get<1>(m_directionalLight); }
		virtual void UpdateComponents(float delta) override;
		void SetLightingShaderData(uint32 shaderID);
		void ResetLightData();
		Matrix GetDirectionalLightMatrix();
		Matrix GetDirLightBiasMatrix();
		std::vector<Matrix> GetPointLightMatrices();
		Color& GetAmbientColor() { return m_ambientColor; }
		const Vector3& GetDirectionalLightPos();

	private:

		RenderDevice* s_renderDevice = nullptr;
		Graphics::RenderEngine* m_renderEngine = nullptr;
		std::tuple < TransformComponent*, DirectionalLightComponent*> m_directionalLight;
		std::vector<std::tuple<TransformComponent*, PointLightComponent*>> m_pointLights;
		std::vector<std::tuple<TransformComponent*, SpotLightComponent*>> m_spotLights;
		Color m_ambientColor = Color(0.0f, 0.0f, 0.0f);
	};
}


#endif