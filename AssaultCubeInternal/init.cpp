#include <Windows.h>
#include <iostream>
#include "init.h"
#include "hack.h"
#include "offsets.h"
#include "config.h"
#include "Menu.h"

Menu m;

//Everything here is done right when the hack starts
Init::Init()
{
	MessageBeep(MB_OK); //Why not?
	Offsets::base = (uintptr_t)GetModuleHandle(L"ac_client.exe"); //Get the base address
	Offsets::localPlayer = (Entity*)*(uintptr_t*)(Offsets::base + 0x10f4f4); //Get localPlayer address and cast it to an Entity*
	Offsets::GetCrosshairEnt = (tGetCrosshairEnt)(Offsets::base + Offsets::crosshair); //Gets the crossshair enity address
	Offsets::showCursorMod = GetModuleHandle(L"SDL.dll"); //Module that holds the ShowCursor() function that we use for input in our menu
	Offsets::showCursorAddr = (uintptr_t)GetProcAddress(Offsets::showCursorMod, "SDL_ShowCursor"); //Address of ShowCursor() function
	m.Create(); //Creates the menu
	m.Initialize(); //Initializes the menu
	printText("\f8Hack Loaded"); //  "\f#" adds color to text Colors; 0 = Green, 1 = Blue, 2 = Yellow, 3 = Red, 4 = Grey, 5 = White, 6 = Dark Orange, 7 = Dark Red, 8 = Pink, 9 = Orange
}

//Constantly does hacks
void Init::DoHacks()
{
	//Health
	doHealth();

	//Ammo
	doAmmo();

	//Recoil/Spread
	doRecoilSpread();

	//Auto
	doAuto();

	//Damage
	doDamage();

	//Triggerbot
	doTriggerbot();

	//Aimbot
	doAimbot();

	//ESP
	doESP();

	//Paperwalls
	doPaperwalls();

	//Teleport
	doTeleport();

	//Headshot
	doHeadshot();

	//Map
	doMap();
}