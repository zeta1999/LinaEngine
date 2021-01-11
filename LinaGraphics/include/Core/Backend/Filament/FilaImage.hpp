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
Class: FilaImage

Serializable image wrapper for filament textures.

Timestamp: 1/11/2021 7:44:52 PM
*/

#pragma once

#ifndef FilaImage_HPP
#define FilaImage_HPP

// Headers here.
#include "EventSystem/Events.hpp"
#include <cereal/cereal.hpp>
#include "filament/Texture.h"
#include "filament/TextureSampler.h"

namespace filament
{
	class Engine;
}

namespace Lina::Graphics
{
	class FilaImage
	{
		
	public:
		
	private:

		friend class RenderEngineFilament;

		FilaImage(filament::Engine* engine, Event::EImageResourceLoaded& e);
		~FilaImage() {};

		void Construct();
		void LoadMetadata(const std::string& path);
		void LoadMetadata(unsigned char* buffer, size_t bufferSize);
		void ExportMetadata(const std::string& path);

	private:

		Event::EImageResourceLoaded m_resEvent;
		filament::Engine* m_engine = nullptr;
		filament::Texture* m_tex = nullptr;
		filament::TextureSampler* m_sampler = nullptr;
		filament::TextureSampler::MinFilter m_minFilter = filament::TextureSampler::MinFilter::NEAREST;
		filament::TextureSampler::MagFilter m_magFilter = filament::TextureSampler::MagFilter::NEAREST;
		filament::TextureSampler::WrapMode m_wrapS = filament::TextureSampler::WrapMode::CLAMP_TO_EDGE;
		filament::TextureSampler::WrapMode m_wrapT = filament::TextureSampler::WrapMode::CLAMP_TO_EDGE;
		filament::TextureSampler::WrapMode m_wrapR = filament::TextureSampler::WrapMode::CLAMP_TO_EDGE;
		filament::Texture::InternalFormat m_internalFormat = filament::Texture::InternalFormat::RGBA8;

		friend class cereal::access;

		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(m_minFilter, m_magFilter, m_wrapS, m_wrapT, m_wrapR);
		}
	};
}

#endif
