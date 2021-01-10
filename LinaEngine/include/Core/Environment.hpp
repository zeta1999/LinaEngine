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
Class: Environment



Timestamp: 1/10/2021 2:49:34 PM
*/

#pragma once

#ifndef Environment_HPP
#define Environment_HPP

// Headers here.
#include "EventSystem/Events.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Core/InputBackend.hpp"
#include "Core/AudioBackend.hpp"
#include "Core/RenderingBackend.hpp"
#include "Core/WindowBackend.hpp"
#include "Core/PhysicsBackend.hpp"
#include "Core/ResourceManager.hpp"
#include "Engine.hpp"
#include <linaengine_export.h>

namespace Lina
{
	struct AppEnvironment
	{
		Engine* g_engine = nullptr;
		Event::EventSystem* g_eventSystem = nullptr;
		ECS::Registry* g_ecs = nullptr;
		Graphics::RenderEngineBackend* g_render = nullptr;
		Resources::ResourceManager* g_resources = nullptr;
		Input::InputEngineBackend* g_input = nullptr;
		Physics::PhysicsEngineBackend* g_physics = nullptr;
		Audio::AudioEngineBackend* g_audio = nullptr;
	};
	
	extern LINAENGINE_EXPORT AppEnvironment g_env;
}

#endif
