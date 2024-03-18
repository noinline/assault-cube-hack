#include "include/imgui/imgui.h"
#include "include/imgui_hook.h"
#include "include/kiero/kiero.h"
#include "vector.h"

#include <Windows.h>
#include <string>

#define CAMERA_MODIFIED		1852730990
#define DEBUG_MODE_MODIFIED 262144
#define HEALTH_MODIFIED		9999

struct offsets_t
{
	uintptr_t base =
		reinterpret_cast<uintptr_t>(::GetModuleHandleA("ac_client.exe"));
	uintptr_t local_player = 0x18AC00;
	uintptr_t entity_list = 0x18AC04;
	uintptr_t player_count = 0x18AC0C;
	uintptr_t view_matrix = 0x17DFD0;
} offset;

struct functions_t
{
	bool show_menu = false;
	bool godmode = false;
	bool funny_camera = false;
	bool teleport = false;
	bool noclip = false;
	bool line = false;
} func;

struct colors_t
{
	ImColor line = (255, 255, 255, 255);
}col;

static auto
WorldToScreen(Vector pos) -> Vector2D
{
	ImGui::Text("called WorldToScreen");
	Vector clip = {0.0f, 0.0f, 0.0f};
	Vector2D ndc = {0.0f, 0.0f};
	Vector2D result = {0.0f, 0.0f};
	Vector2D base = {0.0f, 0.0f};

	float matrix[16]{};
	for (int i = 0; i < 16; i++)
	{
		float elemenet_of_matrix =
			*(float *) (offset.base + offset.view_matrix + 4 * i);
		matrix[i] = elemenet_of_matrix;
	}

	clip.z =
		pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
	if (clip.z < 0.2)
		return base;
	clip.x =
		pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clip.y =
		pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];

	ndc.x = clip.x / clip.z;
	ndc.y = clip.y / clip.z;

	result.x = ((ImGui::GetIO().DisplaySize.x / 2 * ndc.x) +
				(ndc.x + ImGui::GetIO().DisplaySize.x / 2));
	result.y = (-(ImGui::GetIO().DisplaySize.y / 2 * ndc.y) +
				(ndc.y + ImGui::GetIO().DisplaySize.y / 2));
	ImGui::Text("Returned WorldToScreen");
	return result;
}

auto
RenderLineToPlayer(Vector pos, ImColor col, float thickness) -> void
{
	pos = {pos.x, pos.y, pos.z};

	Vector2D translate = WorldToScreen(pos);

	if (translate.x == NULL && translate.y == NULL)
		return;
	
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2
		(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y),
		ImVec2(translate.x, translate.y), col, thickness);

}

auto
Loop(void) -> void
{
	int local_player =
		*reinterpret_cast<int *>((offset.base + offset.local_player));
	char *local_player_name = reinterpret_cast<char *>((local_player + 0x205));
	float local_pos_x = *reinterpret_cast<float *>((local_player + 0x28));
	float local_pos_y = *reinterpret_cast<float *>((local_player + 0x2C));
	float local_pos_z = *reinterpret_cast<float *>((local_player + 0x30));
	float final_pos_x = local_pos_x + 5.0f;
	int ent_list = *reinterpret_cast<int *>((offset.base + offset.entity_list));
	int player_count =
		*reinterpret_cast<int *>((offset.base + offset.player_count));

	int local_player_health_address = local_player + 0xEC;
	int local_player_health =
		*reinterpret_cast<int *>(local_player_health_address);
	const int local_player_health_modified = HEALTH_MODIFIED;

	int camera_address = local_player + 0x324;
	int camera = *reinterpret_cast<int *>(camera_address);
	const int camera_modified = CAMERA_MODIFIED;

	int debug_mode_free_camera_address = local_player + 0x74;
	int debug_mode = *reinterpret_cast<int *>(debug_mode_free_camera_address);
	const int debug_mode_modified = DEBUG_MODE_MODIFIED;

	if (func.godmode)
	{
		*reinterpret_cast<int *>(local_player_health_address) =
			local_player_health_modified;
	}
	else if (!func.godmode && local_player_health == HEALTH_MODIFIED)
	{
		*reinterpret_cast<int *>(local_player_health_address) = 100;
	}

	if (func.funny_camera)
	{
		*reinterpret_cast<int *>(camera_address) = camera_modified;
	}
	else if (!func.funny_camera && camera == CAMERA_MODIFIED)
	{
		*reinterpret_cast<int *>(camera_address) = NULL;
	}

	if (func.noclip)
	{
		if (::GetAsyncKeyState(VK_XBUTTON2) & 1)
		{
			*reinterpret_cast<int *>(debug_mode_free_camera_address) =
				debug_mode_modified;
		}

		if (::GetAsyncKeyState(VK_XBUTTON1) & 1 &&
			debug_mode == DEBUG_MODE_MODIFIED)
		{
			*reinterpret_cast<int *>(debug_mode_free_camera_address) = NULL;
		}
	}

	int local_player_team_id_address = local_player + 0x30C;
	for (int i = 0; i < player_count; ++i)
	{
		int player = *reinterpret_cast<int *>((ent_list + 0x4 * i));

		float player_pos_x = *reinterpret_cast<float *>(player + 0x28);
		float player_pos_y = *reinterpret_cast<float *>(player + 0x2C);
		float player_pos_z = *reinterpret_cast<float *>(player + 0x30); 

		int player_team_id =
			*reinterpret_cast<int *>((ent_list + 0x30C + 0x4 * i));

		int local_player_team_id =
			*reinterpret_cast<int *>(local_player_team_id_address);

		if (!player)
			continue;

		if (!player_team_id)
			continue;

		if (!local_player_team_id)
			continue;

		if (func.line)
		RenderLineToPlayer({player_pos_x, player_pos_y, player_pos_z},
						   col.line, 2.f);

		if (func.teleport)
		{
			if (::GetAsyncKeyState(0x54))
			{
				player_pos_x = final_pos_x;
				player_pos_y = local_pos_y;
				player_pos_z = local_pos_z;
			}
		}
	}
}

auto
UnloadMain(void) -> bool
{
	int local_player =
		*reinterpret_cast<int *>((offset.base + offset.local_player));

	int local_player_health_address = local_player + 0xEC;
	int local_player_health =
		*reinterpret_cast<int *>(local_player_health_address);
	const int local_player_health_modified = HEALTH_MODIFIED;

	int camera_address = local_player + 0x324;
	int camera = *reinterpret_cast<int *>(camera_address);
	const int camera_modified = CAMERA_MODIFIED;

	int debug_mode_free_camera_address = local_player + 0x74;
	int debug_mode = *reinterpret_cast<int *>(debug_mode_free_camera_address);
	const int debug_mode_modified = DEBUG_MODE_MODIFIED;

	*reinterpret_cast<int *>(local_player_health_address) = 100;
	*reinterpret_cast<int *>(camera_address) = NULL;
	*reinterpret_cast<int *>(debug_mode_free_camera_address) = NULL;
	kiero::shutdown();
	return S_OK;
}

auto
RenderMain(void) -> void
{
	ImGui::SetNextWindowPos(
		{ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2},
		ImGuiCond_Once, {0.5, 0.5});
	ImGui::SetNextWindowSize({250.f, 150.f}, NULL);

	if (::GetAsyncKeyState(VK_DELETE) & 1)
		func.show_menu = !func.show_menu;

	if (func.show_menu)
	{
		ImGui::Begin("Stinky Balls");
		ImGui::Checkbox("Toggle godmode", &func.godmode);
		ImGui::Checkbox("Toggle teleport", &func.teleport);
		ImGui::Checkbox("Funny camera", &func.funny_camera);
		ImGui::Checkbox("Debug camera", &func.noclip);
		ImGui::Checkbox("Show line", &func.line);
		ImGui::SameLine();
		ImGui::ColorEdit4("##line_color", reinterpret_cast<float *>(&col.line),
						  ImGuiColorEditFlags_NoLabel);
		if (ImGui::Button("Unload", {100, 50}))
		{
			UnloadMain();
		}
		ImGui::End();
	}

	/* create loop */
	Loop();
}

BOOL __stdcall DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		::DisableThreadLibraryCalls(hMod);
		ImGuiHook::Load(RenderMain);
		break;
	case DLL_PROCESS_DETACH:
		UnloadMain();
		break;
	}
	return TRUE;
}