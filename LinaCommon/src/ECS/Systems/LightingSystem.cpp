/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: LightingSystem
Timestamp: 5/13/2019 12:49:58 AM

*/

#include "LinaPch.hpp"
#include "ECS/Systems/LightingSystem.hpp"  
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/LightComponent.hpp"

namespace LinaEngine::ECS
{
	LightingSystem::LightingSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
	}

	void LightingSystem::UpdateComponents(float delta, BaseECSComponent ** components)
	{

	}

	
}

