/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: TestInteraction
Timestamp: 5/1/2019 4:42:14 PM

*/

#include "LinaPch.hpp"
#include "ECS/Interactions//TestInteraction.hpp"  

namespace LinaEngine::ECS
{
	void TestInteraction::Interact(float delta, BaseECSComponent ** interactorComponents, BaseECSComponent ** interacteeComponents)
	{
		Transformation& transform = ((TransformComponent*)interactorComponents[0])->transform;
		MotionComponent* motionComponent = (MotionComponent*)interactorComponents[2];

		//transform.SetRotation(transform.GetRotation() * Quaternion(Vector3F(0.0f, 0.0f, 1.0f), delta).Normalized());
		motionComponent->velocity *= -1.1f;
	}
}

