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
Class: Shader

Wrapper for shader functionalities, responsible for creating shaders, binding uniform buffers
and all related shader operations.

Timestamp: 2/16/2019 1:47:28 AM
*/

#pragma once

#ifndef Shader_HPP
#define Shader_HPP

#include "Core/Common.hpp"
#include "PackageManager/PAMRenderDevice.hpp"
#include "UniformBuffer.hpp"
#include <string>

namespace LinaEngine::Graphics
{

	class Shader
	{
	public:

		Shader() {};

		~Shader() { m_engineBoundID = s_renderDevice->ReleaseShaderProgram(m_engineBoundID); }

		Shader& Construct(RenderDevice& renderDeviceIn, const std::string& text, bool usesGeometryShader)
		{
			s_renderDevice = &renderDeviceIn;
			m_engineBoundID = s_renderDevice->CreateShaderProgram(text, &m_uniformData, usesGeometryShader);
			return *this;
		}

		// Set uniform buffer through render engine.
		void SetUniformBuffer(const std::string& name, UniformBuffer& buffer) 
		{ 
			s_renderDevice->SetShaderUniformBuffer(m_engineBoundID, name, buffer.GetID()); 
		}

		void BindBlockToBuffer(uint32 bindingPoint, std::string blockName)
		{
			s_renderDevice->BindShaderBlockToBufferPoint(m_engineBoundID, bindingPoint, blockName);
		}

		// Get shader id, this gets matched w/ program id on render engine.
		uint32 GetID() { return m_engineBoundID; }

		ShaderUniformData& GetUniformData() { return m_uniformData; }
		const std::string& GetPath() { return m_path; }

		static Shader& CreateShader(const std::string& path, bool usesGeometryShader = false);
		static Shader& GetShader(const std::string& path);
		static Shader& GetShader(int id);
		static bool ShaderExists(const std::string& path);
		static bool ShaderExists(int id);
		static void UnloadAll();
		static std::map<int, Shader*>& GetLoadedShaders() { return s_loadedShaders; }

	private:

		ShaderUniformData m_uniformData;
		RenderDevice* s_renderDevice = nullptr;
		uint32 m_engineBoundID = 0;
		std::string m_path = "";
		static std::map<int, Shader*> s_loadedShaders;

	
	};
}


#endif