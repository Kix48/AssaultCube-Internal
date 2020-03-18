#include <Windows.h>
#include <iostream>
#include "init.h"
#include "hook.h"
#include "hack.h"
#include "offsets.h"
#include "config.h"
#include "menu.h"

Init init;
Menu menu;

//Next two lines are used for SwapBuffers hook
typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers wglSwapBuffersGateway;

//SwapBuffers is called every frame so it is a good place to have our hack running in
BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
	Offsets::entList = *(EntList**)Offsets::entityListAddr; //Resets entity list incase of server change

	if (GetAsyncKeyState(VK_INSERT) & 1) //Checks if we want to open menu
	{
		Config::bMenu = !Config::bMenu;
	}

	menu.Render(); //Renders menu
	init.DoHacks(); //Does hacks

	return wglSwapBuffersGateway(hDc);
}

DWORD __stdcall Thread(LPVOID param)
{

	//Shhhhhhh below is used for testing purposes only ;)
	/*AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);*/

	Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);

	SwapBuffersHook.Enable(); //Enables our hook

while (1) //Infinite loop
	{
		if (GetAsyncKeyState(VK_DELETE) & 1) //Checks if we want to exit the hack entirely
		{
			break;
		}
	}

	SwapBuffersHook.Disable(); //Disables our hook

	menu.Shutdown(); //Unloads our menu

	MessageBeep(MB_OK); //Lets user know that hack was closed
	Sleep(100); //Gives menu time to hurry up and close properly
	
	//fclose(f); Also used for testing ;)
	//FreeConsole(); Testing again ;)

	FreeLibraryAndExitThread((HMODULE)param, 0); //Finally we unload the DLL
}

//Nothing much to be said about here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, Thread, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

