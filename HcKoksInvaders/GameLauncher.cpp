#include "include/GameLauncher.hpp"

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const enum ID {
	BUTTON_PLAY = 1,
	BUTTON_PLAY_OFFLINE = 2,
	BUTTON_EXIT = 3,
	INPUT_LOGIN = 4,
	INPUT_PASSWORD = 5,
	CHECK_FULLSCREEN = 6
};


GameLauncher::GameLauncher(std::string title, HINSTANCE hInstance) :
	m_msg({ 0 })
{
	InitCommonControls();

	m_dialogAppName = TEXT((char* const)title.c_str());
	m_font = CreateFont(
		0, 0, 0, 0, 400, 
		FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		CLEARTYPE_QUALITY, DEFAULT_PITCH, 
		"Arial"
	);

	m_wndclass.style = CS_HREDRAW | CS_VREDRAW;
	m_wndclass.lpfnWndProc = WndProc;
	m_wndclass.cbClsExtra = 0;
	m_wndclass.cbWndExtra = 0;
	m_wndclass.hInstance = hInstance;
	m_wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	m_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wndclass.lpszMenuName = NULL;
	m_wndclass.lpszClassName = m_dialogAppName;

	RegisterClass(&m_wndclass);

	m_hwndDialog = CreateWindow(
		m_dialogAppName,  
		TEXT(title.c_str()), 
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,       
		CW_USEDEFAULT,       
		400,              
		250,              
		NULL,             
		NULL,             
		hInstance,        
		NULL);        

	// Creation of SubHwnds
	{
		m_hwndButtonPlay = CreateWindowA(
			"BUTTON", "Spielen",
			WS_CHILD | WS_VISIBLE,
			20, 160, 80, 25,
			m_hwndDialog, (HMENU)ID::BUTTON_PLAY, hInstance, 0
		);
		m_hwndButtonPlayOffline = CreateWindowA(
			"BUTTON", "Offline Spielen",
			WS_CHILD | WS_VISIBLE,
			110, 160, 140, 25,
			m_hwndDialog, (HMENU)ID::BUTTON_PLAY_OFFLINE, hInstance, 0
		);
		m_hwndButtonExit = CreateWindowA(
			"BUTTON", "Verlassen",
			WS_CHILD | WS_VISIBLE,
			260, 160, 80, 25,
			m_hwndDialog, (HMENU)xxx, hInstance, 0
		);

		m_hwndInputLogin = CreateWindowA(
			"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			170, 15, 150, 25,
			m_hwndDialog, 0, hInstance, 0
		);

		m_hwndInputPassword = CreateWindowExA(
			WS_EX_CLIENTEDGE,
			"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
			170, 45, 150, 25,
			m_hwndDialog, 0, hInstance, 0
		);

		m_hwndCheckFullscreen = CreateWindowExA(
			NULL, "BUTTON", "Spiel im Vollbild starten",
			BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
			95, 120, 210, 25,
			m_hwndDialog, NULL, hInstance, NULL
		);
	}

	// Set Fonts
	SendMessage(m_hwndButtonExit, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndButtonPlay, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndButtonPlayOffline, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndCheckFullscreen, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndInputLogin, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndInputPassword, WM_SETFONT, (WPARAM)m_font, 0);

	ShowWindow(m_hwndDialog, 1);
	UpdateWindow(m_hwndDialog);
}

void GameLauncher::processDialog(GameLaunchOptions& glo_res) {
	BOOL ret;
	
	while (1) {
		ret = GetMessage(&m_msg, NULL, 0, 0);

		if (ret > 0) {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
		else
			break;

		Sleep(10);
	}
}

GameLauncher::~GameLauncher() {

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc = GetDC(hwnd);
	static HFONT font;

	switch (message) {
	case WM_CREATE:
		font = CreateFont(0, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Arial");
		break;
	case WM_PAINT:
	{
		SelectObject(hdc, font);
		const std::string textLogin = "Benutzername: ";
		RECT textLoginRect = { 0 };
		textLoginRect.right = 160;
		textLoginRect.top = 17;
		DrawText(
			hdc, textLogin.c_str(), textLogin.length(),
			&textLoginRect, DT_RIGHT | DT_SINGLELINE | DT_NOCLIP
		);
		const std::string textPassword = "Passwort: ";
		RECT textPasswordRect = { 0 };
		textPasswordRect.right = 160;
		textPasswordRect.top = 47;
		DrawText(
			hdc, textPassword.c_str(), textPassword.length(),
			&textPasswordRect, DT_RIGHT | DT_SINGLELINE | DT_NOCLIP
		);
	}
	break;
	case WM_CLOSE: 
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}