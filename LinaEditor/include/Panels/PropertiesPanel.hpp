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
Class: PropertiesPanel

The central system for drawing properties of any objects & files including materials, textures,
meshes, entities etc.

Timestamp: 6/7/2020 5:13:24 PM
*/

#pragma once

#ifndef PropertiesPanel_HPP
#define PropertiesPanel_HPP

#include "Panels/EditorPanel.hpp"
#include "ECS/ECS.hpp"
#include "Drawers/TextureDrawer.hpp"
#include "Drawers/EntityDrawer.hpp"
#include "Drawers/MaterialDrawer.hpp"
#include "Drawers/MeshDrawer.hpp"
#include "Rendering/RenderingCommon.hpp"

namespace LinaEngine
{
	namespace Graphics
	{
		class RenderEngine;
		class Mesh;
		class Material;
	}
}


namespace LinaEditor
{
	class EditorFile;

	class PropertiesPanel : public EditorPanel
	{

		enum class DrawType
		{
			None,
			Entities,
			Texture2D,
			Mesh,
			Material
		};

	public:

		PropertiesPanel() {};
		virtual ~PropertiesPanel() {};

		virtual void Setup() override;
		virtual void Draw() override;

		void EntitySelected(LinaEngine::ECS::ECSEntity selectedEntity);
		void TextureSelected(LinaEngine::Graphics::Texture* texture);
		void MeshSelected(LinaEngine::Graphics::Mesh* mesh);
		void MaterialSelected(std::pair<EditorFile*, LinaEngine::Graphics::Material*>);

		void Unselect()
		{
			m_currentDrawType = DrawType::None;
		}


	private:

		// Selected texture
		TextureDrawer m_textureDrawer;

		// Selected mesh
		MeshDrawer m_meshDrawer;

		// Selected material
		MaterialDrawer m_materialDrawer;

		// Selected entity.
		EntityDrawer m_entityDrawer;

		DrawType m_currentDrawType = DrawType::None;

	};
}

#endif
