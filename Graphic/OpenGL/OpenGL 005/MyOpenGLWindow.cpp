#include "MyOpenGLWindow.h"

MyOpenGLWindow::~MyOpenGLWindow() {
	destroy();
}

void MyOpenGLWindow::create() {
	auto hInstance = GetModuleHandle(nullptr);
	destroy();

	//-- Check did we register window class yet
	WNDCLASSEX wc;
	if (!GetClassInfoEx(hInstance, wndClassName(), &wc))
	{
		//-- register window class
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC; //!! <------- CS_OWNDC is required for OpenGL Window
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = nullptr; //!! <------- 
		wc.lpszClassName = wndClassName();

		if (!RegisterClassEx(&wc))
			throw MyError("RegisterClassEx");
	}

	//---------------
	m_hwnd = CreateWindowEx(0,
		wndClassName(),
		L"My OpenGL Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		nullptr, nullptr, hInstance, 
		this); //!! <----- pass this to WM_CREATE

	if (!m_hwnd)
		throw MyError("CreateWindow");

	m_dc = GetDC(m_hwnd);
	if (!m_dc)
		throw MyError("GetDC");

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8; //! <--- Enable stencil buffer
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(m_dc, &pfd);
	if (pf == 0)
		throw MyError("ChoosePixelFormat");

	if (!SetPixelFormat(m_dc, pf, &pfd))
		throw MyError("SetPixelFormat");

	m_rc = wglCreateContext(m_dc);
	if (!m_rc)
		throw MyError("wglCreateContext");

	makeCurrent();

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

void MyOpenGLWindow::destroy() {
	if (m_rc)   { wglDeleteContext(m_rc);	m_rc = nullptr; }
	if (m_dc)   { ReleaseDC(m_hwnd, m_dc);	m_dc = nullptr; }
	if (m_hwnd) { DestroyWindow(m_hwnd);	m_hwnd = nullptr; }
}

void MyOpenGLWindow::makeCurrent() {
	if (m_rc)
		wglMakeCurrent(m_dc, m_rc);
}

void MyOpenGLWindow::swapBuffers() {
	if (m_dc)
		::SwapBuffers(m_dc);
}

void MyOpenGLWindow::_onMouseEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent ev;
	auto x = GET_X_LPARAM(lParam);
	auto y = GET_Y_LPARAM(lParam);

	ev.x = static_cast<float>(x);
	ev.y = static_cast<float>(y);

	ev.leftButton   = (wParam & MK_LBUTTON) != 0;
	ev.middleButton = (wParam & MK_MBUTTON) != 0;
	ev.rightButton  = (wParam & MK_RBUTTON) != 0;
	ev.ctrl         = (wParam & MK_CONTROL) != 0;

	switch (msg) {
		case WM_LBUTTONDOWN: ev.type = MouseEventType::LButtonDown; break;
		case WM_LBUTTONUP:   ev.type = MouseEventType::LButtonUp;   break;
		case WM_MBUTTONDOWN: ev.type = MouseEventType::MButtonDown; break;
		case WM_MBUTTONUP:   ev.type = MouseEventType::MButtonUp;   break;
		case WM_RBUTTONDOWN: ev.type = MouseEventType::RButtonDown; break;
		case WM_RBUTTONUP:   ev.type = MouseEventType::RButtonUp;   break;
		case WM_MOUSEMOVE:   ev.type = MouseEventType::MouseMove;   break;
		default:
			return;
	}

	ReleaseCapture();
	if (ev.leftButton | ev.middleButton | ev.rightButton) {
		SetCapture(hwnd);
	}
	getThis(hwnd)->onMouseEvent(ev);
}

LRESULT WINAPI MyOpenGLWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
		case WM_CREATE:	{
			auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			auto* thisObj = reinterpret_cast<MyOpenGLWindow*>(cs->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)thisObj);
		}break;

		case WM_PAINT: {
			#if 0
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				getThis(hwnd)->onPaint();
				EndPaint(hwnd, &ps);
			#else
				getThis(hwnd)->onPaint();
				return 0;
			#endif
		}break;

		case WM_DESTROY: {
			getThis(hwnd)->onDestroy();
		}break;

		case WM_SIZE: {
			auto* thisObj = getThis(hwnd);
			auto w = GET_X_LPARAM(lParam);
			auto h = GET_Y_LPARAM(lParam);
			thisObj->m_canvasWidth  = w;
			thisObj->m_canvasHeight = h;
		}break;

		case WM_CLOSE: {
			if (!getThis(hwnd)->onShouldClose())
				return 1;
			// DefWindowProc will call DestroyWindow
		}break;

		case WM_LBUTTONDOWN:	_onMouseEvent(hwnd, msg, wParam, lParam); break;
		case WM_LBUTTONUP:		_onMouseEvent(hwnd, msg, wParam, lParam); break;
		case WM_MBUTTONDOWN:	_onMouseEvent(hwnd, msg, wParam, lParam); break;
		case WM_MBUTTONUP:		_onMouseEvent(hwnd, msg, wParam, lParam); break;
		case WM_RBUTTONDOWN:	_onMouseEvent(hwnd, msg, wParam, lParam); break;
		case WM_RBUTTONUP:		_onMouseEvent(hwnd, msg, wParam, lParam); break;
		case WM_MOUSEMOVE:		_onMouseEvent(hwnd, msg, wParam, lParam); break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

MyOpenGLWindow* MyOpenGLWindow::getThis(HWND hwnd) {
	auto data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	auto* thisObj = reinterpret_cast<MyOpenGLWindow*>(data);
	if (hwnd != thisObj->m_hwnd)
		throw MyError("getThis()");
	return thisObj;
}
