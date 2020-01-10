#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "include/Game.hpp"
#include "include/GameLauncher.hpp"
#include "include/NetworkManager.hpp"
#include <iostream>

// 2do
// formular werte holen lassen
// sichtbarkeit ingame fixen

#ifdef RELEASE_BUILD
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, INT nCmdShow)
#else
HINSTANCE hInstance = 0;
int main()
#endif
{
	NetworkManager::init("81.19.159.64","hcki.handata.eu");
	std::string res;
	NetworkManager::verifyUserLoginValid("abc", "abc", res);
	std::cout << res;

	GameLaunchOptions glo;

	GameLauncher gl("Optionen", hInstance);
	gl.processDialog(glo);

	if (glo.exit)
		return 0;

	Game game;
	game.init(glo);
	game.run();
	game.exit();

	NetworkManager::shutdown();
	return 0;
}