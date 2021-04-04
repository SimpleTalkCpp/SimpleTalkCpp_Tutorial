#include "MyOpenGLWindow.h"

class MyDemoWindow : public MyOpenGLWindow {
public:
	MyDemoWindow() {
	}

	virtual void onDestroy() override {
		PostQuitMessage(0);
	}

	virtual void onPaint() override {
		printf("onPaint\n");
		glViewport(0, 0, canvasWidth(), canvasHeight());

		glClearColor(0.2f, 0.5f, 0.6f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float d = 0.5f;
		glBegin(GL_QUADS);
			glColor4f(1,0,0,1);	glVertex3f(-d,-d, 0);
			glColor4f(0,0,1,1);	glVertex3f( d,-d, 0);
			glColor4f(0,1,0,1);	glVertex3f( d, d, 0);
			glColor4f(1,0,1,1);	glVertex3f(-d, d, 0);
		glEnd();

		swapBuffers();
	}
};

int main() {
	MyDemoWindow w;
	w.create();

    MSG msg;

	while(GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);  
		DispatchMessage(&msg);  
	}	
	return msg.wParam;
}