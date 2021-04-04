#pragma once

#include "my_common.h"

class MyOpenGLWindow : public MyNonCopyable {
public:

	~MyOpenGLWindow();

	void create();
	void destroy();

	virtual bool onShouldClose() { return true; }
	virtual void onDestroy() {}
	virtual void onPaint() {}

	static const wchar_t* wndClassName() { return L"MyOpenGLWindowClass"; }

	void makeCurrent();
	void swapBuffers();

	int canvasWidth () const { return m_canvasWidth;  }
	int canvasHeight() const { return m_canvasHeight; }
	

private:

	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static MyOpenGLWindow* getThis(HWND hwnd);

	HWND	m_hwnd = nullptr;
	HDC		m_dc = nullptr;
	HGLRC	m_rc = nullptr;
	int m_canvasWidth = 0;
	int m_canvasHeight = 0;
};