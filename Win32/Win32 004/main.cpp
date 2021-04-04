#include "my_common.h"
#include <Windows.h>
#include <vector>

const wchar_t* szWindowClass = L"MyWindow";

HWND g_hwnd;

class Cell {
public:
	static const int kSize = 22;

	bool hasMine = false;
	bool opened = false;

	int x = 0;
	int y = 0;

	int mineCount = 0;

	void init(int x_, int y_) {
		x = x_;
		y = y_;

		hasMine = false;
		opened = false;
		mineCount = 0;
	}

	void onPaint(PAINTSTRUCT &ps) {
		int sx = x * kSize;
		int sy = y * kSize;

		if (opened) {
			SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH));
		}else{
			SelectObject(ps.hdc, GetStockObject(GRAY_BRUSH));
		}

		Rectangle(ps.hdc, sx, sy, sx + kSize, sy + kSize);

		if (!opened)
			return;

		if (hasMine) {
			TextOut(ps.hdc, sx + 4, sy + 3, L"@", 1);
		} else {

			if (mineCount > 0) {
				wchar_t buf[64];
				wsprintf(buf, L"%d", mineCount);
				TextOut(ps.hdc, sx + 4, sy + 3, buf, wcslen(buf));
			}
		}
	}
};

class Grid {
public:
	
	void create(int w, int h) {
		m_width = w;
		m_height = h;
		int n = w * h;
		m_cells.resize(n);

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				auto* center = getCell(x, y);
				center->init(x, y);
			}
		}

		for (int i = 0; i < w; i++) {
			int index = rand() % n;
			m_cells[index].hasMine = true;
		}

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				auto* center = getCell(x, y);

				_updateMineCount(center, getCell(x-1, y-1));
				_updateMineCount(center, getCell(x,   y-1));
				_updateMineCount(center, getCell(x+1, y-1));

				_updateMineCount(center, getCell(x-1, y));
				_updateMineCount(center, getCell(x+1, y));

				_updateMineCount(center, getCell(x-1, y+1));
				_updateMineCount(center, getCell(x,   y+1));
				_updateMineCount(center, getCell(x+1, y+1));
			}
		}
	}

	void _updateMineCount(Cell* center, Cell* another) {
		if (!another)
			return;
		if (another->hasMine) {
			center->mineCount++;
		}
	}

	Cell* getCell(int x, int y) {
		if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			return nullptr;

		return &m_cells[y * m_width + x];
	}

	void onClick(int x, int y) {
		auto* c = getCell(x, y);
		if (!c)
			return;

		if (c->hasMine) {
			MessageBox(g_hwnd, L"Game Over", L"", MB_OK);
			create(m_width, m_height);
			return;
		}

		openCell(c);
	}

	void openCell(Cell* c) {
		if (!c)
			return;

		if (c->opened)
			return;

		c->opened = true;

		if (c->mineCount > 0)
			return;

		int x = c->x;
		int y = c->y;
		openCell(getCell(x-1, y-1));
		openCell(getCell(x,   y-1));
		openCell(getCell(x+1, y-1));

		openCell(getCell(x-1, y));
		openCell(getCell(x+1, y));

		openCell(getCell(x-1, y+1));
		openCell(getCell(x,   y+1));
		openCell(getCell(x+1, y+1));
	}

	void onPaint(PAINTSTRUCT& ps) {
		for (auto& c : m_cells) {
			c.onPaint(ps);
		}
	}

private:
	int m_width = 0;
	int m_height = 0;

	std::vector<Cell> m_cells;
};

Grid g_grid;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			g_grid.onPaint(ps);
			EndPaint(hWnd, &ps);
		}break;

		case WM_DESTROY:  
			PostQuitMessage(0);
			return 0;

		case WM_LBUTTONDOWN: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);

			int cx = x / Cell::kSize;
			int cy = y / Cell::kSize;

			g_grid.onClick(cx, cy);

			InvalidateRect(hWnd, nullptr, false);
		}break;

    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_grid.create(32, 16);

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
							CW_USEDEFAULT, CW_USEDEFAULT, 840, 480,
							nullptr,
							nullptr,
							hInstance,
							0);

	g_hwnd = hwnd;

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