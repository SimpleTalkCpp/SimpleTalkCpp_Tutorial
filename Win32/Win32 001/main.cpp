#include <stdio.h>
#include <windows.h>
//#include <windowsx.h>
#include <stdint.h>

//Win32 API (Application Programming Interface)

// H -> Handle
// LPSTR -> Long Pointer String
// WINAPI -> __stdcall function calling convention

const wchar_t* szWindowClass = L"MyWindow";

//L -> Long  -> 32bit int

//Byte    (1 byte)
//WORD	(2 bytes) <-- 16 bit
//DWORD	(4 bytes) // double word
//Long    (4 bytes)

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
			break;
		}
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

template<class T>
inline void my_bzero(T& s) {
	memset(&s, 0, sizeof(s));
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

	HWND hwnd = CreateWindow(szWindowClass, 
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