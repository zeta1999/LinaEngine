/*
This file is a part of: Lina Engine
https://github.com/inanevin/Lina

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
Class: EditorLayer

Editor app entry point.

Timestamp: 4/14/2019 7:46:20 PM
*/

#pragma once

#ifndef EDITORAPP_HPP
#define EDITORAPP_HPP

#include <Core/Application.hpp>

namespace filagui
{
	class ImGuiHelper;
}

namespace Lina::Editor
{
	class EditorApplication
	{

	public:

		EditorApplication() { }
		~EditorApplication();

		void Startup();
		void Render();

	private:

		filagui::ImGuiHelper* m_guiHelper;

	};
}


int main(int argc, char** argv)
{
	Lina::Application app;
	Lina::ApplicationInfo appInfo;
	Lina::Editor::EditorApplication editorApp;
	appInfo.m_windowProperties.m_width = 1440;
	appInfo.m_windowProperties.m_height = 900;
	appInfo.m_windowProperties.m_decorated = false;
	appInfo.m_appMode = Lina::ApplicationMode::Editor;
	app.Startup(appInfo);
	editorApp.Startup();
	app.Run();

	return 0;
}

#endif



