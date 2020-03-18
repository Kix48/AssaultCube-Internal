#pragma once
#include "Imgui/imgui.h"

#ifndef CONFIG_H
#define CONFIG_H

//Config variable declarations
namespace Config 
{
	extern int health;
	extern int armor;
	extern int ammo;
	extern int mag;
	extern int damage;
	extern float smooth;
	extern const int fHeight;
	extern const int fWidth;
	extern bool bMenu;
	extern bool bHealth;
	extern bool bTriggerbot;
	extern bool bAmmo;
	extern bool bAuto;
	extern bool bRecoilSpread;
	extern bool bDamage;
	extern bool bAimbot;
	extern int selectedAimKey;
	extern bool bVisibleAim;
	extern bool bSmooth;
	extern bool bCrosshair;
	extern bool bDistance;
	extern bool bEsp;
	extern bool bPaperwalls;
	extern bool bTeleport;
	extern int selectedTeleportKey;
	extern bool bHeadshot;
	extern bool bMap;
	extern bool bVisibleEsp;
	extern bool bSnaplines;
	extern bool bName;
	extern bool bHealthbar;
	extern bool bTeamEsp;
}

//Color variable declarations
namespace rgb
{
	extern ImVec4 enemyBox;
	extern ImVec4 enemyBoxVisible;
	extern ImVec4 teamBox;
	extern ImVec4 text;
	extern ImVec4 snapline;
	extern ImVec4 healthBarBackground;
	extern ImVec4 healthBar;
}

#endif