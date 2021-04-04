#include "my_common.h"
#include <vector>

const wchar_t* szWindowClass = L"MyWindow";

int red = 0;
int my_counter = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_DESTROY:  
			PostQuitMessage(0);
			return 0;

		case WM_LBUTTONDOWN: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);

			bool ctrl = (wParam & MK_CONTROL) != 0;
			printf("WM_LBUTTONDOWN x=%d y=%d wParam=%X\n", x, y, wParam);
			SetCapture(hWnd);
		}break;

		case WM_LBUTTONUP: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			printf("WM_LBUTTONUP x=%d y=%d\n", x, y);
			ReleaseCapture();
		}break;

		case WM_MOUSEWHEEL: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			auto wheel =  GET_WHEEL_DELTA_WPARAM(wParam);
			printf("WM_MOUSEWHEEL %d %d %d\n", x, y, wheel);
		}break;

		case WM_MOUSEMOVE: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			//printf("WM_MOUSEMOVE %d %d\n", x, y);
		}break;

		case WM_KEYDOWN: {
			if (wParam == VK_UP) {
				printf("UP\n");
			}
//			printf("WM_KEYDOWN wParam=%X lParam=%X\n", wParam, lParam);
		}break;

		case WM_CHAR: {
			printf("WM_CHAR wParam=%X '%c' lParam=%X\n", wParam, wParam, lParam);
		}break;

		case WM_UNICHAR: {
			printf("WM_UNICHAR wParam=%X '%c' lParam=%X\n", wParam, wParam, lParam);
		}break;

    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
							nullptr,
							nullptr,
							hInstance,
							0);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	SetTimer(hwnd, 11, 1, nullptr);

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