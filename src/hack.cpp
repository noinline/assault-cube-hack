#include "../include/hack.h"

#include "../include/dllmain.h"
#include "../include/menu.h"
::Flags *flag = new ::Flags();
::Hack *hack = new ::Hack();

auto ::Hack::StorePlayerData(PlayerData &playerData) -> decltype(void())
{
	playerData.localPlayer =
		*reinterpret_cast<int *>(offset.base + offset.localPlayer);
	playerData.localPlayerName =
		reinterpret_cast<char *>(playerData.localPlayer + 0x205);
	playerData.localPlayerFlags =
		*reinterpret_cast<int *>(playerData.localPlayer + 0x5D);
	playerData.localPlayerTeam =
		*reinterpret_cast<int *>(playerData.localPlayer + 0x30C);
	playerData.localPosX =
		*reinterpret_cast<float *>(playerData.localPlayer + 0x28);
	playerData.localPosY =
		*reinterpret_cast<float *>(playerData.localPlayer + 0x2C);
	playerData.localPosZ =
		*reinterpret_cast<float *>(playerData.localPlayer + 0x30);
	playerData.finalPosX = playerData.localPosX + 5.0f;
	playerData.entList =
		*reinterpret_cast<int *>(offset.base + offset.entityList);
	playerData.playerCount =
		*reinterpret_cast<int *>(offset.base + offset.playerCount);
	playerData.localPlayerHealthAddress = playerData.localPlayer + 0xEC;
	playerData.localPlayerHealth =
		*reinterpret_cast<int *>(playerData.localPlayerHealthAddress);
	playerData.cameraAddress = playerData.localPlayer + 0x324;
	playerData.camera = *reinterpret_cast<int *>(playerData.cameraAddress);
	playerData.debugModeFreeCameraAddress = playerData.localPlayer + 0x74;
	playerData.debugMode =
		*reinterpret_cast<int *>(playerData.debugModeFreeCameraAddress);
}

auto ::Hack::Loop(PlayerData &playerData) -> decltype(void())
{
	if (::hack->func.godMode)
	{
		*reinterpret_cast<int *>(playerData.localPlayerHealthAddress) =
			playerData.localPlayerHealthModified;
	}
	else if (!::hack->func.godMode &&
			 playerData.localPlayerHealth == HealthModified)
	{
		*reinterpret_cast<int *>(playerData.localPlayerHealthAddress) = 100;
	}

	if (::hack->func.funnyCamera)
	{
		*reinterpret_cast<int *>(playerData.cameraAddress) =
			playerData.cameraModified;
	}
	else if (!::hack->func.funnyCamera && playerData.camera == CameraModified)
	{
		*reinterpret_cast<int *>(playerData.cameraAddress) = NULL;
	}

	if (::hack->func.noclip)
	{
		if (::GetAsyncKeyState(VK_XBUTTON2) & 1)
		{
			*reinterpret_cast<int *>(playerData.debugModeFreeCameraAddress) =
				playerData.debugModeModified;
		}

		if (::GetAsyncKeyState(VK_XBUTTON1) & 1 &&
			playerData.debugMode == DebugModeModified)
		{
			*reinterpret_cast<int *>(playerData.debugModeFreeCameraAddress) =
				NULL;
		}
	}

	for (int i = 0; i < playerData.playerCount; ++i)
	{
		int playerList = *reinterpret_cast<int *>(playerData.entList + 0x4 * i);

		if (!playerList)
			continue;

		if (::hack->func.line)
		{
			::render.LineToPlayer(
				{*reinterpret_cast<float *>(playerList + 0x28),
				 *reinterpret_cast<float *>(playerList + 0x2C),
				 *reinterpret_cast<float *>(playerList + 0x30)},
				::menu->col.line, 2.f);
		}

		if (::hack->func.teleport)
		{
			if (::GetAsyncKeyState(0x54))
			{
				*reinterpret_cast<float *>(playerList + 0x28) =
					playerData.finalPosX;
				*reinterpret_cast<float *>(playerList + 0x2C) =
					playerData.localPosY;
				*reinterpret_cast<float *>(playerList + 0x30) =
					playerData.localPosZ;
			}
		}
	}
}