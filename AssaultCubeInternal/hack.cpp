#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include "offsets.h"
#include "hack.h"
#include "drawing.h"
#include "config.h"
#include "helpers.h"
#include "menu.h"

GL::Font font;

//Hook the function that prints to the middle of the screen in-game; Credits: Nazalas
typedef void(__cdecl* printMiddle)(const char* string, ...);
printMiddle hkPrint = (printMiddle)0x408E80;

//Sets health and armor to the value the user specifies
void doHealth()
{
	if (Config::bHealth)
	{
		Offsets::localPlayer->Health = Config::health;
		Offsets::localPlayer->Armor = Config::armor;
	}
}

//Same thing as above but with ammo
void doAmmo()
{
	if (Config::bAmmo)
	{
		*(int*)Offsets::localPlayer->CurrentWeaponPtr->Ammo = Config::ammo;
		*(int*)Offsets::localPlayer->CurrentWeaponPtr->Mag = Config::mag;
	}
}

//Prevents recoil and spread(More info can be found at the start here guide)
void doRecoilSpread()
{
	if (Config::bRecoilSpread)
	{
		Nop((BYTE*)0x463786, 10);
	}
	else
	{
		Patch((BYTE*)0x463786, (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
	}
}

//Makes the current weapon shoot insanely fast and makes weapons like pistols and snipers automatic(Don't look inside plz)
void doAuto()
{
	//Welp you did and I tried to warn you! Prepare for your eyes to burn...
	if (Config::bAuto)
	{
		//Not so bad yet... Sets our next shot wait time to zero and makes every gun automatic
		*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = 0;
		Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = true;
	}
	else
	{
		//Here is the pain :(
		//I used a switch statement to check the weapon id and from that reset the values to what they were originally

		switch (Offsets::localPlayer->CurrentWeaponPtr->WeaponId)
		{
		case 1:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::Pistol::wait; //Pistol
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::Pistol::bAuto;
			break;
		case 2:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::Carbine::wait; //Carbine
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::Carbine::bAuto;
			break;
		case 3:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::Shotgun::wait; //Shotgun
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::Shotgun::bAuto;
			break;
		case 4:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::SMG::wait; //SMG
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::SMG::bAuto;
			break;
		case 5:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::Sniper::wait; //Sniper
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::Sniper::bAuto;
			break;
		case 6:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::AR::wait; //AR
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::AR::bAuto;
			break;
		case 7:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::SMG::wait; //SMG again for some reason
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::SMG::bAuto;
			break;
		case 8:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::Grenade::wait; //Grenade
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::Grenade::bAuto;
			break;
		case 9:
			*(int*)Offsets::localPlayer->CurrentWeaponPtr->Wait = Weapon::Akimbo::wait; //Akimbo
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->bAuto = Weapon::Akimbo::bAuto;
			break;
		}
	}
}

//Allows our weapon to do crazy ammounts of damage. However, the bots also get the damage boost
void doDamage()
{
	//Sets damage
	if (Config::bDamage)
	{
		if (Offsets::localPlayer == Offsets::localPlayer->CurrentWeaponPtr->WeaponOwnerPtr)
		{
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = (short)Config::damage;
		}

	}
	else
	{
		//Basically the same as with the doAuto() function; using a switch statement to reset damage to its original value
		switch (Offsets::localPlayer->CurrentWeaponPtr->WeaponId)
		{
		case 1:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::Pistol::damage; //Pistol
			break;
		case 2:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::Carbine::damage; //Carbine
			break;
		case 3:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::Shotgun::damage; //Shotgun
			break;
		case 4:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::SMG::damage; //SMG
			break;
		case 5:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::Sniper::damage; //Sniper
			break;
		case 6:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::AR::damage; //AR
			break;
		case 7:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::SMG::damage; //SMG again for some reason
			break;
		case 8:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::Grenade::damage; //Grenade
			break;
		case 9:
			Offsets::localPlayer->CurrentWeaponPtr->WeaponInfoPtr->Damage = Weapon::Akimbo::damage; //Akimbo
			break;
		}
	}
}

//Automatically shoot if we aim at an enemy
void doTriggerbot()
{
	if (Config::bTriggerbot)
	{
		Entity* crosshairEnt = Offsets::GetCrosshairEnt(); //Get the entity we are looking at

		if (!GetAsyncKeyState(VK_LBUTTON))							//Allows us to shoot manually becuase without this check bAttacking would always be set to 0 aand only allow us to fire one round
		{
			if (isValidEntity(crosshairEnt)) //Check if the entity is valid
			{
				if (crosshairEnt->Team != Offsets::localPlayer->Team || !isTeamGamemode()) //Make sure entity is not on our team because otherwise we will have alot of friendly fire
				{
					Offsets::localPlayer->bAttacking = 1; //Shoot
				}
			}
			else
			{
				Offsets::localPlayer->bAttacking = 0; //Dont shoot
			}
		}
	}
}

//Pretty sure every 12 yr old knows what this is and is going to c&p this
void doAimbot()
{
	if (Config::bAimbot)
	{
		Entity* tgt = NULL;
		Vector3 aim;

		//I know this switch is ugly but I used it to check what our aimKey was and then do the aimbot
		switch (Config::selectedAimKey)
		{
		case 0:
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				if (Config::bCrosshair)
				{
					tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Get closest entity to our crosshair
				}

				if (Config::bDistance)
				{
					tgt = GetClosestTargetDistance(Offsets::localPlayer, Offsets::entList); //Get closest entity to us
				}

				if (tgt != NULL) //Make sure entity is valid
				{
					aim = CalcAngle(Offsets::localPlayer->HeadPos, tgt->HeadPos); //Calulate the angle we need in order to aim at their head

					if (Config::bSmooth) //Make aimbot sm000000000th
					{
						Vector3 diff = Vector3Sub(aim, Offsets::localPlayer->ViewAngles);

						Offsets::localPlayer->ViewAngles.x += diff.x / Config::smooth;
						Offsets::localPlayer->ViewAngles.y += diff.y / Config::smooth;
					}
					else
					{
						Offsets::localPlayer->ViewAngles.x = aim.x; //Set our yaw viewangle
						Offsets::localPlayer->ViewAngles.y = aim.y; //Set our pitch viewangle
					}
				}
			}
			break;

		case 1:
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (Config::bCrosshair)
				{
					tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Get closest entity to our crosshair
				}

				if (Config::bDistance)
				{
					tgt = GetClosestTargetDistance(Offsets::localPlayer, Offsets::entList); //Get closest entity to us
				}

				if (tgt != NULL) //Make sure entity is valid
				{
					aim = CalcAngle(Offsets::localPlayer->HeadPos, tgt->HeadPos); //Calulate the angle we need in order to aim at their head

					if (Config::bSmooth) //Make aimbot sm000000000th
					{
						Vector3 diff = Vector3Sub(aim, Offsets::localPlayer->ViewAngles);

						Offsets::localPlayer->ViewAngles.x += diff.x / Config::smooth;
						Offsets::localPlayer->ViewAngles.y += diff.y / Config::smooth;
					}
					else
					{
						Offsets::localPlayer->ViewAngles.x = aim.x; //Set our yaw viewangle
						Offsets::localPlayer->ViewAngles.y = aim.y; //Set our pitch viewangle
					}
				}
			}
			break;

		case 2:
			if (GetAsyncKeyState(VK_XBUTTON1))
			{
				if (Config::bCrosshair)
				{
					tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Get closest entity to our crosshair
				}

				if (Config::bDistance)
				{
					tgt = GetClosestTargetDistance(Offsets::localPlayer, Offsets::entList); //Get closest entity to us
				}

				if (tgt != NULL) //Make sure entity is valid
				{
					aim = CalcAngle(Offsets::localPlayer->HeadPos, tgt->HeadPos); //Calulate the angle we need in order to aim at their head

					if (Config::bSmooth) //Make aimbot sm000000000th
					{
						Vector3 diff = Vector3Sub(aim, Offsets::localPlayer->ViewAngles);

						Offsets::localPlayer->ViewAngles.x += diff.x / Config::smooth;
						Offsets::localPlayer->ViewAngles.y += diff.y / Config::smooth;
					}
					else
					{
						Offsets::localPlayer->ViewAngles.x = aim.x; //Set our yaw viewangle
						Offsets::localPlayer->ViewAngles.y = aim.y; //Set our pitch viewangle
					}
				}
			}
			break;

		case 3:
			if (GetAsyncKeyState(VK_XBUTTON2))
				{
					if (Config::bCrosshair)
					{
						tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Get closest entity to our crosshair
					}

					if (Config::bDistance)
					{
						tgt = GetClosestTargetDistance(Offsets::localPlayer, Offsets::entList); //Get closest entity to us
					}

					if (tgt != NULL) //Make sure entity is valid
					{
						aim = CalcAngle(Offsets::localPlayer->HeadPos, tgt->HeadPos); //Calulate the angle we need in order to aim at their head

						if (Config::bSmooth) //Make aimbot sm000000000th
						{
							Vector3 diff = Vector3Sub(aim, Offsets::localPlayer->ViewAngles);

							Offsets::localPlayer->ViewAngles.x += diff.x / Config::smooth;
							Offsets::localPlayer->ViewAngles.y += diff.y / Config::smooth;
						}
						else
						{
							Offsets::localPlayer->ViewAngles.x = aim.x; //Set our yaw viewangle
							Offsets::localPlayer->ViewAngles.y = aim.y; //Set our pitch viewangle
						}
					}
				}
			break;

		case 4:
			if (GetAsyncKeyState(VK_MENU))
				{
					if (Config::bCrosshair)
					{
						tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Get closest entity to our crosshair
					}

					if (Config::bDistance)
					{
						tgt = GetClosestTargetDistance(Offsets::localPlayer, Offsets::entList); //Get closest entity to us
					}

					if (tgt != NULL) //Make sure entity is valid
					{
						aim = CalcAngle(Offsets::localPlayer->HeadPos, tgt->HeadPos); //Calulate the angle we need in order to aim at their head

						if (Config::bSmooth) //Make aimbot sm000000000th
						{
							Vector3 diff = Vector3Sub(aim, Offsets::localPlayer->ViewAngles);

							Offsets::localPlayer->ViewAngles.x += diff.x / Config::smooth;
							Offsets::localPlayer->ViewAngles.y += diff.y / Config::smooth;
						}
						else
						{
							Offsets::localPlayer->ViewAngles.x = aim.x; //Set our yaw viewangle
							Offsets::localPlayer->ViewAngles.y = aim.y; //Set our pitch viewangle
						}
					}
				}
			break;

		case 5:
			if (GetAsyncKeyState(VK_LCONTROL))
				{
					if (Config::bCrosshair)
					{
						tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Get closest entity to our crosshair
					}

					if (Config::bDistance)
					{
						tgt = GetClosestTargetDistance(Offsets::localPlayer, Offsets::entList); //Get closest entity to us
					}

					if (tgt != NULL) //Make sure entity is valid
					{
						aim = CalcAngle(Offsets::localPlayer->HeadPos, tgt->HeadPos); //Calulate the angle we need in order to aim at their head

						if (Config::bSmooth) //Make aimbot sm000000000th
						{
							Vector3 diff = Vector3Sub(aim, Offsets::localPlayer->ViewAngles);

							Offsets::localPlayer->ViewAngles.x += diff.x / Config::smooth;
							Offsets::localPlayer->ViewAngles.y += diff.y / Config::smooth;
						}
						else
						{
							Offsets::localPlayer->ViewAngles.x = aim.x; //Set our yaw viewangle
							Offsets::localPlayer->ViewAngles.y = aim.y; //Set our pitch viewangle
						}
					}
				}
			break;
		}
	}
}

//Yup... the wallhakz
void doESP()
{
	if (Config::bEsp)
	{
		float viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);

		GL::SetupOrtho();

		for (int i = 0; i < *Offsets::numOfPlayers; i++) //Loop through entity list
		{
			if (isValidEntity(Offsets::entList->ents[i]) && isTeamGamemode() && Offsets::localPlayer->Team != Offsets::entList->ents[i]->Team && Offsets::entList->ents[i]->Health > 0 || isValidEntity(Offsets::entList->ents[i]) && !isTeamGamemode() && Offsets::entList->ents[i]->Health > 0) //Check if entity is not on our team and is valid. Gosh what a long line of code
			{
				Entity* p = Offsets::entList->ents[i];

				Vector3 headPos = p->HeadPos;
				Vector3 feetPos = p->Pos;

				headPos.z += 0.8f; //Makes boxes a little higher
				
				if (p->bCrouching) //Boxes show actual headheight better
				{
					headPos.z -= 0.5625; //When someone crouches, their head height decreases by 1.125 so I just divided by 2
				}

				Vector2 headScreenPos, feetScreenPos;
				if (WorldToScreen(headPos, headScreenPos, Offsets::vMatix, (int)GL::GetRes().x, (int)GL::GetRes().y) && WorldToScreen(feetPos, feetScreenPos, Offsets::vMatix, (int)GL::GetRes().x, (int)GL::GetRes().y))
				{
					float height = feetScreenPos.y - headScreenPos.y; //Get entity height
					float width = height / 2.0f; //Get entity width

					Vector2 tl; //Top left of box
					tl.x = headScreenPos.x - width / 2.0f;
					tl.y = headScreenPos.y;
					Vector2 br; //Bottom right of box
					br.x = headScreenPos.x + width / 2.0f;
					br.y = headScreenPos.y + height;

					//Visible
					if (Config::bVisibleEsp)
					{
						if (isVisible(p))
						{
							GL::DrawOutline(tl.x, tl.y, br.x, br.y, 2.0f, rgb::enemyBoxVisible);
						}
						else
						{
							GL::DrawOutline(tl.x, tl.y, br.x, br.y, 2.0f, rgb::enemyBox);
						}
					}
					else
					{
						GL::DrawOutline(tl.x, tl.y, br.x, br.y, 2.0f, rgb::enemyBox);
					}


					//Snaplines
					if (Config::bSnaplines)
					{
						GL::DrawLine(GL::GetRes().x / 2, GL::GetRes().y, feetScreenPos.x, feetScreenPos.y, 2.0f, rgb::snapline);
					}

					//Name
					if (Config::bName)
					{
						std::string name = p->Name;
						int tWidth = name.length() * Config::fWidth;
						float dif = width - tWidth;

						font.Print(tl.x + dif / 2, tl.y - Config::fHeight / 2, rgb::text, "%s", p->Name);
					}

					//Healthbar
					if (Config::bHealthbar)
					{			
						//Constrains the healthbar to not go past a certain position while scaling
						float x = width / 8.0f;
						if (x < 3.5)
						{
							x = 3.5;
						}

						//Background(Red part)
						Vector2 healthB;
						healthB.x = br.x + x;
						healthB.y = br.y;
						Vector2 healthT;
						healthT.x = br.x + x;
						healthT.y = tl.y;

						GL::DrawLine(healthB.x, healthB.y, healthT.x, healthT.y, 5, rgb::healthBarBackground);

						//Actual health(Green part)
						float healthBarHeight = healthT.y - healthB.y;
						float health = p->Health * healthBarHeight / 100;
						Vector2 healthA;
						healthA.y = healthB.y + health;

						GL::DrawLine(healthB.x, healthB.y, healthT.x, healthA.y, 5, rgb::healthBar);
					}
				}
			}

			//TeamEsp(Same thing as regular esp)
			if (Config::bTeamEsp && isTeamGamemode())
			{
				if (isValidEntity(Offsets::entList->ents[i]) && Offsets::localPlayer->Team == Offsets::entList->ents[i]->Team && Offsets::entList->ents[i]->Health > 0)
				{
					Entity* p = Offsets::entList->ents[i];

					Vector3 headPos = p->HeadPos;
					Vector3 feetPos = p->Pos;

					headPos.z += 0.8f;

					Vector2 headScreenPos, feetScreenPos;
					if (WorldToScreen(headPos, headScreenPos, Offsets::vMatix, (int)GL::GetRes().x, (int)GL::GetRes().y) && WorldToScreen(feetPos, feetScreenPos, Offsets::vMatix, (int)GL::GetRes().x, (int)GL::GetRes().y))
					{
						float height = feetScreenPos.y - headScreenPos.y;
						float width = height / 2.0f;

						Vector2 tl;
						tl.x = headScreenPos.x - width / 2.0f;
						tl.y = headScreenPos.y;
						Vector2 br;
						br.x = headScreenPos.x + width / 2.0f;
						br.y = headScreenPos.y + height;

						GL::DrawOutline(tl.x, tl.y, br.x, br.y, 2.0f, rgb::teamBox);

						//Name
						if (Config::bName)
						{
							std::string name = p->Name;
							int tWidth = name.length() * Config::fWidth;
							float dif = width - tWidth;

							font.Print(tl.x + dif / 2, tl.y - Config::fHeight / 2, rgb::text, "%s", p->Name);
						}
					}
				}
			}
		}

		GL::RestoreGL();
	}
}

//Allows us to shoot through walls
void doPaperwalls()
{
	if (Config::bPaperwalls)
	{
		glDisable(GL_DEPTH_TEST);
	}
}

//Teleports us to the closest enemy to our crosshair
void doTeleport()
{
	if (Config::bTeleport)
	{
		Entity* tgt;

		//Ugly again but used to check teleportKey
		switch (Config::selectedTeleportKey)
		{
		case 0:
			if (GetAsyncKeyState(VK_XBUTTON1) & 1)
			{
				tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Sets tgt to closest entity
				if (tgt != NULL)
				{
					Offsets::localPlayer->Pos = tgt->Pos; //Set our position to the targets position
					Sleep(100);
				}
			}
			break;

		case 1:
			if (GetAsyncKeyState(VK_XBUTTON2) & 1)
			{
				tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Sets tgt to closest entity
				if (tgt != NULL)
				{
					Offsets::localPlayer->Pos = tgt->Pos; //Set our position to the targets position
					Sleep(100);
				}
			}
			break;

		case 2:
			if (GetAsyncKeyState(VK_LCONTROL) & 1)
			{
				tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Sets tgt to closest entity
				if (tgt != NULL)
				{
					Offsets::localPlayer->Pos = tgt->Pos; //Set our position to the targets position
					Sleep(100);
				}
			}
			break;

		case 3:
			if (GetAsyncKeyState(0x54) & 1) //0x54 is 'T' key
			{
				tgt = GetClosestTargetCrosshair(Offsets::localPlayer, Offsets::entList); //Sets tgt to closest entity
				if (tgt != NULL)
				{
					Offsets::localPlayer->Pos = tgt->Pos; //Set our position to the targets position
					Sleep(100);
				}
			}
			break;
		}
	}
}

//Always headshot with sniper; Credit: till0sch
void doHeadshot()
{
	if (Config::bHeadshot)
	{
		Nop((BYTE*)0x461767, 2); //Nop the instructions that check if we have hit the enemy in the head
	}
	else
	{
		//Reset the instructions
		Patch((BYTE*)0x461767, (BYTE*)"\x75\x09", 2);
	}
}

//Shows all enemies on the map; Credit: Rake
void doMap()
{
	if (Config::bMap)
	{
		//Nop instructions
		Nop((BYTE*)0x04096A1, 6);
		Nop((BYTE*)0x0409FB3, 6);
	}
	else
	{
		//Reset instructions
		Patch((BYTE*)0x4096a1, (BYTE*)"\x0f\x85\x53\x01\x00\x00", 6);
		Patch((BYTE*)0x409fb3, (BYTE*)("\x0f\x85\x4a\x01\x00\x00"), 6);
	}

}

//Used to print that our hack was loaded; Credit: Nazalas
void printText(const char* text)
{
	hkPrint(text);
}