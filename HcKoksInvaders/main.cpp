#include "include/Game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <SFML/Audio.hpp>
#include "include/NetworkManager.hpp"

#ifdef RELEASE_BUILD
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	NetworkManager::init();

	GameLaunchOptions glo;

	int choice = MessageBoxW(
		NULL,
		(LPCWSTR)L"HcKoksInvaders im Vollbild starten?",
		(LPCWSTR)L"Vollbild",
		MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2
	);

	switch (choice) {
	case IDYES:
		glo = {
			glm::ivec2(
				GetSystemMetrics(SM_CXSCREEN),
				GetSystemMetrics(SM_CYSCREEN)
			),
			true
		};
		break;
	case IDNO:
		glo = {
			glm::ivec2(640,960),
			false
		};
		break;
	case IDCANCEL:
		return 0;
		break;
	}

	Game game;
	game.init(glo);
	game.run();
	game.exit();

	NetworkManager::shutdown();
	return 0;
}