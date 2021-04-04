#include "MyOpenGLWindow.h"


class MyDemoWindow : public MyOpenGLWindow {
public:
	MyDemoWindow() {
	}

	virtual void onDestroy() override {
		PostQuitMessage(0);
	}

	virtual void onMouseEvent(MouseEvent& ev) override {
		switch (ev.type) {
			case MouseEventType::MouseMove: {
				if (ev.rightButton) {
					float dx = ev.x - m_mouseLastPosX;
					float dy = ev.y - m_mouseLastPosY;

					float s = 0.25f;

					m_cameraY += dx * s;
					m_cameraX += dy * s;
				}
				if (ev.middleButton) {
					float dx = ev.x - m_mouseLastPosX;
					m_fovy += dx * 0.025f;
				}
			}break;
		}

		m_mouseLastPosX = ev.x;
		m_mouseLastPosY = ev.y;
	}

	void example1(float uptime) {
		float angle = uptime * 10;

		float aspect = static_cast<float>(canvasWidth()) / canvasHeight();

		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(m_fovy, aspect, 0.01, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glTranslatef(0, 0, -4.0f);
		glRotatef(m_cameraX, 1,0,0);
		glRotatef(m_cameraY, 0,1,0);

		// draw grid
		glColor4f(1,1,1,1);
		glLineWidth(2);
		glBegin(GL_LINES);
			for (float x = -10; x <= 10; x++) {
				glVertex3f( x, 0,-10);
				glVertex3f( x, 0, 10);
			}

			for (float z = -10; z <= 10; z++) {
				glVertex3f(-10, 0, z);
				glVertex3f( 10, 0, z);
			}
		glEnd();

//------------------------------------------
//		  0--------1
//		 /|       /|
//		3--------2 |
//      | |      | |
//		| 4------|-5
//      |/       |/
//      7--------6

		float d = 0.5f;
		float v[8][3] = {
			{-d,-d,-d},
			{ d,-d,-d},
			{ d, d,-d},
			{-d, d,-d},

			{-d,-d, d},
			{ d,-d, d},
			{ d, d, d},
			{-d, d, d},
		};

		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
			glVertex3fv(v[0]); glVertex3fv(v[1]);
			glVertex3fv(v[1]); glVertex3fv(v[2]);
			glVertex3fv(v[2]); glVertex3fv(v[3]);
			glVertex3fv(v[3]); glVertex3fv(v[0]);

			glVertex3fv(v[4]); glVertex3fv(v[5]);
			glVertex3fv(v[5]); glVertex3fv(v[6]);
			glVertex3fv(v[6]); glVertex3fv(v[7]);
			glVertex3fv(v[7]); glVertex3fv(v[4]);

			glVertex3fv(v[0]); glVertex3fv(v[4]);
			glVertex3fv(v[1]); glVertex3fv(v[5]);
			glVertex3fv(v[2]); glVertex3fv(v[6]);
			glVertex3fv(v[3]); glVertex3fv(v[7]);
		glEnd();
	}

	virtual void onPaint() override {
		auto uptime = static_cast<float>(m_uptime.get());

		glViewport(0, 0, canvasWidth(), canvasHeight());

		glClearColor(0.0f, 0.2f, 0.2f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		example1(uptime);

		swapBuffers();
	}

	MyHiResTimer m_uptime;
	float m_cameraX = 30;
	float m_cameraY = 30;
	float m_fovy = 90;

	float m_mouseLastPosX;
	float m_mouseLastPosY;

};

int main() {
	MyDemoWindow w;
	w.create();

    MSG msg;

#if 0 // using PeekMessage
	for(;;) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);  
			DispatchMessage(&msg);  
		}else{
			w.onPaint();
		}
	}
#else
	while(GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);  
		DispatchMessage(&msg);  
	}	
#endif

	return msg.wParam;
}