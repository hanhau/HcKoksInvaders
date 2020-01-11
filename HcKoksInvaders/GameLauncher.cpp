#include "include/GameLauncher.hpp"
#include "include/NetworkManager.hpp"

LRESULT CALLBACK __WndProc(HWND, UINT, WPARAM, LPARAM);

const enum ID {
	BUTTON_PLAY = 1,
	BUTTON_PLAY_OFFLINE = 2,
	BUTTON_EXIT = 3,
	INPUT_LOGIN = 4,
	INPUT_PASSWORD = 5,
	CHECK_FULLSCREEN = 6,
	BUTTON_REGISTER = 7
};

static const size_t STR_BUF_SIZE = 64;
static std::map<std::string, GameLaunchOptions*> _globalGloPtrs = std::map<std::string, GameLaunchOptions*>();

std::string GetDlgItemString(HWND hwnd,ID id) {
	static char buf[128];
	HWND h = GetDlgItem(hwnd, id);
	GetWindowTextA(h, buf, 128);
	return std::string(buf);
}

GameLauncher::GameLauncher(std::string title, HINSTANCE hInstance) :
	m_msg({ 0 }),
	m_dialogAppName(title)
{
	m_font = CreateFont(
		0, 0, 0, 0, 400, 
		FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		CLEARTYPE_QUALITY, DEFAULT_PITCH, 
		"Arial"
	);
	m_fontRegister = CreateFont(
		0, 0, 0, 0, 200,
		FALSE, TRUE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH,
		"Arial"
	);

	m_wndclass.style = CS_HREDRAW | CS_VREDRAW;
	m_wndclass.lpfnWndProc = __WndProc;
	m_wndclass.cbClsExtra = 0;
	m_wndclass.cbWndExtra = 0;
	m_wndclass.hInstance = hInstance;
	m_wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	m_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wndclass.lpszMenuName = NULL;
	m_wndclass.lpszClassName = m_dialogAppName.c_str();

	RegisterClass(&m_wndclass);

	m_hwndDialog = CreateWindowA(
		m_dialogAppName.c_str(),  
		title.c_str(), 
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		GetSystemMetrics(SM_CXSCREEN) / 2 - 200,
		GetSystemMetrics(SM_CYSCREEN) / 2 - 125,
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
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			20, 160, 80, 25,
			m_hwndDialog, (HMENU)ID::BUTTON_PLAY, hInstance, 0
		);
		m_hwndButtonPlayOffline = CreateWindowA(
			"BUTTON", "Offline Spielen",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			110, 160, 140, 25,
			m_hwndDialog, (HMENU)ID::BUTTON_PLAY_OFFLINE, hInstance, 0
		);
		m_hwndButtonExit = CreateWindowA(
			"BUTTON", "Verlassen",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			260, 160, 80, 25,
			m_hwndDialog, (HMENU)ID::BUTTON_EXIT, hInstance, 0
		);

		m_hwndInputLogin = CreateWindowA(
			"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			170, 15, 150, 25,
			m_hwndDialog, (HMENU)ID::INPUT_LOGIN, hInstance, 0
		);
		m_hwndInputPassword = CreateWindowExA(
			WS_EX_CLIENTEDGE,
			"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | WS_TABSTOP,
			170, 45, 150, 25,
			m_hwndDialog, (HMENU)ID::INPUT_PASSWORD, hInstance, 0
		);
		m_hwndButtonRegister = CreateWindowA(
			"BUTTON", "Auf Homepage registrieren",
			WS_CHILD | WS_VISIBLE,
			100, 77, 200, 25,
			m_hwndDialog, (HMENU)ID::BUTTON_REGISTER, hInstance, 0
		);

		m_hwndCheckFullscreen = CreateWindowExA(
			NULL, "BUTTON", "Spiel im Vollbild starten",
			BS_CHECKBOX | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			95, 120, 210, 25,
			m_hwndDialog, (HMENU)ID::CHECK_FULLSCREEN, hInstance, NULL
		);
	}

	// Set Fonts
	SendMessage(m_hwndButtonExit, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndButtonPlay, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndButtonPlayOffline, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndCheckFullscreen, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndInputLogin, WM_SETFONT, (WPARAM)m_font, 0);
	SendMessage(m_hwndInputPassword, WM_SETFONT, (WPARAM)m_font, 0);

	// Style Register Button
	SendMessage(m_hwndButtonRegister, WM_SETFONT, (WPARAM)m_fontRegister, 0);

	ShowWindow(m_hwndDialog, 1);
	UpdateWindow(m_hwndDialog);
}

void GameLauncher::processDialog(GameLaunchOptions& glo_res) 
{
	// set static glo ptr
	char winTitle[STR_BUF_SIZE];
	GetClassNameA(m_hwndDialog, winTitle, STR_BUF_SIZE);
	_globalGloPtrs[std::string(winTitle)] = &glo_res;
	
	// set glo options in gui
	if (glo_res.fullscreen) {
		CheckDlgButton(m_hwndCheckFullscreen, ID::CHECK_FULLSCREEN, BST_UNCHECKED);
	}
	else {
		CheckDlgButton(m_hwndCheckFullscreen, ID::CHECK_FULLSCREEN, BST_CHECKED);
	}

	SetWindowTextA(m_hwndInputLogin, glo_res.login.c_str());
	SetWindowTextA(m_hwndInputPassword, glo_res.password.c_str());

	BOOL ret;
	while (ret = GetMessage(&m_msg, NULL, 0, 0)) 
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	DestroyWindow(m_hwndDialog);
}

GameLauncher::~GameLauncher() {

}

LRESULT CALLBACK __WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HDC hdc = GetDC(hwnd);
	static HFONT font = nullptr;

	static char winTitle[STR_BUF_SIZE];
	GetClassNameA(hwnd, winTitle, STR_BUF_SIZE);
	GameLaunchOptions* gloPtr = _globalGloPtrs[std::string(winTitle)];

	switch (message) {
	break;
	case WM_CREATE:
		if(font == nullptr)
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
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case ID::BUTTON_REGISTER:
			ShellExecute(0, 0, "https://hcki.handata.eu/register.html", 0, 0, SW_SHOW);
			break;
		case ID::BUTTON_EXIT:
			if (gloPtr != nullptr)
				gloPtr->exit = true;

			PostQuitMessage(0);
			break;
		case ID::BUTTON_PLAY:
			printf_s("play\n");
			{
				const std::string login = GetDlgItemString(hwnd, ID::INPUT_LOGIN);
				if (login == "") {
					MessageBox(hwnd, "Kein Login eingetragen.", "Info", MB_OK);
					break;IsDlgButtonChecked(hwnd, ID::CHECK_FULLSCREEN);
				}

				const std::string password = GetDlgItemString(hwnd, ID::INPUT_PASSWORD);
				if (password == "") {
					MessageBox(hwnd, "Kein Passwort eingetragen.", "Info", MB_OK);
					break; IsDlgButtonChecked(hwnd, ID::CHECK_FULLSCREEN);
				}

				std::string errMessage = "";
				bool res = NetworkManager::verifyUserLoginValid(login, password, errMessage);

				if (!res) {
					MessageBoxA(hwnd, errMessage.c_str(), "Info", MB_OK);
					break;
				}
			
				res = NetworkManager::getUserID(login, password, gloPtr->userid);
				if (!res) {
					MessageBoxA(hwnd, "Kann ID des Users nicht vom Server holen.", "Fehler", MB_OK);
					break;
				}

				gloPtr->fullscreen = IsDlgButtonChecked(hwnd, ID::CHECK_FULLSCREEN);
				if (gloPtr->fullscreen) {
					gloPtr->res.x = GetSystemMetrics(SM_CXSCREEN);
					gloPtr->res.y = GetSystemMetrics(SM_CYSCREEN);
				}
				else {
					RECT workarea;
					SystemParametersInfoA(SPI_GETWORKAREA, 0, &workarea, 0);
					float prefAspectRatio = 640.f / 960.f;

					gloPtr->res.y = abs(workarea.bottom - workarea.top) - 20;
					gloPtr->res.x = gloPtr->res.y * prefAspectRatio;
				}
				gloPtr->exit = false;

				PostQuitMessage(0);
			}
			break;
		case ID::BUTTON_PLAY_OFFLINE:
			printf_s("play_offline\n");
			break;
		case ID::CHECK_FULLSCREEN:
			printf_s("check_fullscreen\n");
			BOOL checked = IsDlgButtonChecked(hwnd, ID::CHECK_FULLSCREEN);
			if (checked) {
				CheckDlgButton(hwnd, ID::CHECK_FULLSCREEN, BST_UNCHECKED);
			}
			else {
				CheckDlgButton(hwnd, ID::CHECK_FULLSCREEN, BST_CHECKED);
			}
			break;
		}
	}
	break;
	case WM_CLOSE: 
		if (gloPtr != nullptr)
			gloPtr->exit = true;

		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}