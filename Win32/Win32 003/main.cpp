#include "my_common.h"
#include <vector>

const wchar_t* szWindowClass = L"MyWindow";

int red = 0;
int my_counter = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_CLOSE: {
			int ret = MessageBox(hWnd, L"Quit ?", L"My Title", MB_YESNO);
			if (ret == IDYES) {
				DestroyWindow(hWnd);
			}
			return 0;
		}break;

		case WM_TIMER: {
			//printf("WM_TIMER %d\n", wParam);
			my_counter++;
			InvalidateRect(hWnd, nullptr, true);
		}break;

		case WM_SIZING: {
			auto* rc = reinterpret_cast<RECT*>(lParam);
			rc->right = rc->left + 640;
		}break;

		case WM_SIZE: {
			int w = GET_X_LPARAM(lParam);
			int h = GET_Y_LPARAM(lParam);

			//printf("WM_SIZE %d %d\n", w, h);
		}break;

		case WM_PAINT: {
			//printf("WM_PAINT\n");
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			auto brush = CreateSolidBrush(RGB(red, 0, 0));
			//red += 10;
			//red %= 256;

			SelectObject(ps.hdc, brush);

			auto& rc = ps.rcPaint;
			Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);

			DeleteObject(brush);

			SYSTEMTIME st;
			GetSystemTime(&st);

			wchar_t buf[256];
			wsprintf(buf, L"%d  %02d:%02d:%02d", my_counter, st.wHour, st.wMinute, st.wSecond);
			TextOut(ps.hdc, 100, 100, buf, wcslen(buf));
			

			EndPaint(hWnd, &ps);
		}break;

		case WM_DESTROY:  
			PostQuitMessage(0);
			return 0;

		case WM_LBUTTONDOWN: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);

			auto hdc = GetDC(hWnd);
			Rectangle(hdc, x, y, x + 20, y + 20);
			ReleaseDC(hWnd, hdc);

			InvalidateRect(hWnd, nullptr, true);
		}break;

    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	int sx = GetSystemMetrics(SM_CXSCREEN);
	int sy = GetSystemMetrics(SM_CYSCREEN);

	printf("screen size = %d %d\n", sx, sy);

	int cy = GetSystemMetrics(SM_CYCAPTION);
	printf("caption height = %d\n", cy);

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszMenuName   = NULL;  
    wcex.lpszClassName  = szWindowClass;  
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  

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