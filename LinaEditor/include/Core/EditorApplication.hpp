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
Class: EditorApplication

This class can also be derived from LinaEngine::Application to create a standalone editor build.
Probably will be deprecated in the future as the editor is now a linked-library from Sandbox.

Timestamp: 10/15/2020 10:44:39 PM
*/

#pragma once

#ifndef EditorApplication_HPP
#define EditorApplication_HPP

#include "Actions/ActionDispatcher.hpp"
#include "Utility/Log.hpp"
#include "ECS/Systems/EditorCameraSystem.hpp"
#include "Core/GUILayer.hpp"

namespace LinaEngine
{
	namespace World
	{
		class Level;
	}

}

namespace LinaEditor
{
	class EditorApplication
	{
	public:

		EditorApplication();
		~EditorApplication();

		void Setup();
		void Refresh();
		void LevelInstalled(LinaEngine::World::Level* level);
		static LinaEngine::Action::ActionDispatcher& GetEditorDispatcher() { return s_editorDispatcher; }

	private:

		void PlayModeChanged(bool enabled);

	private:

		LinaEngine::ECS::EditorCameraSystem editorCameraSystem;
		static LinaEngine::Action::ActionDispatcher s_editorDispatcher;
		GUILayer m_guiLayer;
	};
}

#endif
