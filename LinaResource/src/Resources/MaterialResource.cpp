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

#include "Resources/MaterialResource.hpp"
#include "Utility/FileUtility.hpp"
#include "Core/Log.hpp"
#include "EventSystem/EventSystem.hpp"
#include "EventSystem/Events.hpp"
#include <fstream>

namespace Lina::Resources
{

    bool MaterialResource::LoadFromFile(const std::string& path, Event::EventSystem* eventSys)
    {
#ifdef LINA_GRAPHICS_FILAMENT
       
        std::ifstream file(path, std::ios::binary);
        // Stop eating new lines in binary mode!!!
        file.unsetf(std::ios::skipws);

        // get its size:
        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        Event::EMaterialResourceLoaded e = Event::EMaterialResourceLoaded();
        e.m_sid = StringID(path.c_str()).value();

        // reserve capacity
        e.m_data.reserve(fileSize);

        // read the data:
        e.m_data.insert(e.m_data.begin(),
            std::istream_iterator<unsigned char>(file),
            std::istream_iterator<unsigned char>());

        // Trigger event w/ data 
        eventSys->Trigger<Event::EMaterialResourceLoaded>(e);
        LINA_TRACE("[Material Loader] -> Material loaded from file: {0}", path);

        return true;

#else
        // LoadFromFile the level file if exists.
        if (FileUtility::FileExists(path))
        {
            {
                std::ifstream stream(path, std::ios::binary);
                {
                    cereal::PortableBinaryInputArchive iarchive(stream);
                    iarchive(*this);
                }
            }
        }
        else
        {
            LINA_ERR("[Material Loader] -> Material does not exist: {0}", path);
            return false;
        }

        LINA_TRACE("[Material Loader] -> Material loaded from file: {0}", path);
        return true;
#endif
    }

    bool MaterialResource::LoadFromMemory(StringIDType m_sid, unsigned char* buffer, size_t bufferSize, Event::EventSystem* eventSys)
    {
#ifdef LINA_GRAPHICS_FILAMENT
        Event::EMaterialResourceLoaded e = Event::EMaterialResourceLoaded();
        e.m_sid = m_sid;
        e.m_data = std::vector(buffer, buffer + bufferSize);
        eventSys->Trigger<Event::EMaterialResourceLoaded>(e);
        LINA_TRACE("[Material Loader] -> Material loaded from memory");
        return true;
#else
        {
            std::string data((char*)buffer, bufferSize);
            std::istringstream stream(data);
            {
                cereal::PortableBinaryInputArchive iarchive(stream);
                iarchive(*this);
            }           
        }

        LINA_TRACE("[Material Loader] -> Material loaded from memory.");
        return true;
#endif
    }

}
