#include <Windows.h>
#include <stdint.h>
#include "Imgui/imgui.h"
#include "offsets.h"
#include "drawing.h"
#include "config.h"

//Config for hack; used to turn features on and off
int Config::health = 1337;
int Config::armor = 1337;
int Config::ammo = 1337;
int Config::mag = 1337;
int Config::damage = 1000;
float Config::smooth = 1;
const int Config::fHeight = 15; //Change according to font
const int Config::fWidth = 8; //Change according to font
bool Config::bMenu = false;
bool Config::bHealth = false;
bool Config::bTriggerbot = false;
bool Config::bAmmo = false;
bool Config::bAuto = false;
bool Config::bRecoilSpread = false;
bool Config::bDamage = false;
bool Config::bAimbot = false;
int Config::selectedAimKey = 0;
bool Config::bVisibleAim = false;
bool Config::bSmooth = false;
bool Config::bCrosshair = false;
bool Config::bDistance = false;
bool Config::bEsp = false;
bool Config::bPaperwalls = false;
bool Config::bTeleport = false;
int Config::selectedTeleportKey = 0;
bool Config::bHeadshot = false;
bool Config::bMap = false;
bool Config::bVisibleEsp = true;
bool Config::bSnaplines = true;
bool Config::bName = true;
bool Config::bHealthbar = true;
bool Config::bTeamEsp = true;

//Color variables used in esp drawing
ImVec4 rgb::enemyBox = ImColor(255, 0, 0, 1); //Default: Red
ImVec4 rgb::enemyBoxVisible = ImColor(0, 255, 0, 1);
ImVec4 rgb::teamBox = ImColor(0, 0, 255, 1);
ImVec4 rgb::text = ImColor(0, 0, 0, 1);
ImVec4 rgb::snapline = ImColor(0, 0, 0, 1);
ImVec4 rgb::healthBarBackground = ImColor(255, 0, 0, 1);
ImVec4 rgb::healthBar = ImColor(0, 255, 0, 1);