#include "MyOpenGLWindow.h"


class MyDemoWindow : public MyOpenGLWindow {
public:
	MyDemoWindow() {
	}

	virtual void onDestroy() override {
		PostQuitMessage(0);
	}

	float degToRad(float deg) {
		return deg / 180.0f * 3.141592f;
	}

	void example1(float uptime) {
		float x = 0.5f;
		float y = 0;

		float angle = uptime * 90;

		float angleInRad = my_rad(angle);
		float c = cos(angleInRad);
		float s = sin(angleInRad);

		float nx = x * c + y * -s;
		float ny = x * s + y *  c;

		glPointSize(12);
		glBegin(GL_POINTS);
			glColor4f(1,0,0,1);
			glVertex2f(x,y);

			glVertex2f(nx,ny);
		glEnd();
	}

	void example2(float uptime) {
		float x = 0.5f;
		float y = 0;

		float angle = uptime * 90.0f;

//		glPointSize(12);
//		glBegin(GL_POINTS);
//			glVertex2f(x,y);
//		glEnd();

//		float scale = uptime * 0.1f;
//		glScalef(scale, scale, scale);

		//glPushMatrix();
		{
			glRotatef(angle, 0,0,1);
			glTranslatef(0.5f, 0, 0);

			glColor4f(1,0,0,1);
			float d = 0.1f;
			glBegin(GL_QUADS);
				glVertex2f(-d,-d);
				glVertex2f( d,-d);
				glVertex2f( d, d);
				glVertex2f(-d, d);
			glEnd();
		}
		//glPopMatrix();

		{
			glRotatef(angle, 0,0,1);
			glTranslatef(-0.2f, 0, 0);

			glColor4f(0,0,1,1);
			float d = 0.05f;
			glBegin(GL_QUADS);
				glVertex2f(-d,-d);
				glVertex2f( d,-d);
				glVertex2f( d, d);
				glVertex2f(-d, d);
			glEnd();
		}
	}

	virtual void onPaint() override {
		auto uptime = static_cast<float>(m_uptime.get());

		glViewport(0, 0, canvasWidth(), canvasHeight());

		glClearColor(0.0f, 0.2f, 0.2f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		glColor4f(1,1,1,1);
		glLineWidth(2);
		glBegin(GL_LINES);
			glVertex2f(-1,0);
			glVertex2f( 1,0);

			glVertex2f(0,-1);
			glVertex2f(0, 1);
		glEnd();

		//example1(uptime);
		example2(uptime);

		swapBuffers();
	}

	MyHiResTimer m_uptime;
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