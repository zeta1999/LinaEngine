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

#include "Core/Backend/Filament/FilaImage.hpp"
#include "Core/Log.hpp"
#include "Core/ResourcesCommon.hpp"
#include "Utility/FileUtility.hpp"
#include <stb/stb_image.h>
#include <fstream>
#include <cereal/archives/portable_binary.hpp>

using namespace filament;
namespace Lina::Graphics
{
	FilaImage::FilaImage(filament::Engine* engine, Event::EImageResourceLoaded& e)
	{
		m_engine = engine;
		m_resEvent = e;
	}

	void FilaImage::Construct()
	{
		Texture::PixelBufferDescriptor buffer(m_resEvent.m_data, size_t((size_t)m_resEvent.m_width * (size_t)m_resEvent.m_height * (size_t)4), Texture::Format::RGBA, Texture::Type::UBYTE, (Texture::PixelBufferDescriptor::Callback)&stbi_image_free);
		m_tex = Texture::Builder().width(uint32_t(m_resEvent.m_width)).height(uint32_t(m_resEvent.m_height)).levels(m_mipLevel).sampler(Texture::Sampler::SAMPLER_2D).format(m_internalFormat).build(*m_engine);
		m_sampler = new TextureSampler(m_minFilter, m_magFilter, m_wrapS, m_wrapT, m_wrapR);
		m_isConstructed = true;
		LINA_TRACE("[Fila Image] -> Constructed.");
	}

	void FilaImage::LoadMetadata(const std::string& path)
	{
		{
			std::ifstream stream(path, std::ios::binary);
			{
				cereal::PortableBinaryInputArchive iarchive(stream);
				iarchive(*this);
			}
		}
	}

	void FilaImage::LoadMetadata(unsigned char* buffer, size_t bufferSize)
	{
		std::string data((char*)buffer, bufferSize);
		std::istringstream stream(data);
		{
			cereal::PortableBinaryInputArchive iarchive(stream);
			iarchive(*this);
		}
	}

	void FilaImage::ExportMetadata()
	{
		{
			std::string path = FileUtility::RemoveExtensionFromFileName(m_resourcePath) + std::string(LINAIMAGE_EXT);
			std::ofstream stream(path, std::ios::binary);
			{
				cereal::PortableBinaryOutputArchive oarchive(stream);
				oarchive(*this);
			}
		}
	}

}