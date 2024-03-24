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

	auto IsEnemy(int local_player_flags) -> bool
	{
		if (local_player_flags == Team::EnemyTeam)
		{
			return true;
		}
		return false;
	}
};

extern ::Flags *flag;

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
		int localPlayer = 0;
		char *localPlayerName = nullptr;
		int localPlayerFlags = 0;
		int localPlayerTeam = 0;
		float localPosX = 0.0f;
		float localPosY = 0.0f;
		float localPosZ = 0.0f;
		float finalPosX = 0.0f;
		int entList = 0;
		int playerCount = 0;
		int playerTeam = 0;
		int localPlayerHealthAddress = 0;
		int localPlayerHealth = 0;
		const int localPlayerHealthModified = HealthModified;
		int cameraAddress = 0;
		int camera = 0;
		const int cameraModified = CameraModified;
		int debugModeFreeCameraAddress = 0;
		int debugMode = 0;
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

extern ::Hack *hack;