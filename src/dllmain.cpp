#include "../include/dllmain.h"

#include "../include/hack.h"
#include "../include/menu.h"
#include "../lib/imgui_hook.h"
#include "../lib/kiero/kiero.h"

::Main *dllmain = new ::Main();

auto __stdcall ::Main::CleanUpDllMain(void) -> decltype(void())
{
	if (::flag != nullptr)
	{
		::delete ::flag;
		::flag = nullptr;
	}

	if (::hack != nullptr)
	{
		::hack->offset.base = NULL;
		::delete ::hack;
		::hack = nullptr;
	}
	if (::menu != nullptr)
	{
		::delete ::menu;
		::menu = nullptr;
	}
}

auto __stdcall ::Main::UnloadDllMain(void) -> decltype(int())
{
	::dllmain->CleanUpDllMain();
	::kiero::shutdown();
	return TRUE;
}

auto __stdcall ::Main::Init(void) -> decltype(::std::function<void()>())
{
	if (::dllmain != nullptr)
	return [&] {
		if (::menu != nullptr)
		{
			::menu->ShowMenu();
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

	return NULL;
}

auto __stdcall DllMain(::HMODULE hMod, ::DWORD dwReason, ::LPVOID lpReserved)
	-> decltype(int())
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		::dllmain->handleModule = hMod;
		::DisableThreadLibraryCalls(hMod);
		::ImGuiHook::Load(::dllmain->Init());
		break;
	case DLL_PROCESS_DETACH:
		if (::dllmain != nullptr)
		{
			::delete ::dllmain;
			::dllmain = nullptr;
		}
		break;
	}
	return TRUE;
}
