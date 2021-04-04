#include "my_common.h"
#include <vector>
#include <cassert>
#include "resource.h"

const wchar_t* szWindowClass = L"MyWindow";

struct MyColor {
	uint8_t b = 0;
	uint8_t g = 0;
	uint8_t r = 0;
	uint8_t a = 0;

	void set(uint8_t r_, uint8_t g_, uint8_t b_) {
		r = r_;
		g = g_;
		b = b_;
		a = 0;
	}
};

class MyBitmap : public MyNonCopyable {
public:

	~MyBitmap() {
		destroy();
	}

	void create(int width, int height) {
		destroy();

		std::vector<MyColor> pixels;
		pixels.resize(width * height);

		for (int y = 0; y < height; y++) {
			auto* p = &pixels[y * width];

			for (int x = 0; x < width; x++) {
				p->set(x, y, 0);
				p++;
			}
		}

		m_bmp = CreateBitmap(width, height, 1, 32, pixels.data());
		if (!m_bmp) {
			assert(false);
			return;
		}
		m_width = width;
		m_height = height;
	}

	void loadFromResource(int resourceId) {
		destroy();
		m_bmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(resourceId));
		if (!m_bmp) 
			throw 1;

		BITMAP info;
		GetObject(m_bmp, sizeof(info), &info);

		m_width = info.bmWidth;
		m_height = info.bmHeight;
	}

	void loadFromFile(const wchar_t* filename) {
		destroy();
		m_bmp = (HBITMAP)LoadImage(GetModuleHandle(nullptr), filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (!m_bmp) {
			auto dw = GetLastError();
			throw 1;
		}

		BITMAP info;
		GetObject(m_bmp, sizeof(info), &info);

		m_width = info.bmWidth;
		m_height = info.bmHeight;
	}

	void destroy() {
		if (m_bmp) {
			DeleteObject(m_bmp);
			m_bmp = nullptr;
			m_width = 0;
			m_height = 0;
		}
	}

	void draw(HDC hdc, int x, int y) {
		if (!hdc || !m_bmp)
			return;

		auto src = CreateCompatibleDC(hdc);
		SelectObject(src, m_bmp);

		BitBlt(hdc, x, y, m_width, m_height, src, 0, 0, SRCCOPY);

		DeleteDC(src);
	}

private:
	HBITMAP m_bmp = nullptr;
	int m_width = 0;
	int m_height = 0;
};

class MySprite {
public:
	static const int kFrames = 8;

	void create() {
		for (int i = 0; i < kFrames; i++) {
			m_bmp[i].loadFromResource(IDB_BITMAP1 + i);
		}
	}

	void draw(HDC hdc, int x, int y) {
		m_bmp[m_currectFrame].draw(hdc, x, y);
	}

	void nextFrame() {
		m_currectFrame = (m_currectFrame + 1) % kFrames;
	}

private:
	int m_currectFrame = 0;
	MyBitmap m_bmp[kFrames];
};

MyBitmap bmp;
MySprite sprite;

class MyBackBuffer {
public:
	~MyBackBuffer() {
		destroy();
	}

	void create(HDC hdc, int width, int height) {
		destroy();

		m_hdc = CreateCompatibleDC(hdc);
		m_bmp = CreateCompatibleBitmap(m_hdc, width, height);
		SelectObject(m_hdc, m_bmp);

		RECT rc;
		rc.left = 0;
		rc.right = width;
		rc.top = 0;
		rc.bottom = height;

		auto brush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		FillRect(m_hdc, &rc, brush);

		m_width = width;
		m_height = height;
	}

	void destroy() {
		if (m_bmp) {
			DeleteObject(m_bmp);
			m_bmp = nullptr;
		}

		if (m_hdc) {
			DeleteDC(m_hdc);
			m_hdc = nullptr;
		}

		m_width = 0;
		m_height = 0;
	}

	HDC getDC() { return m_hdc; }

	void draw(HDC hdc, int x, int y) {
		if (!hdc) return;
		if (!m_bmp) return;
		if (!m_hdc) return;

		BitBlt(hdc, x, y, m_width, m_height, m_hdc, 0, 0, SRCCOPY);
	}

private:
	HBITMAP m_bmp = nullptr;
	HDC m_hdc = nullptr;
	int m_width = 0;
	int m_height = 0;
};

MyBackBuffer backBuffer;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)  
    {
		case WM_CREATE: {
			//bmp.create(256, 256);
			//bmp.loadFromResource();
			bmp.loadFromFile(L"cat.bmp");
			sprite.create();

			RECT rc;
			GetClientRect(hWnd, &rc);

			auto hdc = GetDC(hWnd);
			backBuffer.create(hdc, rc.right - rc.left, rc.bottom - rc.left);
			ReleaseDC(hWnd, hdc);

			SetTimer(hWnd, 0, 100, nullptr);
		}break;

		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		case WM_LBUTTONDOWN: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);

			bmp.draw(backBuffer.getDC(), x, y);
			InvalidateRect(hWnd, nullptr, false);
		}break;

		case WM_TIMER: {
			sprite.nextFrame();
			InvalidateRect(hWnd, nullptr, false);
		}break;

		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			RECT rc;
			GetClientRect(hWnd, &rc);

			backBuffer.draw(ps.hdc, 0, 0);

			//bmp.draw(ps.hdc, 30, 30);
			sprite.draw(ps.hdc, 30, 30);

			EndPaint(hWnd, &ps);
		}break;
    } // switch    
	return DefWindowProc(hWnd, message, wParam, lParam);  
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
    wcex.lpszMenuName   = NULL;  
    wcex.lpszClassName  = szWindowClass;  
    wcex.hIconSm        = nullptr;

    RegisterClassEx(&wcex);
	//------------------------
	HWND hwnd = CreateWindowEx(0,
							szWindowClass, 
							L"MyWindow Title",
							WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
							CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
							nullptr,
							nullptr,
							hInstance,
							(LPVOID)0x123);

	CreateWindowEx(0, L"Button", L"OK", WS_CHILD | WS_VISIBLE, 30, 80, 300, 50, hwnd, nullptr, hInstance, nullptr);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

    MSG msg;  
    while (GetMessage(&msg, NULL, 0, 0)) {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }

	return msg.wParam;
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}