#pragma once
#include <windows.h>
#include <string>
#include "GameLaunchOptions.hpp"
#include <map>

class GameLauncher {
	const std::string m_dialogAppName;

	HWND m_hwndDialog;
	HWND m_hwndButtonPlay;
	HWND m_hwndButtonPlayOffline;
	HWND m_hwndButtonRegister;
	HWND m_hwndButtonExit;
	HWND m_hwndInputLogin;
	HWND m_hwndInputPassword;
	HWND m_hwndCheckFullscreen;

	MSG m_msg;
	WNDCLASS m_wndclass;

	HFONT m_font;
	HFONT m_fontRegister;
public:
	GameLauncher(std::string title,HINSTANCE hInstance);
	void processDialog(GameLaunchOptions& glo_res);
	~GameLauncher();
};