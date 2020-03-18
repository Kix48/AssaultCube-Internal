#pragma once
#include <Windows.h>
#include <stdint.h>
#include "Helpers.h"

#ifndef OFFSETS_H
#define OFFSETS_H

//Reversed game classes using ReClass
class Entity;

typedef Entity* (__cdecl* tGetCrosshairEnt)();

struct glMatrix
{
	float matrix[16]; //0x003C
};

struct Vector2
{
	float x; //0x0000
	float y; //0004
};

struct Vector3
{
	float x; //0x0000
	float y; //0x0004
	float z; //0x0008
};

struct Vector4
{
	float x; //0x0000
	float y; //0x0004
	float z; //0x0008
	float w; //0x000C
};

struct TraceResult
{
	Vector3 end;
	bool collided;
};

class PlayerPtr
{
public:
	class Entity *EntityPtr; //0x0000
	char pad_0004[56]; //0x0004
}; //Size: 0x003C

class Entity
{
public:
	uintptr_t vTable; //0x0000
	Vector3 HeadPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 Pos; //0x0034
	Vector3 ViewAngles; //0x0040
	char pad_004C[34]; //0x004C
	bool bCrouching; //0x006E
	char pad_006F[137]; //0x006F
	int32_t Health; //0x00F8
	int32_t Armor; //0x00FC
	char pad_0100[292]; //0x0100
	bool bAttacking; //0x0224
	char Name[16]; //0x0225
	char pad_0235[247]; //0x0235
	uint8_t Team; //0x032C
	char pad_032D[11]; //0x032D
	uint8_t State; //0x0338
	char pad_0339[59]; //0x0339
	class CurrentWeapon* CurrentWeaponPtr; //0x0374
	char pad_0378[282]; //0x0378
}; //Size: 0x0492

class CurrentWeapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t WeaponId; //0x0004
	class Entity *WeaponOwnerPtr; //0x0008
	class WeaponInfo *WeaponInfoPtr; //0x000C
	int16_t *Mag; //0x0010
	int16_t *Ammo; //0x0014
	int16_t *Wait; //0x0018
	int16_t Shots; //0x001C
	char pad_001E[2]; //0x001E
	int16_t Reloading; //0x0020
	char pad_0022[1038]; //0x0022
}; //Size: 0x0430

class WeaponInfo
{
public:
	char pad_0000[260]; //0x0000
	int16_t Sound; //0x0104
	int16_t Reload; //0x0106
	int16_t ReloadTime; //0x0108
	int16_t Wait; //0x010A
	int16_t Damage; //0x010C
	int16_t Piercing; //0x010E
	int16_t ProjSpeed; //0x0110
	int16_t Part; //0x0112
	int16_t Spread; //0x0114
	int16_t Recoil; //0x0116
	int16_t MagSize; //0x0118
	int16_t KickRotation; //0x011A
	int16_t KickBack; //0x011C
	int16_t RecoilIncrease; //0x011E
	int16_t RecoilBase; //0x0120
	int16_t MaxRecoil; //0x0122
	int16_t RecoilBackFade; //0x0124
	int16_t PushFactor; //0x0126
	bool bAuto; //0x0128
	char pad_0129[807]; //0x0129
}; //Size: 0x0450

class EntList
{
public:
	Entity* ents[32];
};

class Weapon
{
public:
	struct AR
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct Pistol
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct Grenade
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct SMG
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct Sniper
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct Shotgun
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct Carbine
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};

	struct Akimbo
	{
		static int weaponId;
		static short sound;
		static short reload;
		static short reloadTime;
		static short wait;
		static short damage;
		static short piercing;
		static short projSpeed;
		static short part;
		static short spread;
		static short recoil;
		static short magSize;
		static short kickRotation;
		static short kickBack;
		static short recoilIncrease;
		static short recoilBase;
		static short maxRecoil;
		static short recoilBackFade;
		static short pushFactor;
		static bool bAuto;
	};
};

//Offsets used
namespace Offsets
{
	extern uintptr_t base;
	extern Entity* localPlayer;
	extern EntList* entList;
	extern uintptr_t crosshair;
	extern uintptr_t traceLine;
	extern uintptr_t entityListAddr;
	extern tGetCrosshairEnt GetCrosshairEnt;
	extern int* numOfPlayers;
	extern glMatrix* vMatix;
	extern int* gameMode;
	extern HMODULE showCursorMod;
	extern uintptr_t showCursorAddr;
}

#endif