#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Author: Inan Evin
# www.inanevin.com
# 
# Copyright (C) 2018 Inan Evin
# 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
# and limitations under the License.
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.6)
project(LinaGraphics)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/vendor/filament)
	if(NOT EXISTS ${CMAKE_SOURCE_DIR}/vendor/filament.zip)
		message("Downloading filament dependencies...")
		file(DOWNLOAD https://github.com/inanevin/LinaEngine/releases/download/v1.0/filament.zip ${CMAKE_SOURCE_DIR}/vendor/filament.zip SHOW_PROGRESS
		TLS_VERIFY ON)
	endif()
endif()

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/vendor/filament)
	message("Extracting filament dependencies...")
	file(ARCHIVE_EXTRACT INPUT ${CMAKE_SOURCE_DIR}/vendor/filament.zip DESTINATION ${CMAKE_SOURCE_DIR}/vendor/)
	file(REMOVE INPUT ${CMAKE_SOURCE_DIR}/vendor/filament.zip)
endif()

#--------------------------------------------------------------------
# Set sources
#--------------------------------------------------------------------
set (LINAGRAPHICS_SOURCES

	#Rendering	
	src/Core/Backend/Vulkan/RenderEngineVulkan.cpp
	src/Core/Backend/Vulkan/VulkanLoader.cpp
	src/Core/Backend/Vulkan/WindowVulkan.cpp
	src/Core/Backend/Vulkan/VulkanHelper.cpp
	
	src/Core/Backend/Vulkan/Objects/VulkanSwapchain.cpp	
	src/Core/Backend/Vulkan/Objects/VulkanLogicalDevice.cpp	
	
	src/Core/Backend/Vulkan/Utility/VulkanFunctions.cpp

	src/Core/Backend/Filament/WindowFilament.cpp
	src/Core/Backend/Filament/RenderEngineFilament.cpp

	include/Core/Backend/Filament/MaterialResources/materialresources.c
	
	src/Core/Backend/Filament/filagui/ImGuiHelper.cpp
	src/Core/Backend/Filament/filagui/ImGuiExtensions.cpp
	src/Core/Backend/Filament/filagui/filagui.c
	
	# ImGui
	src/Core/Backend/Filament/imgui/imgui.cpp
	src/Core/Backend/Filament/imgui/imgui_demo.cpp
	src/Core/Backend/Filament/imgui/imgui_draw.cpp
	src/Core/Backend/Filament/imgui/imgui_widgets.cpp
)

#--------------------------------------------------------------------
# Set headers
#--------------------------------------------------------------------
set(LINAGRAPHICS_HEADERS

	include/Core/Backend/Vulkan/Utility/VulkanFunctions.hpp
	include/Core/Backend/Vulkan/Utility/VulkanFunctionList.inl
	
	include/Core/RenderingBackend.hpp
	include/Core/WindowBackend.hpp
	include/Core/RenderingCommon.hpp
	
	include/Core/Backend/Vulkan/WindowVulkan.hpp
	include/Core/Backend/Vulkan/VulkanHelper.hpp
	include/Core/Backend/Vulkan/RenderEngineVulkan.hpp
	include/Core/Backend/Vulkan/VulkanLoader.hpp
	include/Core/Backend/Vulkan/VulkanCommon.hpp

	include/Core/Backend/Vulkan/Objects/VulkanSwapchain.hpp
	include/Core/Backend/Vulkan/Objects/VulkanLogicalDevice.hpp
	
	include/Core/Backend/Filament/WindowFilament.hpp
	include/Core/Backend/Filament/RenderEngineFilament.hpp
	include/Core/Backend/Filament/MaterialResources/materialresources.h

	include/Core/Backend/Filament/filagui/ImGuiExtensions.h
	include/Core/Backend/Filament/filagui/ImGuiHelper.h
	include/Core/Backend/Filament/filagui/ImGuiMath.h
	include/Core/Backend/Filament/filagui/filagui.h
			
	# ImGui
	include/Core/Backend/Filament/imgui/dirent.h
	include/Core/Backend/Filament/imgui/imconfig.h
	include/Core/Backend/Filament/imgui/imgui.h
	include/Core/Backend/Filament/imgui/imgui_internal.h
	include/Core/Backend/Filament/imgui/imstb_rectpack.h
	include/Core/Backend/Filament/imgui/imstb_textedit.h
	include/Core/Backend/Filament/imgui/imstb_truetype.h
	
	
	include/linagraphics_export.h
)	


#--------------------------------------------------------------------
# Define the library & create an alias
#--------------------------------------------------------------------
add_library(${PROJECT_NAME} SHARED ${LINAGRAPHICS_SOURCES} ${LINAGRAPHICS_HEADERS})
add_library(Lina::Graphics ALIAS ${PROJECT_NAME}) 
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})
#--------------------------------------------------------------------
# Set include directories
#--------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/vulkan/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/LinaCommon/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/LinaResource/include)


#--------------------------------------------------------------------
# Subdirectories & linking
#--------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} 
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glfw/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/glfw3dll.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/filament.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/filameshio.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/backend.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/bluegl.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/bluevk.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/filabridge.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/filaflat.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/utils.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/geometry.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/meshoptimizer.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/smol-v.lib
	PUBLIC ${CMAKE_SOURCE_DIR}/vendor/filament/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/ibl.lib
	PUBLIC gdi32.lib
	PUBLIC shlwapi.lib
	PUBLIC user32.lib
	PUBLIC opengl32.lib
	PRIVATE Lina::Common
	PRIVATE Lina::Resource
)
	

#--------------------------------------------------------------------
# Folder structuring in visual studio
#--------------------------------------------------------------------
if(MSVC_IDE)
	foreach(source IN LISTS LINAGRAPHICS_HEADERS LINAGRAPHICS_SOURCES)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${LinaGraphics_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
				source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endif()


