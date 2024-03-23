#include "../include/hack.h"
#include "../include/menu.h"
#include "../lib/imgui_hook.h"
#include "../lib/kiero/kiero.h"

#include <Windows.h>
#include <functional>

auto __stdcall UnloadDllMain(void) -> decltype(int())
{
	::kiero::shutdown();
	return TRUE;
}

auto __stdcall Init(void) -> decltype(::std::function<void()>())
{
	return [&] {
		if (::menu != nullptr)
		{
			::menu->Render();
		}
		else
		{
			::printf("menu pointer is: nullptr");
		}

		if (::hack != nullptr)
		{
			::Hack::PlayerData playerData;
			::hack->StorePlayerData(playerData);
			::hack->Loop(playerData);
		}
		else
		{
			::printf("hack pointer is: nullptr");
		}
	};
}

auto __stdcall DllMain(::HMODULE hMod, ::DWORD dwReason, ::LPVOID lpReserved)
	-> decltype(int())
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		::DisableThreadLibraryCalls(hMod);
		::ImGuiHook::Load(::Init());
		break;
	case DLL_PROCESS_DETACH:
		::UnloadDllMain();
		break;
	}
	return TRUE;
}
