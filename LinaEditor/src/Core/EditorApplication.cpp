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

#include "Core/EditorApplication.hpp"
#include "Profiling/Profiler.hpp"
#include "Core/Log.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Core/Environment.hpp"
#include "Core/InputMappings.hpp"
#include "Core/WindowBackend.hpp"
#include "Core/RenderingBackend.hpp"

#include "Core/Backend/Filament/imgui/imgui.h"
#include "Core/Backend/Filament/imgui/imgui_internal.h"

#include <IconsFontAwesome5.h>
#include <IconsForkAwesome.h>
#include <IconsMaterialDesign.h>
#include <Core/Backend/Filament/filagui/ImGuiHelper.h>

namespace Lina::Editor
{
	std::function<void(filament::Engine*, filament::View*)> cb;
	static bool s = true;
	void sa(filament::Engine*, filament::View*)
	{
		//ImGui::ShowDemoWindow(&s);

	};

	EditorApplication::~EditorApplication()
	{
		if (m_guiHelper != nullptr)
			delete m_guiHelper;

		m_guiHelper = nullptr;
		g_env.g_eventSystem->Disconnect<Event::ETick>(this);
	}

	void EditorApplication::Startup()
	{
		// Setup Dear ImGui context
		cb = std::bind(sa, std::placeholders::_1, std::placeholders::_2);

		g_env.g_eventSystem->Connect<Event::ETick, &EditorApplication::Render>(this);

		m_guiHelper = new filagui::ImGuiHelper(g_env.g_render->GetEngine(), g_env.g_render->GetUIView(), "Resources/Editor/Fonts/Mukta-Medium.ttf");
		m_guiHelper->setDisplaySize(g_env.g_appInfo->m_windowProperties.m_width, g_env.g_appInfo->m_windowProperties.m_height, 1.0f, 1.0f);

		ImGuiIO& io = ImGui::GetIO();

		io.ImeWindowHandle = Lina::g_env.g_render->GetWindow().GetNativeWindow();

		io.KeyMap[ImGuiKey_Tab] = LINA_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = LINA_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = LINA_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = LINA_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = LINA_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = LINA_KEY_PAGEUP;
		io.KeyMap[ImGuiKey_PageDown] = LINA_KEY_PAGEDOWN;
		io.KeyMap[ImGuiKey_Home] = LINA_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = LINA_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = LINA_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = LINA_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = LINA_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = LINA_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = LINA_KEY_RETURN;
		io.KeyMap[ImGuiKey_Escape] = LINA_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = LINA_KEY_A;
		io.KeyMap[ImGuiKey_C] = LINA_KEY_C;
		io.KeyMap[ImGuiKey_V] = LINA_KEY_V;
		io.KeyMap[ImGuiKey_X] = LINA_KEY_X;
		io.KeyMap[ImGuiKey_Y] = LINA_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = LINA_KEY_Z;
		io.SetClipboardTextFn = [](void*, const char* text) {
			g_env.g_render->GetWindow().SetClipboardString(text);
		};
		io.GetClipboardTextFn = [](void*) -> const char* {
			return g_env.g_render->GetWindow().GetClipboardString();
		};

		io.ClipboardUserData = nullptr;


		return;
		ImGui::CreateContext();

		// Add default font.
		io.Fonts->AddFontFromFileTTF("Resources/Editor/Fonts/Mukta-Medium.ttf", 20.0f, NULL);

		// merge in icons from Font Awesome
		static const ImWchar icons_rangesFA[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		static const ImWchar icons_rangesFK[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		static const ImWchar icons_rangesMD[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;

		io.Fonts->AddFontFromFileTTF("Resources/Editor/Fonts/FontAwesome/fa-solid-900.ttf", 20.0f, &icons_config, icons_rangesFA);
		io.Fonts->AddFontFromFileTTF("Resources/Editor/Fonts/ForkAwesome/forkawesome-webfont.ttf", 30.0f, &icons_config, icons_rangesFK);
		io.Fonts->AddFontFromFileTTF("Resources/Editor/Fonts/MaterialIcons/MaterialIcons-Regular.ttf", 30.0f, &icons_config, icons_rangesMD);

		// Setup configuration flags.
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();

		GLFWwindow* window = static_cast<GLFWwindow*>(Lina::g_env.g_render->GetWindow().GetWindowPointer());


		// Setup Dear ImGui style
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;
		style.AntiAliasedFill = false;
		style.WindowRounding = 0.0f;
		style.TabRounding = 3.0f;
		style.ChildRounding = 0.0f;
		style.PopupRounding = 3.0f;
		style.FrameRounding = 3.0f;
		style.ScrollbarRounding = 5.0f;
		style.FramePadding = ImVec2(0, 5);
		style.WindowPadding = ImVec2(0, 0);
		style.ItemInnerSpacing = ImVec2(8, 4);
		style.GrabRounding = 6.0f;
		style.ChildBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMenuButtonPosition = ImGuiDir_None;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.17f, 0.18f, 0.54f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.45f, 0.28f, 0.46f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.34f, 0.46f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.28f, 0.46f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.45f, 0.28f, 0.46f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.46f, 0.34f, 0.47f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.39f, 0.47f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.25f, 0.26f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
		//colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.28f, 0.46f, 1.00f);
		//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.69f, 0.15f, 0.29f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 0.28f, 0.46f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.61f);
		// Imgui first frame initialization.
		Render();
	}

	void EditorApplication::Render()
	{
		m_guiHelper->render(0.001f, cb);

	}
}