#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_opengl2.h"
#include <iostream>
#include "Menu.h"
#include "offsets.h"
#include "config.h"
#include "hook.h"

//Options used in menu combos
static const char* aimKeys[]{ "RMB", "LMB", "XMB1", "XMB2", "LAlt", "LCtrl" };
static const char* aimModes[]{ "Crosshair", "Distance" };
static const char* teleportKeys[]{ "XMB1", "XMB2", "LCtrl", "T" };
static int selectedMode = 0;

//Default menu size
ImVec2 windowSize(800, 800);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WNDPROC oriWndProc = NULL;

typedef int(__cdecl* tSDL_ShowCursor)(int);
tSDL_ShowCursor SDL_ShowCursor = (tSDL_ShowCursor)Offsets::showCursorAddr;


//WndProc hook
LRESULT __stdcall hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Config::bMenu)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}

		ImGuiIO& io = ImGui::GetIO();

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[0];
			return 0;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[1];
			return 0;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = !io.MouseDown[2];
			return 0;
		case WM_MOUSEWHEEL:
			return 0;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		}
	}

	return CallWindowProc(oriWndProc, hWnd, uMsg, wParam, lParam);
}

//Creates menu context
void Menu::Create()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io  = ImGui::GetIO();
}

//Initializes the menu and does style
void Menu::Initialize() 
{
	HWND hwnd = NULL;
	while (hwnd == NULL)
	{
		hwnd = FindWindowA(NULL, "AssaultCube");
	}
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();
	
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.26f, 0.94f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.26f, 0.54f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.26f, 0.98f, 0.40f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.26f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.40f, 0.26f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.40f, 0.26f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.26f, 1.00f, 0.63f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.26f, 1.00f, 0.51f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.26f, 1.00f, 0.50f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.26f, 1.00f, 0.50f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.26f, 1.00f, 0.56f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.22f, 0.26f, 0.54f);	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.26f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	ImGui::SetNextWindowSize(windowSize, 0);
	oriWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)hWndProc);
}
 
//Renders actual menu(Look at the imgui source code to tell what is happening)
void Menu::Render()
{
	//Needs to reset every render to check for changes
	if (selectedMode == 0)
	{
		Config::bDistance = false;
		Config::bCrosshair = true;
	}
	else
	{
		Config::bCrosshair = false;
		Config::bDistance = true;
	}
	if (Config::bMenu)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		SDL_ShowCursor(1);

		ImGui::Begin("Kix's AC Internal", &Config::bMenu, ImGuiWindowFlags_None);
			
			ImGui::BeginTabBar("Lolggez", ImGuiTabBarFlags_None);
			
			if (ImGui::BeginTabItem("Visuals"))
			{
				ImGui::Checkbox("Enable Esp", &Config::bEsp);
				ImGui::Separator();
				if (ImGui::TreeNode("Toggles##2"))
				{
					ImGui::Checkbox("Team Esp", &Config::bTeamEsp);
					ImGui::Checkbox("Check if Visible##2", &Config::bVisibleEsp);
					ImGui::Checkbox("Name", &Config::bName);
					ImGui::Checkbox("Healthbar", &Config::bHealthbar);
					ImGui::Checkbox("Snaplines", &Config::bSnaplines);
					ImGui::TreePop();
				}
				ImGui::Separator();
				if (ImGui::TreeNode("Colors"))
				{
					ImGui::Text("Enemy Box Color:");
					ImGui::SameLine();
					ImGui::ColorEdit3("Enemy Box Color", (float*)&rgb::enemyBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					/////////////////////////////////////////
					ImGui::Text("Enemy Visible Color:");
					ImGui::SameLine();
					ImGui::ColorEdit3("Enemy Visible Color", (float*)&rgb::enemyBoxVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					/////////////////////////////////////////
					ImGui::Text("Team Box Color:");
					ImGui::SameLine();
					ImGui::ColorEdit3("Team Box Color", (float*)&rgb::teamBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					/////////////////////////////////////////
					ImGui::Text("Name Color:");
					ImGui::SameLine();
					ImGui::ColorEdit3("Name Color", (float*)&rgb::text, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					/////////////////////////////////////////
					ImGui::Text("Snapline Color:");
					ImGui::SameLine();
					ImGui::ColorEdit3("Snapline Color", (float*)&rgb::snapline, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					/////////////////////////////////////////
					ImGui::TreePop();
				}
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::Checkbox("Enable Aimbot", &Config::bAimbot);
				ImGui::Separator();
				if (ImGui::TreeNode("Toggles##1"))
				{
					ImGui::Text("Aim Key");
					ImGui::Combo("##AimKey", &Config::selectedAimKey, aimKeys, IM_ARRAYSIZE(aimKeys));
					ImGui::Text("Mode");
					ImGui::Combo("##AimMode", &selectedMode, aimModes, IM_ARRAYSIZE(aimModes));
					ImGui::Checkbox("Check if Visible##1", &Config::bVisibleAim);
					ImGui::Checkbox("Smooth", &Config::bSmooth);
					ImGui::SliderFloat("##Smooth Value", &Config::smooth, 1, 10);
					ImGui::TreePop();
				}

				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::Checkbox("Health/Armor", &Config::bHealth);
				ImGui::SameLine();
				IconHelpMarker("Only works in singleplayer!");
				if (ImGui::TreeNode("Health Values"))
				{
					ImGui::Text("Health Amount");
					ImGui::SliderInt("##Health Amount", &Config::health, 0, 9999);
					ImGui::Text("Armor Amount");
					ImGui::SliderInt("##Armor Amount", &Config::armor, 0, 9999);
					ImGui::TreePop();
				}
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Ammo", &Config::bAmmo);
				ImGui::SameLine();
				IconHelpMarker("Only works in singleplayer!");
				if (ImGui::TreeNode("Ammo Values"))
				{
					ImGui::Text("Ammo Amount");
					ImGui::SliderInt("##Ammo Amount", &Config::ammo, 1, 9999);
					ImGui::Text("Mag Amount");
					ImGui::SliderInt("##Mag Amount", &Config::mag, 1, 9999);
					ImGui::TreePop();
				}
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Damage", &Config::bDamage);
				ImGui::SameLine();
				IconHelpMarker("Only works in singleplayer! Bots also recieve hack!");
				if (ImGui::TreeNode("Damage Value"))
				{
					ImGui::Text("Weapon Damage");
					ImGui::SliderInt("##Weapon Damage", &Config::damage, 0, 9999);
					ImGui::TreePop();
				}
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Recoil/Spread", &Config::bRecoilSpread);
				ImGui::SameLine();
				IconHelpMarker("Needs to be fixed!");
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Rapid Fire", &Config::bAuto);
				ImGui::SameLine();
				IconHelpMarker("Only works in singleplayer! Recommended to have Recoil/Spread on");
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Triggerbot", &Config::bTriggerbot);
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Paperwalls", &Config::bPaperwalls);
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Teleport", &Config::bTeleport);
				if (ImGui::TreeNode("Teleport Key"))
				{
					ImGui::Combo("##Teleport Key", &Config::selectedTeleportKey, teleportKeys, IM_ARRAYSIZE(teleportKeys));
					ImGui::TreePop();
				}

				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Always Headshot", &Config::bHeadshot);
				ImGui::Separator();
				/////////////////////////////////////////
				ImGui::Checkbox("Map", &Config::bMap);
				/////////////////////////////////////////

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}
	else
	{
		SDL_ShowCursor(0);
	}
}

//Unloads menu and unhooks WndProc
void Menu::Shutdown()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtr(FindWindowA(NULL, "AssaultCube"), GWLP_WNDPROC, (LONG_PTR)oriWndProc);
	SDL_ShowCursor(0);
}
