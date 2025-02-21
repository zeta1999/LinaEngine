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
Class: Window

Abstract representation of a window, declares all window related functionality to be
implemented in platform dependent window classes.

Timestamp: 4/14/2019 7:46:12 PM
*/

#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <functional>
#include "Utility/Math/Vector.hpp"
#include "RenderingCommon.hpp"

namespace LinaEngine::Graphics
{
	class Window
	{
	public:

		Window() {};

		virtual ~Window() {};

		virtual bool CreateContext(WindowProperties propsIn) = 0;
		virtual void Tick() = 0;
		virtual void* GetNativeWindow() const = 0;

		virtual void SetVsync(bool enabled)
		{
			m_windowProperties.vSyncEnabled = enabled;
		}

		virtual double GetTime() = 0;
		virtual void SetSize(const Vector2& newSize) = 0;
		virtual void SetPos(const Vector2& newPos) = 0;
		virtual void SetPosCentered(const Vector2 newPos) = 0;
		virtual void Iconify() = 0;
		virtual void Maximize() = 0;
		virtual void Close() = 0;

		bool GetVsycnEnabled() { return m_windowProperties.vSyncEnabled; }

		uint32 GetWidth() { return m_windowProperties.m_width; }

		uint32 GetHeight() { return m_windowProperties.m_height; }

		Vector2 GetSize() { return Vector2(m_windowProperties.m_width, m_windowProperties.m_height); }

		Vector2 GetPos() { return Vector2(m_windowProperties.m_xPos, m_windowProperties.m_yPos); }

		const WindowProperties& GetWindowProperties() { return m_windowProperties; }

		// Set event callbacks.
		void SetKeyCallback(std::function<void(int, int)>& callback) { m_keyCallback = callback; }
		void SetMouseCallback(std::function<void(int, int)>& callback) { m_mouseCallback = callback; }
		void SetWindowResizeCallback(std::function<void(Vector2)>& callback) { m_windowResizeCallback = callback; }
		void SetWindowClosedCallback(std::function<void()>& callback) { m_windowCloseCallback = callback; }
	
	protected:

		std::function<void(int, int)> m_keyCallback;
		std::function<void(int, int)> m_mouseCallback;
		std::function<void(Vector2)> m_windowResizeCallback;
		std::function<void()> m_windowCloseCallback;
		WindowProperties m_windowProperties;
	};
}

#endif