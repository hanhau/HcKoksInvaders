#pragma once
#include <windows.h>
#include <string>
#include "GameLaunchOptions.hpp"


class GameLauncher {
	TCHAR* m_dialogAppName;

	HWND m_hwndDialog;
	HWND m_hwndButtonPlay;
	HWND m_hwndButtonPlayOffline;
	HWND m_hwndButtonExit;
	HWND m_hwndInputLogin;
	HWND m_hwndInputPassword;
	HWND m_hwndCheckFullscreen;

	MSG m_msg;
	WNDCLASS m_wndclass;

	HFONT m_font;

public:
	GameLauncher(std::string title,GameLaunchOptions& glo,HINSTANCE hInstance);
	~GameLauncher();
};