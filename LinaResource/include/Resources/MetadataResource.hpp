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
Class: MetadataResource



Timestamp: 1/11/2021 7:59:29 PM
*/

#pragma once

#ifndef MetadataResource_HPP
#define MetadataResource_HPP

// Headers here.
#include "Utility/StringId.hpp"

namespace Lina
{
	namespace Event
	{
		class EventSystem;
	}
}

namespace Lina::Resources
{

	class MetadataResource
	{

	public:

	private:

		friend class ResourceBundle;
		friend class ResourceManager;

		MetadataResource() {};
		~MetadataResource() {};

		bool LoadFromFile(ResourceType type, const std::string& path, Event::EventSystem* eventSys);
		bool LoadFromMemory(ResourceType type, StringIDType m_sid, unsigned char* buffer, size_t bufferSize, Event::EventSystem* eventSys);

	};
}
#endif
