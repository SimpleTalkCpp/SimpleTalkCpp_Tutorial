#include "my_common.h"
#include <vector>

const wchar_t* szWindowClass = L"MyWindow";

HWND scrollBarR;
HWND scrollBarG;
HWND scrollBarB;

HWND editR;
HWND editG;
HWND editB;

HWND progressBar;

HBRUSH brush;

struct MyColor {
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
};

MyColor myColor;

void updateColor() {
	if (brush) {
		DeleteObject(brush);
	}
	brush = CreateSolidBrush(RGB(myColor.r, myColor.g, myColor.b));

	wchar_t buf[256];
	wsprintf(buf, L"%u", myColor.r);
	SetWindowText(editR, buf);

	wsprintf(buf, L"%u", myColor.g);
	SetWindowText(editG, buf);

	wsprintf(buf, L"%u", myColor.b);
	SetWindowText(editB, buf);

	SendMessage(progressBar, PBM_SETPOS, myColor.r, 0);
}

WNDPROC editDefProc;

LRESULT CALLBACK EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
		case WM_KEYDOWN: {
			if (wParam == VK_RETURN) {
				printf("Enter\n");
			}
		}break;

		case WM_CHAR: {
			auto ch = wParam;
			if (!isdigit(ch))
				return 0;
		}break;
	}

	return editDefProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_CREATE: {
			auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			printf("Create Param %p\n", cs->lpCreateParams);
		}break;

		case WM_DESTROY: {
			if (brush) {
				DeleteObject(brush);
			}
			PostQuitMessage(0);
			return 0;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			RECT rc;
			GetClientRect(hWnd, &rc);

			rc.left = 400;

			FillRect(ps.hdc, &rc, brush);
			EndPaint(hWnd, &ps);
		}break;

		case WM_HSCROLL: {
			auto request = LOWORD(wParam);
			if (request == SB_THUMBPOSITION || request == SB_THUMBTRACK) {
				auto pos = HIWORD(wParam);
				auto sb = (HWND)lParam;
				if (sb == scrollBarR) {
					myColor.r = static_cast<uint8_t>(pos);
				}else if(sb == scrollBarG) {
					myColor.g = static_cast<uint8_t>(pos);
				}else if(sb == scrollBarB) {
					myColor.b = static_cast<uint8_t>(pos);
				}

				updateColor();
				SetScrollPos(sb, SB_CTL, pos, true);
				InvalidateRect(hWnd, nullptr, false);
				return 0;
			}
		}break;

		case WM_COMMAND: {
			//printf("WM_COMMAND %d\n", wParam);
		}break;
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	InitCommonControls(); //<--------

	//Window Class
	WNDCLASSEX wcex;
	//ZeroMemory(&wcex, sizeof(wcex)); // bzero
	//memset(&wcex, 0 sizeof(wcex));

	my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;  
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;  
    wcex.cbWndExtra     = 0;  
    wcex.hInstance      = hInstance;  
    wcex.hIcon          = nullptr;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszMenuName   = NULL;  
    wcex.lpszClassName  = szWindowClass;  
    wcex.hIconSm        = nullptr;

    RegisterClassEx(&wcex);
	//------------------------
	HWND hwnd = CreateWindowEx(0,
							szWindowClass, 
							L"MyWindow Title",
							WS_OVERLAPPEDWINDOW | WS_VSCROLL,
							CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
							nullptr,
							nullptr,
							hInstance,
							(LPVOID)0x123);
	//-----------------------

	int y = 10;
	int h = 30;
	int sep = 4;
	{
		auto p = CreateWindowEx(0, L"Static", L"Testing", WS_CHILD | WS_VISIBLE | SS_BLACKFRAME, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		y += h + sep;
	}

	{
		auto p = CreateWindowEx(0, L"Static", L"Testing", WS_CHILD | WS_VISIBLE, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		//SetWindowFont(p, GetStockObject(SYSTEM_FIXED_FONT), true);
		SendMessage(p, WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FIXED_FONT), true);
		y += h + sep;
	}

	{
		auto p = CreateWindowEx(0, L"Button", L"CheckBox", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		y += h + sep;
	}
	{
		auto p = CreateWindowEx(0, L"Button", L"3 State", WS_CHILD | WS_VISIBLE | BS_AUTO3STATE, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		y += h + sep;
	}

	//-------------
	{
		auto p = CreateWindowEx(0, L"Button", L"Radio 1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		y += h + sep;
	}
	{
		auto p = CreateWindowEx(0, L"Button", L"Radio 2", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		y += h + sep;
	}
	{
		auto p = CreateWindowEx(0, L"Button", L"Radio 3", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		y += h + sep;
	}

	{
		progressBar = CreateWindowEx(0, PROGRESS_CLASS, nullptr, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		SendMessage(progressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 255));
		y += h + sep;
	}

	{
		editR = CreateWindowEx(0, L"Edit",      L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 214, y, 100, h, hwnd, nullptr, hInstance, nullptr);
		editDefProc = (WNDPROC) SetWindowLongPtr(editR, GWL_WNDPROC, (LPARAM)EditWndProc);

		auto p = CreateWindowEx(0, L"SCROLLBAR", L"", WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		SetScrollRange(p, SB_CTL, 0, 255, false);
		y += h + sep;
		scrollBarR = p;
	}
	{
		editG = CreateWindowEx(0, L"Edit",      L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 214, y, 100, h, hwnd, nullptr, hInstance, nullptr);
		auto p = CreateWindowEx(0, L"SCROLLBAR", L"", WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		SetWindowLongPtr(editG, GWL_WNDPROC, (LPARAM)EditWndProc);

		SetScrollRange(p, SB_CTL, 0, 255, false);
		y += h + sep;
		scrollBarG = p;
	}
	{
		editB = CreateWindowEx(0, L"Edit",      L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 214, y, 100, h, hwnd, nullptr, hInstance, nullptr);
		auto p = CreateWindowEx(0, L"SCROLLBAR", L"", WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, y, 200, h, hwnd, nullptr, hInstance, nullptr);
		SetWindowLongPtr(editB, GWL_WNDPROC, (LPARAM)EditWndProc);

		SetScrollRange(p, SB_CTL, 0, 255, false);
		y += h + sep;
		scrollBarB = p;
	}

	updateColor();

//-----------------------
	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

// Main message loop:  
    MSG msg;  
    while (GetMessage(&msg, NULL, 0, 0))  
    {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }

	return msg.wParam;
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}