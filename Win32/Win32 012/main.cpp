#include "my_common.h"
#include <vector>
#include <string>

#include "MyDll/MyDll.h"
#pragma comment(lib, "MyDll.lib")

const wchar_t* szWindowClass = L"MyWindow";
HINSTANCE g_hInstance = nullptr;

void MemoryMappingExample() {
	auto file = CreateFile(L"Test.txt", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		throw 1;

	LARGE_INTEGER fileSize;
	GetFileSizeEx(file, &fileSize);

	auto map = CreateFileMapping(file, nullptr, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, nullptr);
	if (!map) {
		CloseHandle(file);
		throw 1;
	}

	auto view = (char*)MapViewOfFile(map, FILE_MAP_READ, 0, 0, fileSize.LowPart);
	if (!view) {
		CloseHandle(map);
		CloseHandle(file);
		throw 1;
	}

	printf("FileMapping = ");
	for (SIZE_T i=0; i<fileSize.LowPart; i++) {
		printf("%c", view[i]);
	}
	printf("\n");

	UnmapViewOfFile(view);
	CloseHandle(map);
	CloseHandle(file);
}

void CompileTimeLinkDllExample() {
	int v = MyDll_Testing();
	printf("CompileTimeLinkDllExample = %d\n\n", v);
}

HMODULE myDll;
typedef int (*MyTestFunc)();
void DynamicLoadDllExample() {
	myDll = LoadLibrary(L"MyDll.dll");
	if (!myDll)
		throw 1;

	 auto fp = (MyTestFunc)GetProcAddress(myDll, "MyDll_Testing");
	 if (!fp)
		 throw 1;

	 int v = fp();
	 printf("DynamicLoadDllExample = %d\n\n", v);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {  
    switch (message)  
    {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}break;
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CompileTimeLinkDllExample();

	DynamicLoadDllExample();
	MemoryMappingExample();

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

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

    MSG msg;  
    while (GetMessage(&msg, NULL, 0, 0)) {  		
		TranslateMessage(&msg);  
		DispatchMessage(&msg);
    }

	return static_cast<int>(msg.wParam);
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}