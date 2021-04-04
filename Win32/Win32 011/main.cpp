#include "my_common.h"
#include <vector>
#include <string>

#pragma comment(lib, "Winmm.lib")

const wchar_t* szWindowClass = L"MyWindow";
HINSTANCE g_hInstance = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {  
    switch (message)  
    {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}break;

		case WM_COMMAND: {
			auto cmdId = LOWORD(wParam);
			switch (cmdId)
			{
				case 101: {
					PlaySound(L"guitar.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				}break;

				case 102: {
					PlaySound(MAKEINTRESOURCE(SND_ALIAS_SYSTEMQUESTION), nullptr, SND_ALIAS_ID | SND_ASYNC);
				}break;
			}
		}break;
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

void WriteFileExample() {
	auto f = CreateFile(L"Test.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	// if (f == nullptr)  // <-- incorrect
	if (f ==  INVALID_HANDLE_VALUE)
		throw 1;

	const char* text = "Hello World"; 

	DWORD written = 0;
	if (!WriteFile(f, text, strlen(text), &written, nullptr)) {
		throw 1;
	}

	FILETIME creationTime, accessTime, lastWriteTime;
	GetFileTime(f, &creationTime, &accessTime, &lastWriteTime);

	FILETIME localFileTime;
	FileTimeToLocalFileTime(&lastWriteTime, &localFileTime);

	SYSTEMTIME t;
	FileTimeToSystemTime(&localFileTime, &t);
					
	CloseHandle(f);
}

void ListFilesExample() {
	wprintf(L"------ List Files ----------\n");

	WIN32_FIND_DATA fd;
	auto h = FindFirstFile(L"*", &fd);
	if (h == INVALID_HANDLE_VALUE)
		return;

	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			printf("[DIR] ");
		}
		wprintf(L"%s\n", fd.cFileName);
	} while(FindNextFile(h, &fd));

	FindClose(h);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_hInstance = hInstance;

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
    wcex.lpszMenuName   = nullptr;
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
							nullptr);

	auto btn1 = CreateWindowEx(0, L"Button", L"Sound 1", WS_CHILD | WS_VISIBLE, 10, 10, 300, 40, hwnd, (HMENU)101, hInstance, nullptr);
	auto btn2 = CreateWindowEx(0, L"Button", L"Sound 2", WS_CHILD | WS_VISIBLE, 10, 60, 300, 40, hwnd, (HMENU)102, hInstance, nullptr);

	auto font = CreateFont(	14, 0, 0, 0, FW_NORMAL, 
							false, false, false, 
							DEFAULT_CHARSET, 
							0, 0, 
							DEFAULT_QUALITY, 
							DEFAULT_PITCH, 
							L"Consolas");

	SendMessage(btn1, WM_SETFONT, (WPARAM)font, true);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	WriteFileExample();
	ListFilesExample();

    MSG msg;  
    while (GetMessage(&msg, NULL, 0, 0)) {  		
		TranslateMessage(&msg);  
		DispatchMessage(&msg);
    }

	DeleteObject(font);

	return static_cast<int>(msg.wParam);
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}