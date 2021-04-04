#pragma once

#include "my_common.h"

class MyOpenGLWindow : public MyNonCopyable {
public:

	enum class MouseEventType {
		Unknown,
		LButtonDown,
		LButtonUp,
		MButtonDown,
		MButtonUp,
		RButtonDown,
		RButtonUp,
		MouseMove,
	};

	class MouseEvent {
	public:
		using Type = MouseEventType;

		MouseEvent() 
			: type(Type::Unknown)
			, leftButton(false)
			, middleButton(false)
			, rightButton(false)
			, ctrl(false)
		{}

		Type type;
		float x = 0;
		float y = 0;
		bool leftButton : 1;
		bool middleButton : 1;
		bool rightButton : 1;
		bool ctrl: 1;
	};

	~MyOpenGLWindow();

	void create();
	void destroy();

	virtual bool onShouldClose() { return true; }
	virtual void onDestroy() {}
	virtual void onPaint() {}
	virtual void onMouseEvent(MouseEvent& ev) {}

	static const wchar_t* wndClassName() { return L"MyOpenGLWindowClass"; }

	void makeCurrent();
	void swapBuffers();

	int canvasWidth () const { return m_canvasWidth;  }
	int canvasHeight() const { return m_canvasHeight; }
	

private:
	static void _onMouseEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static MyOpenGLWindow* getThis(HWND hwnd);

	HWND	m_hwnd = nullptr;
	HDC		m_dc = nullptr;
	HGLRC	m_rc = nullptr;
	int m_canvasWidth = 0;
	int m_canvasHeight = 0;
};