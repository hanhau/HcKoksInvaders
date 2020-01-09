#include "include/GameLauncher.hpp"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GameLauncher::GameLauncher(std::string title,
						   GameLaunchOptions& glo,
						   HINSTANCE hInstance) :
	m_msg({ 0 })
{
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
		m_dialogAppName,  // window class name
		TEXT(title.c_str()), // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		500,              // initial x size
		250,              // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);                     // creation parameters

	// Creation of SubHwnds
	{
		m_hwndButtonPlay = CreateWindowA(
			"BUTTON", "Spielen",
			WS_CHILD | WS_VISIBLE,
			70, 160, 80, 25,
			m_hwndDialog, 0, hInstance, 0
		);
		m_hwndButtonPlayOffline = CreateWindowA(
			"BUTTON", "Offline Spielen",
			WS_CHILD | WS_VISIBLE,
			160, 160, 140, 25,
			m_hwndDialog, 0, hInstance, 0
		);
		m_hwndButtonExit = CreateWindowA(
			"BUTTON", "Verlassen",
			WS_CHILD | WS_VISIBLE,
			310, 160, 80, 25,
			m_hwndDialog, 0, hInstance, 0
		);

		m_hwndInputLogin = CreateWindowA(
			"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			220, 15, 150, 25,
			m_hwndDialog, 0, hInstance, 0
		);

		m_hwndInputPassword = CreateWindowA(
			"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			220, 45, 150, 25,
			m_hwndDialog, 0, hInstance, 0
		);

		m_hwndCheckFullscreen = CreateWindowExA(
			NULL, "BUTTON", "Spiel im Vollbild starten",
			BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
			145, 120, 210, 25,
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
		textLoginRect.right = 210;
		textLoginRect.top = 17;
		DrawText(
			hdc, textLogin.c_str(), textLogin.length(),
			&textLoginRect, DT_RIGHT | DT_SINGLELINE | DT_NOCLIP
		);
		const std::string textPassword = "Passwort: ";
		RECT textPasswordRect = { 0 };
		textPasswordRect.right = 210;
		textPasswordRect.top = 47;
		DrawText(
			hdc, textPassword.c_str(), textPassword.length(),
			&textPasswordRect, DT_RIGHT | DT_SINGLELINE | DT_NOCLIP
		);
	}
	break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}