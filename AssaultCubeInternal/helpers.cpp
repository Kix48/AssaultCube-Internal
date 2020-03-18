#define _USE_MATH_DEFINES

#include <Windows.h>
#include <iostream>
#include <math.h>
#include <float.h>
#include "drawing.h"
#include "offsets.h"
#include "config.h"

//Patches bytes. Explained in the beginners guide
void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

//Nops instructions. Explained in the beginners guide
void Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

//Next 4 functions do math with vectors(Didn't even use most of them lol)
Vector3 Vector3Add(Vector3 src, Vector3 dst)
{
	Vector3 add;
	add.x = src.x + dst.x;
	add.y = src.y + dst.y;
	add.z = src.z + dst.z;
	return add;
}

Vector3 Vector3Sub(Vector3 src, Vector3 dst)
{
	Vector3 sub;
	sub.x = src.x - dst.x;
	sub.y = src.y - dst.y;
	sub.z = src.z - dst.z;
	return sub;
}

Vector3 Vector3Multiply(Vector3 src, Vector3 dst)
{
	Vector3 multiply;
	multiply.x = src.x * dst.x;
	multiply.y = src.y * dst.y;
	multiply.z = src.z * dst.z;
	return multiply;
}

Vector3 Vector3Divide(Vector3 src, Vector3 dst)
{
	Vector3 divide;
	divide.x = src.x / dst.x;
	divide.y = src.y / dst.y;
	divide.z = src.z / dst.z;
	return divide;
}

//Checks if the entity is valid. Credit: Rake for having it somewhere
bool isValidEntity(Entity* ent)
{
	if (ent)
	{
		if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0)
		{
			return true;
		}
	}
	return false;
}

//Traceline function to see if entity is visible
bool isVisible(Entity* ent)
{
	TraceResult trace;
	trace.collided = false;
	Vector3 from = Offsets::localPlayer->HeadPos;
	Vector3 to = ent->HeadPos;

	__asm 
	{
		push 0; bSkipTags
		push 0; bCheckPlayers
		push Offsets::localPlayer
		push to.z
		push to.y
		push to.x
		push from.z
		push from.y
		push from.x
		lea eax, [trace]
		call Offsets::traceLine;
		add esp, 36
	}
	return !trace.collided;
}     

//Checks if the current gamemode is a team gamemode
bool isTeamGamemode()
{
	int gameMode = *(int*)Offsets::gameMode;

	return (gameMode == 0 || gameMode == 4 || gameMode == 5 || gameMode == 7 || gameMode == 11 || gameMode == 13 || gameMode == 14 || gameMode == 16 || gameMode == 17 || gameMode == 20 || gameMode == 21);
}

//Next 2 functions do distance calculations with vectors
float DistanceVec2(Vector2 dst, Vector2 src)
{
	float distance;
	distance = sqrtf(powf(dst.x - src.x, 2) + powf(dst.y - src.y, 2));
	return distance;
}

float DistanceVec3(Vector3 dst, Vector3 src)
{
	float distance;
	distance = sqrtf(powf(dst.x - src.x, 2) + powf(dst.y - src.y, 2) + powf(dst.z - src.z, 2));
	return distance;
}

//CalcAngle function that literally gave me so many problems at first but I eventually figured out the math(I used ms paint to do visuals lol)
Vector3 CalcAngle(Vector3 src, Vector3 dst)
{
	Vector3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / (float)M_PI * 180 + 180;
	angle.y = asinf((dst.z - src.z) / DistanceVec3(src, dst)) * 180 / (float)M_PI;
	angle.z = 0;

	return angle;
}

//Gets closest entity to us
Entity* GetClosestTargetDistance(Entity* localPlayer, EntList* entList)
{
	float oDist = FLT_MAX;
	float nDist = 0;
	Entity* target = nullptr;

	for (int i = 0; i < *Offsets::numOfPlayers; i++)
	{
		if (Config::bVisibleAim)
		{
			if (isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && isTeamGamemode() && Offsets::entList->ents[i]->Team != Offsets::localPlayer->Team && isVisible(entList->ents[i]) || isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && !isTeamGamemode() && isVisible(entList->ents[i]))
			{

				nDist = DistanceVec3(localPlayer->Pos, Offsets::entList->ents[i]->Pos);

				if (nDist < oDist)
				{
					oDist = nDist;
					target = Offsets::entList->ents[i];
				}
			}
		}
		else
		{
			if (isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && isTeamGamemode() && Offsets::entList->ents[i]->Team != Offsets::localPlayer->Team || isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && !isTeamGamemode())
			{

				nDist = DistanceVec3(localPlayer->Pos, Offsets::entList->ents[i]->Pos);

				if (nDist < oDist)
				{
					oDist = nDist;
					target = Offsets::entList->ents[i];
				}
			}
		}
	}
	return target;
}

//Gets closest entity to our crosshair
Entity* GetClosestTargetCrosshair(Entity* localPlayer, EntList* entList)
{
	float oDist = FLT_MAX;
	float nDist = 0;
	Entity* target = nullptr;
	Vector2 screenPos;

	for (int i = 0; i < *Offsets::numOfPlayers; i++)
	{
		Vector2 screenRes = GL::GetRes();

		if (Config::bVisibleAim)
		{
			if (isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && isTeamGamemode() && Offsets::entList->ents[i]->Team != Offsets::localPlayer->Team && isVisible(entList->ents[i]) || isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && !isTeamGamemode() && isVisible(entList->ents[i]))
			{
				if (WorldToScreen(Offsets::entList->ents[i]->HeadPos, screenPos, Offsets::vMatix, (int)screenRes.x, (int)screenRes.y))
				{
					screenRes.x /= 2;
					screenRes.y /= 2;

					nDist = DistanceVec2(screenPos, screenRes);

					if (nDist < oDist)
					{
						oDist = nDist;
						target = Offsets::entList->ents[i];
					}
				}
			}
		}
		else
		{
			if (isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && isTeamGamemode() && Offsets::entList->ents[i]->Team != Offsets::localPlayer->Team || isValidEntity(Offsets::entList->ents[i]) && Offsets::entList->ents[i]->Health > 0 && !isTeamGamemode())
			{
				if (WorldToScreen(Offsets::entList->ents[i]->HeadPos, screenPos, Offsets::vMatix, (int)screenRes.x, (int)screenRes.y))
				{
					screenRes.x /= 2;
					screenRes.y /= 2;

					nDist = DistanceVec2(screenPos, screenRes);

					if (nDist < oDist)
					{
						oDist = nDist;
						target = Offsets::entList->ents[i];
					}
				}
			}
		}
	}

	return target;
}

//World to screen function that can be found on the forum(Kinda a c&p... but after I understood the concept, of course)
bool WorldToScreen(Vector3 pos, Vector2 &screen, glMatrix* matrix, int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix->matrix[0] + pos.y * matrix->matrix[4] + pos.z * matrix->matrix[8] + matrix->matrix[12];
	clipCoords.y = pos.x * matrix->matrix[1] + pos.y * matrix->matrix[5] + pos.z * matrix->matrix[9] + matrix->matrix[13];
	clipCoords.z = pos.x * matrix->matrix[2] + pos.y * matrix->matrix[6] + pos.z * matrix->matrix[10] + matrix->matrix[14];
	clipCoords.w = pos.x * matrix->matrix[3] + pos.y * matrix->matrix[7] + pos.z * matrix->matrix[11] + matrix->matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//Perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}