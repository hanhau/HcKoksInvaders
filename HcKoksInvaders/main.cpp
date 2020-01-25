#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "include/Game.hpp"
#include "include/GameLauncher.hpp"
#include "include/NetworkManager.hpp"

#ifdef RELEASE_BUILD
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, INT nCmdShow)
#else
HINSTANCE hInstance = 0;
int main()
#endif
{
	NetworkManager::init("81.19.159.64","hcki.handata.eu");
	TextureManager.preloadToMemory();
	ProgramManager.preloadToMemory();

	GameLaunchOptions gameLaunchOptions;
	gameLaunchOptions.loadFromFileOrDefault();

	GameLauncher gameLauncher("HcKoksInvaders Launcher", hInstance);
	gameLauncher.processDialog(gameLaunchOptions);

	gameLaunchOptions.saveToFile();

	if (gameLaunchOptions.exit)
		return 0;

	Game game;
	game.init(gameLaunchOptions);
	game.run();
	game.exit();

	NetworkManager::shutdown();
	return 0;
}