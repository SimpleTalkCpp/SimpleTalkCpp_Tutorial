#include "my_common.h"
#include <vector>
#include "resource.h"

const wchar_t* szWindowClass = L"MyWindow";
HMENU subMenu;

void MyInsertMenuItem(HMENU menu, UINT pos, const wchar_t* caption, UINT cmdId, HMENU subMenu = nullptr) {
	MENUITEMINFO info;
	my_bzero(info);
	info.cbSize = sizeof(info);
	info.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_SUBMENU;

	info.fType = MFT_STRING;
	info.wID = cmdId;
	info.hSubMenu = subMenu;

	info.dwTypeData = const_cast<wchar_t*>(caption);
	info.cch = static_cast<UINT>(wcslen(caption));

	InsertMenuItem(menu, pos, true, &info);
}

void MySetMenuItemChecked(HMENU menu, UINT cmdId, bool checked) {
	MENUITEMINFO info;
	my_bzero(info);
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;

	GetMenuItemInfo(menu, cmdId, false, &info);
	if (checked) { 
		info.fState |= MFS_CHECKED; //bitwise
	}else{
		info.fState &= ~MFS_CHECKED;
	}
	SetMenuItemInfo(menu, cmdId, false, &info);
}

bool MyGetMenuItemChecked(HMENU menu, UINT cmdId) {
	MENUITEMINFO info;
	my_bzero(info);
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;

	GetMenuItemInfo(menu, cmdId, false, &info);
	return (info.fState & MFS_CHECKED) != 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}break;

		case WM_CREATE: {
			auto mainMenu = GetMenu(hWnd);
			auto menuFile = GetSubMenu(mainMenu, 0);

			subMenu = CreateMenu();
			MyInsertMenuItem(subMenu, -1, L"Sub Item 1", 200);
			MyInsertMenuItem(subMenu, -1, L"Sub Item 2", 201);
			MyInsertMenuItem(subMenu, -1, L"Sub Item 3", 202);
			MyInsertMenuItem(subMenu, -1, L"Sub Item 4", 203);

			MyInsertMenuItem(menuFile, 0, L"Item 1", 100);
			MyInsertMenuItem(menuFile, 1, L"Item 2", 101);
			MyInsertMenuItem(menuFile, 2, L"SubMenu ", 0, subMenu);
			MySetMenuItemChecked(menuFile, 101, true);
		}break;

		case WM_COMMAND: {
			auto cmdId = LOWORD(wParam);
			printf("WM_COMMAND id = %d\n", cmdId);

			switch (cmdId) {
				case ID_FILE_EXIT: {
					SendMessage(hWnd, WM_CLOSE, 0, 0);					
				}break;

				case 101: {
					auto mainMenu = GetMenu(hWnd);
					auto menuFile = GetSubMenu(mainMenu, 0);

					bool b = MyGetMenuItemChecked(menuFile, 101);
					MySetMenuItemChecked(menuFile, 101, !b);
				}break;
			}
		}break;

		case WM_RBUTTONUP: {
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			ClientToScreen(hWnd, &pt);
			TrackPopupMenu(subMenu, 0, pt.x, pt.y, 0, hWnd, nullptr);
		}break;

    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wcex;
	my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;  
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;  
    wcex.cbWndExtra     = 0;  
    wcex.hInstance      = hInstance;  
    wcex.hIcon          = nullptr;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;  
    wcex.hIconSm        = nullptr;

    RegisterClassEx(&wcex);
	//------------------------
	HWND hwnd = CreateWindowEx(0,
							szWindowClass, 
							L"MyWindow Title",
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
							nullptr,
							nullptr,
							hInstance,
							(LPVOID)0x123);

	auto hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1)); // <----------- Load

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

    MSG msg;  
    while (GetMessage(&msg, NULL, 0, 0)) {  		
		if (!TranslateAccelerator(hwnd, hAccel, &msg)) { // <------------ Add this to support accelerator
			TranslateMessage(&msg);  
			DispatchMessage(&msg);
		}
    }
	return static_cast<int>(msg.wParam);
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}