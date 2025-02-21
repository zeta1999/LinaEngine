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
Class: RenderBuffer

Used for managing and storing render buffer data from the GPU backend.


Timestamp: 5/19/2020 8:10:11 PM
*/

#pragma once

#ifndef RenderBuffer_HPP
#define RenderBuffer_HPP

#include "PackageManager/PAMRenderDevice.hpp"

namespace LinaEngine::Graphics
{
	class RenderBuffer
	{

	public:

		RenderBuffer() {};
		~RenderBuffer() { m_id = s_renderDevice->ReleaseRenderBufferObject(m_id); };
		
		void Construct(RenderDevice& renderDeviceIn, RenderBufferStorage storage, const Vector2& size, int sampleCount = 0)
		{
			m_id = s_renderDevice->CreateRenderBufferObject(storage,(uint32)size.x, (uint32)size.y, sampleCount);
		}

		uint32 GetID() { return m_id; }

	private:

		RenderDevice* s_renderDevice = nullptr;
		uint32 m_id = 0;
	};
}

#endif
