#include "MyOpenGLWindow.h"
#include "MyMesh.h"
#include "MyTexture2D.h"

void checkGLError() {
	auto e = glGetError();
	if (e != GL_NO_ERROR) {
		printf("glGetError %d\n", e);
	}	
}

class MyDemoWindow : public MyOpenGLWindow {
public:
	virtual void onGLinit() override {
		m_mesh.loadObjFile("models/test.obj");
		m_mesh.doubleSided = true;
		m_mesh.wireframe = true;
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
			case MouseEventType::MouseWheel: {
				m_cameraDistance += ev.zDelta * 0.001f;
				if (m_cameraDistance < 0.01f)
					m_cameraDistance = 0.01f;
			}break;
		}

		m_mouseLastPosX = ev.x;
		m_mouseLastPosY = ev.y;
	}

	void drawGrid() {
		glColor4f(0.5f, 0.5f, 0.5f, 1);
		glLineWidth(1);
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
		glColor4f(1,1,1,1);
	}

	void drawOriginAxis() {
		glLineWidth(2);
		glBegin(GL_LINES);
			glColor4f(1,0,0,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
			glColor4f(0,1,0,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
			glColor4f(0,0,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);
		glEnd();
		glColor4f(1,1,1,1);
		glLineWidth(1);
	}

	void initCamera() {
		float aspect = static_cast<float>(canvasWidth()) / canvasHeight();
		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(m_fovy, aspect, 0.01, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
		
		glTranslatef(0, 0, -m_cameraDistance);
		glRotatef(m_cameraX, 1,0,0);
		glRotatef(m_cameraY, 0,1,0);
	}

	void example1(float uptime) {
		m_mesh.draw();
	}

	virtual void onPaint() override {
		auto uptime = static_cast<float>(m_uptime.get());

		glViewport(0, 0, canvasWidth(), canvasHeight());

		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);

		glColor4f(1,1,1,1);

		glClearColor(0.0f, 0.2f, 0.2f, 0);
		glClearDepth(1);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glLoadIdentity();

		initCamera();
		drawGrid();
		drawOriginAxis();

		example1(uptime);

		swapBuffers();
	}

	MyHiResTimer m_uptime;
	float m_cameraX = 30;
	float m_cameraY = 30;
	float m_cameraDistance = 2.0f;

	float m_fovy = 90;

	float m_mouseLastPosX;
	float m_mouseLastPosY;

	MyMesh m_mesh;
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