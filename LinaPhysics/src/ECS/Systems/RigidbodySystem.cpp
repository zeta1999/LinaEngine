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

#include "ECS/Systems/RigidbodySystem.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/RigidbodyComponent.hpp"
#include "Physics/PhysicsEngine.hpp"

namespace LinaEngine::ECS
{
	void RigidbodySystem::UpdateComponents(float delta)
	{
		auto view = m_ecs->view<TransformComponent, RigidbodyComponent>();

		// Find all entities with rigidbody component and transform component attached to them.
		for (auto entity : view)
		{
			RigidbodyComponent& rbComponent = view.get<RigidbodyComponent>(entity);
			if (!rbComponent.m_isEnabled) continue;

			TransformComponent& transform = view.get<TransformComponent>(entity);

			// We get the rigidbody information from the world, and update the entity's transformation
			// based on the body's transformation. So we keep the game world that does the rendering via
			// transformations in sync with the physics world.
			btRigidBody* rb = m_physicsEngine->GetActiveRigidbody(rbComponent.m_bodyID);
			btTransform btTrans;
			rb->getMotionState()->getWorldTransform(btTrans);

			transform.transform.SetLocation(Vector3(btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ()));
			transform.transform.SetRotation(Quaternion(btTrans.getRotation().getX(), btTrans.getRotation().getY(), btTrans.getRotation().getZ(), btTrans.getRotation().getW()));
		}
	}
}


