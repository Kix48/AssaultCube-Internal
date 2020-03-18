#pragma once
#include "offsets.h"

#ifndef HELPERS_H
#define HELPERS_H

struct Vector2;
struct Vector3;
struct glMatrix;
class Entity;
class EntList;

//Function declarations
void Patch(BYTE* dst, BYTE* src, unsigned int size);

void Nop(BYTE* dst, unsigned int size);

Vector3 Vector3Add(Vector3 src, Vector3 dst);

Vector3 Vector3Sub(Vector3 src, Vector3 dst);

Vector3 Vector3Multiply(Vector3 src, Vector3 dst);

Vector3 Vector3Divide(Vector3 src, Vector3 dst);

bool isValidEntity(Entity* ent);

bool isVisible(Entity* ent);						//Stolen from Rake Thx ;)

bool isTeamGamemode();

float DistanceVec2(Vector2 src, Vector2 dst);

float DistanceVec3(Vector3 src, Vector3 dst);

Vector3 CalcAngle(Vector3 src, Vector3 dst);

Entity* GetClosestTargetDistance(Entity* localPlayer, EntList* entList);

Entity* GetClosestTargetCrosshair(Entity* localPlayer, EntList* entList);

bool WorldToScreen(Vector3 pos, Vector2 &screen, glMatrix* matrix, int windowWidth, int windowHeight);

#endif