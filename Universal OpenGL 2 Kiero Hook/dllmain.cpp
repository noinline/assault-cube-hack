#include "include/imgui/imgui.h"
#include "include/imgui_hook.h"
#include "include/kiero/kiero.h"

#include <Windows.h>
#include <string>

struct offsets_t
{
	uintptr_t base =
		reinterpret_cast<uintptr_t>(::GetModuleHandleA("ac_client.exe"));
	uintptr_t local_player = 0x18AC00;
	uintptr_t entity_list = 0x18AC04;
	uintptr_t player_count = 0x18AC0C;
} offset;

struct functions_t
{
	bool show_menu = false;
	bool godmode = false;
	bool funny_camera = false;
	bool teleport = false;
	bool noclip = false;
} func;

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
	const int local_player_health_modified = 9999;

	int camera_address = local_player + 0x324;
	int camera = *reinterpret_cast<int *>(camera_address);
	const int camera_modified = 1852730990;

	int debug_mode_free_camera_address = local_player + 0x74;
	int debug_mode = *reinterpret_cast<int *>(debug_mode_free_camera_address);
	const int debug_mode_modified = 262144;

	if (func.godmode)
	{
		*reinterpret_cast<int *>(local_player_health_address) =
			local_player_health_modified;
	}
	else if (!func.godmode && local_player_health == 9999)
	{
		*reinterpret_cast<int *>(local_player_health_address) = 100;
	}

	if (func.funny_camera)
	{
		*reinterpret_cast<int *>(camera_address) = camera_modified;
	}
	else if (!func.funny_camera && camera == 1852730990)
	{
		*reinterpret_cast<int *>(camera_address) = 0;
	}

	if (func.noclip)
	{
		if (::GetAsyncKeyState(0x06) & 1)
		{
			*reinterpret_cast<int *>(debug_mode_free_camera_address) =
				debug_mode_modified;
		}

		if (::GetAsyncKeyState(0x05) & 1 && debug_mode == 262144)
		{
			*reinterpret_cast<int *>(debug_mode_free_camera_address) = 0;
		}
	}

	int local_player_team_id_address = local_player + 0x30C;

	for (int i = 0; i < player_count; ++i)
	{
		int player = *reinterpret_cast<int *>((ent_list + 0x4 * i));

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

		if (func.teleport && player_team_id != local_player_team_id)
		{
			if (::GetAsyncKeyState(0x54))
			{
				*reinterpret_cast<float *>((player + 0x28)) = final_pos_x;
				*reinterpret_cast<float *>((player + 0x2C)) = local_pos_y;
				*reinterpret_cast<float *>((player + 0x30)) = local_pos_z;
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
	const int local_player_health_modified = 9999;

	int camera_address = local_player + 0x324;
	int camera = *reinterpret_cast<int *>(camera_address);
	const int camera_modified = 1852730990;

	int debug_mode_free_camera_address = local_player + 0x74;
	int debug_mode = *reinterpret_cast<int *>(debug_mode_free_camera_address);
	const int debug_mode_modified = 262144;

	*reinterpret_cast<int *>(local_player_health_address) = 100;
	*reinterpret_cast<int *>(camera_address) = 0;
	*reinterpret_cast<int *>(debug_mode_free_camera_address) = 0;
	kiero::shutdown();
	return S_OK;
}

auto
RenderMain(void) -> void
{
	ImGui::SetNextWindowPos(
		{ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2},
		ImGuiCond_Once, {0.5, 0.5});
	ImGui::SetNextWindowSize({200.f, 100.f}, NULL);

	if (::GetAsyncKeyState(VK_DELETE) & 1)
		func.show_menu = !func.show_menu;

	if (func.show_menu)
	{
		ImGui::Begin("Stinky Balls");
		ImGui::Checkbox("Toggle godmode", &func.godmode);
		ImGui::Checkbox("Toggle teleport", &func.teleport);
		ImGui::Checkbox("Funny camera", &func.funny_camera);
		ImGui::Checkbox("Debug camera", &func.noclip);
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