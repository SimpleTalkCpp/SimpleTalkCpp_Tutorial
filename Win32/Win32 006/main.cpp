#include "my_common.h"
#include <vector>

const wchar_t* szWindowClass = L"MyWindow";
const wchar_t* szButtonClass = L"MyButton";

int red = 0;
int my_counter = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		case WM_COMMAND: {
			printf("WM_COMMAND %d\n", wParam);
		}break;
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

struct MyButtonData {
	bool down = false;
	int yourData;
};

bool isInsideRect(RECT& rc, LONG x, LONG y) {
	return (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom);
}

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_CREATE: {
			auto* data = new MyButtonData();
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
		}break;

		case WM_DESTROY: {
			auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			delete data;
		}break;

		case WM_PAINT: {
			auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			RECT rc;
			GetClientRect(hWnd, &rc);
			SelectObject(ps.hdc, GetStockObject(BLACK_PEN));

			SelectObject(ps.hdc, data->down ? GetStockObject(LTGRAY_BRUSH) : GetStockObject(WHITE_BRUSH));

			Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);

			wchar_t text[512];
			GetWindowText(hWnd, text, 512);

			SetBkMode(ps.hdc, TRANSPARENT);
			DrawText(ps.hdc, text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hWnd, &ps);
		}break;

		case WM_LBUTTONDOWN: {
			auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			data->down = true;
			SetCapture(hWnd);
			InvalidateRect(hWnd, nullptr, false);
		}break;

		case WM_LBUTTONUP: {
			auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			data->down = false;
			ReleaseCapture();
			InvalidateRect(hWnd, nullptr, false);

			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);

			RECT rc;
			GetClientRect(hWnd, &rc);

			if (isInsideRect(rc, x, y)) {				
				SendMessage(GetParent(hWnd), WM_COMMAND, (WPARAM)GetMenu(hWnd), 0);				
			}
		}break;

    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

void initButtonClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;  
    wcex.lpfnWndProc    = ButtonProc;
    wcex.cbClsExtra     = 0;  
    wcex.cbWndExtra     = 0;  
    wcex.hInstance      = hInstance;  
    wcex.hIcon          = nullptr;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszMenuName   = NULL;  
    wcex.lpszClassName  = szButtonClass;  
    wcex.hIconSm        = nullptr;

    RegisterClassEx(&wcex);
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
	//--- Button ------
	initButtonClass(hInstance);
	CreateWindowEx(0, szButtonClass, L"YES", WS_CHILD | WS_VISIBLE, 10,  10, 200, 40, hwnd, (HMENU)1, hInstance, nullptr);
	CreateWindowEx(0, szButtonClass, L"NO",  WS_CHILD | WS_VISIBLE, 220, 10, 200, 40, hwnd, (HMENU)2, hInstance, nullptr);

	CreateWindowEx(0, L"Button", L"Test", WS_CHILD | WS_VISIBLE, 10, 100, 200, 40, hwnd, (HMENU)10, hInstance, nullptr);
	CreateWindowEx(0, L"Edit",   L"Text Input...", WS_CHILD | WS_VISIBLE | WS_BORDER, 220, 100, 200, 40, hwnd, nullptr, hInstance, nullptr);

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