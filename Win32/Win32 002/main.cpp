#include "my_common.h"

const wchar_t* szWindowClass = L"MyWindow";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_DESTROY:  
			PostQuitMessage(0);
			return 0;

		case WM_LBUTTONDOWN: {
			auto x = (int16_t)(lParam & 0xffff);
			auto y = (int16_t)((lParam >> 16) & 0xffff);
			printf("Left Button Down %d %d\n", x, y);

			HDC hdc = GetDC(hWnd);

			HBRUSH brush = CreateHatchBrush(HS_BDIAGONAL, RGB(0,100, 255));
			HPEN pen = CreatePen(PS_DASH, 1, RGB(255, 0, 0));

			SelectObject(hdc, brush);
			SelectObject(hdc, pen);

			//SelectObject(hdc, GetStockObject(GRAY_BRUSH));

			//SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

			//TextOut(hdc, x, y, L"Hello", 5);

			//POINT pt;
			//MoveToEx(hdc, x, y, &pt);
			//LineTo(hdc, x + 100, y + 50);
			//LineTo(hdc, x + 0, y + 50);
			//LineTo(hdc, x + 50, y + 10);

			//for (int j = 0; j < 256; j++) {
			//	for (int i = 0; i < 256; i++) {
			//		SetPixel(hdc, x+i, y+j, RGB(i, j, 0));
			//	}
			//}
		
			//Rectangle(hdc, x, y, x + 50, y + 50);


			DeleteObject(brush);
			DeleteObject(pen);

			ReleaseDC(hWnd, hdc);

			break;
		}
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	printf("Hello\n");

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
	HWND hwnd = CreateWindowEx(	0,
								szWindowClass, 
								L"MyWindow Title",
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								nullptr,
								nullptr,
								hInstance,
								0);

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