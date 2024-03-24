#pragma once
#include <Windows.h>
#include <functional>

class Main
{
  public:
	HMODULE handleModule = nullptr;
	bool unload = false;
  public:
	void __stdcall CleanUpDllMain();
	int __stdcall UnloadDllMain();
	std::function<void()> __stdcall Init();
};

extern Main *dllmain;