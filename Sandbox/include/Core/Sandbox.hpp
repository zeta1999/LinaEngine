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
Class: Sandbox



Timestamp: 12/24/2020 4:27:17 PM
*/

#pragma once

#ifndef Sandbox_HPP
#define Sandbox_HPP

#include <Core/Application.hpp>

int main(int argc, char** argv)
{
	Lina::Application app;
	Lina::ApplicationInfo appInfo;
	appInfo.m_windowProperties.m_width = 800;
	appInfo.m_windowProperties.m_height = 600;
	appInfo.m_windowProperties.m_decorated = false;
	appInfo.m_appMode = Lina::ApplicationMode::Editor;
	app.Startup(appInfo);
	app.Run(appInfo);
	return 0;
}

class Sandbox
{

public:

	Sandbox() {};
	~Sandbox() {};

private:

};


#endif
