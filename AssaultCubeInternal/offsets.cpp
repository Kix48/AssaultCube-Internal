#include <Windows.h>
#include "offsets.h"

//Sets some global addresses that wil be used
uintptr_t Offsets::base = NULL;
Entity* Offsets::localPlayer = NULL;
EntList* Offsets::entList = NULL;
uintptr_t Offsets::crosshair = 0x607c0;
uintptr_t Offsets::traceLine = 0x48a310;
uintptr_t Offsets::entityListAddr = 0x50f4f8;
tGetCrosshairEnt Offsets::GetCrosshairEnt = nullptr;
int* Offsets::numOfPlayers = (int*)(0x50f500);
glMatrix* Offsets::vMatix = (glMatrix*)0x501ae8;
int* Offsets::gameMode = (int*)0x50f49c;
HMODULE Offsets::showCursorMod = NULL;
uintptr_t Offsets::showCursorAddr = NULL;

/*
These next blocks of code are really ugly :(
I used these to set some of my hacks that 
involve weapon data back to normal.
You might be suprised to hear that this took
almost 2 hours to make because I kept screwing up.
Tbh I dont want to delete all of this because Im lazy
and I think you can relate in some way or another.
Omg I just ranted I'm sorry
*/

//AR
	int Weapon::AR::weaponId = 6;
	short Weapon::AR::sound = 18;
	short Weapon::AR::reload = 19;
	short Weapon::AR::reloadTime = 2000;
	short Weapon::AR::wait = 120;
	short Weapon::AR::damage = 18;
	short Weapon::AR::piercing = 0;
	short Weapon::AR::projSpeed = 0;
	short Weapon::AR::part = 0;
	short Weapon::AR::spread = 18;
	short Weapon::AR::recoil = 30;
	short Weapon::AR::magSize = 20;
	short Weapon::AR::kickRotation = 0;
	short Weapon::AR::kickBack = 2;
	short Weapon::AR::recoilIncrease = 3;
	short Weapon::AR::recoilBase = 25;
	short Weapon::AR::maxRecoil = 50;
	short Weapon::AR::recoilBackFade = 115;
	short Weapon::AR::pushFactor = 1;
	bool Weapon::AR::bAuto = true;

	//Pistol

	int Weapon::Pistol::weaponId = 1;
	short Weapon::Pistol::sound = 8;
	short Weapon::Pistol::reload = 9;
	short Weapon::Pistol::reloadTime = 1400;
	short Weapon::Pistol::wait = 160;
	short Weapon::Pistol::damage = 18;
	short Weapon::Pistol::piercing = 0;
	short Weapon::Pistol::projSpeed = 0;
	short Weapon::Pistol::part = 0;
	short Weapon::Pistol::spread = 53;
	short Weapon::Pistol::recoil = 10;
	short Weapon::Pistol::magSize = 10;
	short Weapon::Pistol::kickRotation = 6;
	short Weapon::Pistol::kickBack = 5;
	short Weapon::Pistol::recoilIncrease = 6;
	short Weapon::Pistol::recoilBase = 35;
	short Weapon::Pistol::maxRecoil = 58;
	short Weapon::Pistol::recoilBackFade = 125;
	short Weapon::Pistol::pushFactor = 1;
	bool Weapon::Pistol::bAuto = false;

	//Grenade

	int Weapon::Grenade::weaponId = 8;
	short Weapon::Grenade::sound = 102;
	short Weapon::Grenade::reload = 102;
	short Weapon::Grenade::reloadTime = 1000;
	short Weapon::Grenade::wait = 650;
	short Weapon::Grenade::damage = 200;
	short Weapon::Grenade::piercing = 0;
	short Weapon::Grenade::projSpeed = 20;
	short Weapon::Grenade::part = 6;
	short Weapon::Grenade::spread = 1;
	short Weapon::Grenade::recoil = 1;
	short Weapon::Grenade::magSize = 1;
	short Weapon::Grenade::kickRotation = 3;
	short Weapon::Grenade::kickBack = 1;
	short Weapon::Grenade::recoilIncrease = 0;
	short Weapon::Grenade::recoilBase = 0;
	short Weapon::Grenade::maxRecoil = 0;
	short Weapon::Grenade::recoilBackFade = 0;
	short Weapon::Grenade::pushFactor = 3;
	bool Weapon::Grenade::bAuto = false;

	//SMG

	int Weapon::SMG::weaponId = 4 & 7;
	short Weapon::SMG::sound = 14;
	short Weapon::SMG::reload = 15;
	short Weapon::SMG::reloadTime = 1650;
	short Weapon::SMG::wait = 80;
	short Weapon::SMG::damage = 16;
	short Weapon::SMG::piercing = 0;
	short Weapon::SMG::projSpeed = 0;
	short Weapon::SMG::part = 0;
	short Weapon::SMG::spread = 45;
	short Weapon::SMG::recoil = 15;
	short Weapon::SMG::magSize = 30;
	short Weapon::SMG::kickRotation = 1;
	short Weapon::SMG::kickBack = 2;
	short Weapon::SMG::recoilIncrease = 5;
	short Weapon::SMG::recoilBase = 25;
	short Weapon::SMG::maxRecoil = 50;
	short Weapon::SMG::recoilBackFade = 18;
	short Weapon::SMG::pushFactor = 1;
	bool Weapon::SMG::bAuto = true;

	//Sniper

	int Weapon::Sniper::weaponId = 5;
	short Weapon::Sniper::sound = 16;
	short Weapon::Sniper::reload = 17;
	short Weapon::Sniper::reloadTime = 1500;
	short Weapon::Sniper::wait = 1950;
	short Weapon::Sniper::damage = 82;
	short Weapon::Sniper::piercing = 25;
	short Weapon::Sniper::projSpeed = 0;
	short Weapon::Sniper::part = 0;
	short Weapon::Sniper::spread = 50;
	short Weapon::Sniper::recoil = 50;
	short Weapon::Sniper::magSize = 5;
	short Weapon::Sniper::kickRotation = 4;
	short Weapon::Sniper::kickBack = 4;
	short Weapon::Sniper::recoilIncrease = 10;
	short Weapon::Sniper::recoilBase = 85;
	short Weapon::Sniper::maxRecoil = 85;
	short Weapon::Sniper::recoilBackFade = 100;
	short Weapon::Sniper::pushFactor = 1;
	bool Weapon::Sniper::bAuto = false;

	//Shotgun

	int Weapon::Shotgun::weaponId = 3;
	short Weapon::Shotgun::sound = 12;
	short Weapon::Shotgun::reload = 13;
	short Weapon::Shotgun::reloadTime = 2400;
	short Weapon::Shotgun::wait = 880;
	short Weapon::Shotgun::damage = 1;
	short Weapon::Shotgun::piercing = 0;
	short Weapon::Shotgun::projSpeed = 0;
	short Weapon::Shotgun::part = 0;
	short Weapon::Shotgun::spread = 1;
	short Weapon::Shotgun::recoil = 35;
	short Weapon::Shotgun::magSize = 7;
	short Weapon::Shotgun::kickRotation = 9;
	short Weapon::Shotgun::kickBack = 9;
	short Weapon::Shotgun::recoilIncrease = 10;
	short Weapon::Shotgun::recoilBase = 140;
	short Weapon::Shotgun::maxRecoil = 140;
	short Weapon::Shotgun::recoilBackFade = 125;
	short Weapon::Shotgun::pushFactor = 1;
	bool Weapon::Shotgun::bAuto = false;

	//Carbine

	int Weapon::Carbine::weaponId = 2;
	short Weapon::Carbine::sound = 10;
	short Weapon::Carbine::reload = 11;
	short Weapon::Carbine::reloadTime = 1800;
	short Weapon::Carbine::wait = 720;
	short Weapon::Carbine::damage = 60;
	short Weapon::Carbine::piercing = 40;
	short Weapon::Carbine::projSpeed = 0;
	short Weapon::Carbine::part = 0;
	short Weapon::Carbine::spread = 10;
	short Weapon::Carbine::recoil = 60;
	short Weapon::Carbine::magSize = 10;
	short Weapon::Carbine::kickRotation = 4;
	short Weapon::Carbine::kickBack = 4;
	short Weapon::Carbine::recoilIncrease = 10;
	short Weapon::Carbine::recoilBase = 60;
	short Weapon::Carbine::maxRecoil = 60;
	short Weapon::Carbine::recoilBackFade = 150;
	short Weapon::Carbine::pushFactor = 1;
	bool Weapon::Carbine::bAuto = false;

	//Akimbo

	int Weapon::Akimbo::weaponId = 9;
	short Weapon::Akimbo::sound = 8;
	short Weapon::Akimbo::reload = 45;
	short Weapon::Akimbo::reloadTime = 1400;
	short Weapon::Akimbo::wait = 80;
	short Weapon::Akimbo::damage = 19;
	short Weapon::Akimbo::piercing = 0;
	short Weapon::Akimbo::projSpeed = 0;
	short Weapon::Akimbo::part = 0;
	short Weapon::Akimbo::spread = 50;
	short Weapon::Akimbo::recoil = 10;
	short Weapon::Akimbo::magSize = 20;
	short Weapon::Akimbo::kickRotation = 6;
	short Weapon::Akimbo::kickBack = 5;
	short Weapon::Akimbo::recoilIncrease = 4;
	short Weapon::Akimbo::recoilBase = 15;
	short Weapon::Akimbo::maxRecoil = 25;
	short Weapon::Akimbo::recoilBackFade = 115;
	short Weapon::Akimbo::pushFactor = 1;
	bool Weapon::Akimbo::bAuto = true;