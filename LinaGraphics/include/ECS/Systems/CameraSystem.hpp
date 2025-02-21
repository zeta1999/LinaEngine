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
Class: CameraSystem

Responsible for finding cameras in the level and updating the view, projection and other
matrices depending on the most active camera.

Timestamp: 5/2/2019 12:40:46 AM
*/

#pragma once

#ifndef CameraSystem_HPP
#define CameraSystem_HPP

#include "ECS/ECS.hpp"
#include "Utility/Math/Matrix.hpp"
#include "Utility/Math/Color.hpp"
#include "ECS/Components/LightComponent.hpp"

namespace LinaEngine::ECS
{
	struct TransformComponent;
	struct CameraComponent;


	class CameraSystem : public BaseECSSystem
	{
	public:

		CameraSystem() {};
		virtual void UpdateComponents(float delta) override;

		void Construct(ECSRegistry& registry);
		void SetAspectRatio(float aspect) { m_aspectRatio = aspect; }
		void InjectViewMatrix(const Matrix& view) { m_viewMatrixInjected = true;  m_view = view; }
		void InjectProjMatrix(const Matrix& proj) { m_projMatrixInjected = true; m_projection = proj; }
		void SetActiveCamera(ECSEntity cameraOwner);
		
		void OnCameraDestroyed(entt::registry& registry, entt::entity entity)
		{
			if (entity == m_activeCameraEntity)
				m_activeCameraEntity = entt::null;
		}

		ECSEntity GetActiveCamera() { return m_activeCameraEntity; }
		Matrix& GetViewMatrix() { return m_view; }
		Matrix& GetProjectionMatrix() { return m_projection; }
		Vector3 GetCameraLocation();
		Color& GetCurrentClearColor();
		CameraComponent* GetActiveCameraComponent();


	private:

		Matrix m_view = Matrix::Identity();
		Matrix m_projection = Matrix::Perspective(35, 1.33f, 0.01f, 1000.0f);
		float m_aspectRatio = 1.33f;
		bool m_useDirLightView = false;
		bool m_viewMatrixInjected = false;
		bool m_projMatrixInjected = false;
		ECSEntity m_activeCameraEntity = entt::null;

	};
}


#endif