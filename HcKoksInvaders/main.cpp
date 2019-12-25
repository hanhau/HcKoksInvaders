#include <SFML/Graphics.hpp>
#include "include/Game.hpp"
#include <iostream>
#include <Windows.h>
#include <SFML/Audio.hpp>

#ifdef RELEASE_BUILD
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	Game game;
	game.init();
	game.run();
	game.exit();

	return 0;
}