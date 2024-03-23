#pragma once
#include "../include/render.h"
#include "../include/vector.h"

#include <Windows.h>
#include <string>

#define CameraModified	  1852730990
#define DebugModeModified 262144
#define HealthModified	  9999

class Flags
{
	enum State
	{
		OnGround = 1,
		InJump = 0
	};

	enum Team
	{
	LocalTeam = 0,
	EnemyTeam = 1
	};

  public:
	auto IsInJump(int local_player_flags) -> bool
	{
		if (local_player_flags == State::InJump)
		{
			return true;
		}
		return false;
	}

	auto IsOnGround(int local_player_flags) -> bool
	{
		if (local_player_flags == State::OnGround)
		{
			return true;
		}
		return false;
	}
};

class Hack
{
  public:
	struct Offsets
	{
		uintptr_t base =
			reinterpret_cast<uintptr_t>(::GetModuleHandleA("ac_client.exe"));
		uintptr_t localPlayer = 0x18AC00;
		uintptr_t entityList = 0x18AC04;
		uintptr_t playerCount = 0x18AC0C;
		uintptr_t viewMatrix = 0x17DFD0;
	} offset;

	struct PlayerData
	{
		int localPlayer;
		char *localPlayerName;
		int localPlayerFlags;
		int localPlayerTeam;
		float localPosX;
		float localPosY;
		float localPosZ;
		float finalPosX;
		int entList;
		int playerCount;
		int playerTeam;
		int localPlayerHealthAddress;
		int localPlayerHealth;
		const int localPlayerHealthModified = HealthModified;
		int cameraAddress;
		int camera;
		const int cameraModified = CameraModified;
		int debugModeFreeCameraAddress;
		int debugMode;
		const int debugModeModified = DebugModeModified;

		PlayerData()
			: localPlayerHealthModified(HealthModified),
			  cameraModified(CameraModified),
			  debugModeModified(DebugModeModified)
		{
		}
	};

	struct Functions
	{
		bool showMenu = false;
		bool godMode = false;
		bool funnyCamera = false;
		bool teleport = false;
		bool noclip = false;
		bool bhop = false;
		bool line = false;
	} func;

  public:
	void StorePlayerData(PlayerData &playerData);
	void Loop(PlayerData &playerData);
};

extern Hack *hack;