#include "my_common.h"
#include <vector>
#include <string>
#include "resource.h"

const wchar_t* szWindowClass = L"MyWindow";
HINSTANCE g_hInstance = nullptr;

struct MyOpenFileFilter {
	MyOpenFileFilter() {}
	MyOpenFileFilter(const wchar_t* desc_, const wchar_t* ext_) 
		: desc(desc_)
		, ext(ext_)
	{
	}

	void set(const wchar_t* desc_, const wchar_t* ext_) {
		desc = desc_;
		ext = ext_;
	}
	std::wstring desc;
	std::wstring ext;
};

std::wstring MyOpenFile(HWND hWnd, const wchar_t* title, const wchar_t* defaultFilename, const std::vector<MyOpenFileFilter>& filter) {
	wchar_t buf[MAX_PATH + 1];
	wcsncpy(buf, defaultFilename, MAX_PATH);
	buf[MAX_PATH] = 0;

	OPENFILENAME s;
	my_bzero(s);
	s.lStructSize = sizeof(s);
	s.hwndOwner = hWnd;
	s.hInstance = g_hInstance;

	std::wstring fs;
	for (auto & f : filter) {
		fs.append(f.desc);
		fs.push_back('\0');
		fs.append(f.ext);
		fs.push_back('\0');
	}
	fs.push_back('\0');

					// Desc \0  file ext \0
//	s.lpstrFilter = L"Text Files (*.txt)\0*.txt\0"
//						"Log Files (*.log)\0*.log\0"
//						"All Files (*.*)\0*.*\0"
//						"\0";
	s.lpstrFilter = fs.c_str();

	s.lpstrTitle = L"My Open File";
	s.lpstrFile = buf;
	s.nMaxFile = MAX_PATH;
	s.Flags = OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&s))
		return L"";

	return std::wstring(buf);
}


BOOL CALLBACK MyDialogFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG: {
			auto editBox = GetDlgItem(hWnd, IDC_EDIT1);
			if (editBox) {
				SetWindowText(editBox, L"Hello !!");
			}
			return true;
		}

		case WM_COMMAND: {
			auto cmdId = LOWORD(wParam);
			printf("dialog cmdId = %d\n", cmdId);
			switch (cmdId) {
				case IDC_BUTTON1: {
					auto editBox = GetDlgItem(hWnd, IDC_EDIT1);
					if (editBox) {
						wchar_t buf[512];
						GetWindowText(editBox, buf, 512);
						wprintf(L"input = %s\n", buf);
					}
				}break;

				case IDOK: {
					EndDialog(hWnd, 100);
				}break;
				case IDCANCEL: {
					EndDialog(hWnd, 105);
				}break;

			}
			return true;
		}
	}
	return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {  
    switch (message)  
    {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}break;

		case WM_COMMAND: {
			auto cmdId = LOWORD(wParam);
			switch (cmdId) {
				case ID_FILE_EXIT: {
					SendMessage(hWnd, WM_CLOSE, 0, 0);
				}break;
				case ID_FILE_OPEN: {
					std::vector<MyOpenFileFilter> filter;
					filter.emplace_back(L"Text Files (*.txt)", L"*.txt");
					filter.emplace_back(L"Log Files (*.log)",  L"*.log");
					filter.emplace_back(L"All Files (*.*)",    L"*.*");

					MyOpenFile(hWnd, L"My Open", L"Default.txt", filter);
				}break;
			}
		}break;

		case WM_RBUTTONUP: {
			//MessageBox(hWnd, L"Hello !", L"My Title", MB_OKCANCEL);

			//Modal dialog
			auto ret = DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDialogFunc);
			printf("Dialog return %d\n", ret);

		}break;
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
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