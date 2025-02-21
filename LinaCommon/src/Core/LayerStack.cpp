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


#include "Core/LayerStack.hpp"  
#include "Core/Layer.hpp"

namespace LinaEngine
{
	LayerStack::LayerStack()
	{
		
	}

	LayerStack::~LayerStack()
	{
		
	}

	void LayerStack::PushLayer(Layer& layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, &layer);
		m_layerInsertIndex++;
		layer.Attach();
	}

	void LayerStack::PushOverlay(Layer& overlay)
	{
		m_layers.emplace_back(&overlay);
		overlay.Attach();
	}

	void LayerStack::PopLayer(Layer& layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), &layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			layer.Detach();
			m_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer& overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), &overlay);
		if (it != m_layers.end())
		{
			overlay.Detach();
			m_layers.erase(it);
		}
	}
}

