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
Class: EntityDrawer

Responsible for drawing the selected entity properties, name, layers etc. as well as initiating
component draw for that entity.

Timestamp: 10/12/2020 1:02:29 AM
*/

#pragma once

#ifndef EntityDrawer_HPP
#define EntityDrawer_HPP

#include "ECS/ECS.hpp"
#include "ComponentDrawer.hpp"

namespace LinaEditor
{
	class EntityDrawer
	{
	public:

		EntityDrawer() {};
		~EntityDrawer() {};
		
		void Setup();
		void SetSelectedEntity(LinaEngine::ECS::ECSEntity entity);
		void DrawSelectedEntity();

	private:

		ComponentDrawer m_componentDrawer;
		LinaEngine::ECS::ECSEntity m_selectedEntity;
		bool m_shouldCopyEntityName = true;

	};
}

#endif
