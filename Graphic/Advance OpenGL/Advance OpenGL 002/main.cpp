#include "precompiledHeader.h"
#include "MyOpenGLWindow.h"
#include "MyMesh.h"
#include "MyTexture2D.h"
#include "MyShader.h"

class MyDemoWindow : public MyOpenGLWindow {
public:
	virtual void onGLinit() override {
		m_gridMesh.createGrid(20, 20, 1, MyColor4b(128, 128, 128, 255));
		m_orignMesh.createOrigin();

		m_mesh.loadObjFile("../models/test2.obj");
		//m_mesh.wireframe = true;

		m_simpleColorShader.loadFile(   "../shaders/simpleColor");
		m_simpleLightingShader.loadFile("../shaders/simpleLighting");
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
				m_cameraDistance += ev.zDelta * 0.01f;
				if (m_cameraDistance < 0.01f)
					m_cameraDistance = 0.01f;
			}break;

			case MouseEventType::LButtonDown: {
				m_simpleColorShader.reload();
				m_simpleLightingShader.reload();
			}break;
		}

		m_mouseLastPosX = ev.x;
		m_mouseLastPosY = ev.y;
	}

	void drawGrid() {
//		glColor4f(0.5f, 0.5f, 0.5f, 1);
//		glLineWidth(1);
//		glBegin(GL_LINES);
//			for (float x = -10; x <= 10; x++) {
//				glVertex3f( x, 0,-10);
//				glVertex3f( x, 0, 10);
//			}
//
//			for (float z = -10; z <= 10; z++) {
//				glVertex3f(-10, 0, z);
//				glVertex3f( 10, 0, z);
//			}
//		glEnd();
//		glColor4f(1,1,1,1);
	}

	void drawOriginAxis() {
//		glLineWidth(2);
//		glBegin(GL_LINES);
//			glColor4f(1,0,0,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
//			glColor4f(0,1,0,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
//			glColor4f(0,0,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);
//		glEnd();
//		glColor4f(1,1,1,1);
//		glLineWidth(1);
	}

	void initCamera() {
		float aspect = static_cast<float>(canvasWidth()) / canvasHeight();

//		glMatrixMode(GL_PROJECTION);
//			glLoadIdentity();
//			gluPerspective(m_fovy, aspect, 0.01, 1000.0);
		m_matProj.setPerspective(my_deg_to_rad(m_fovy), aspect, 0.01f, 1000.0f);

//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		
//		glTranslatef(0, 0, -m_cameraDistance);
//		glRotatef(m_cameraX, 1,0,0);
//		glRotatef(m_cameraY, 0,1,0);

		m_matModelview.setTranslate(0, 0, -m_cameraDistance);
		m_matModelview.rotateX(my_deg_to_rad(m_cameraX));
		m_matModelview.rotateY(my_deg_to_rad(m_cameraY));
	}

	void example1(float uptime) {
//		drawGrid();
//		drawOriginAxis();
		//m_mesh.draw();

		auto matMVP = m_matProj * m_matModelview;

		{
			m_simpleColorShader.bind();
			m_simpleColorShader.setUniform("matMVP", matMVP);
			//m_simpleColorShader.dumpActiveAttrib();

			m_simpleColorShader.draw(m_gridMesh);
			m_simpleColorShader.draw(m_orignMesh);

			m_simpleColorShader.unbind();
		}

		{
			m_simpleLightingShader.bind();
			m_simpleLightingShader.setUniform("uptime", uptime);
			m_simpleLightingShader.setUniform("matMVP", matMVP);
			m_simpleLightingShader.setUniform("lightPos", MyVec3f(1, 2, 2));

			m_simpleLightingShader.draw(m_mesh);

			m_simpleLightingShader.unbind();
		}
	}

	virtual void onPaint() override {
		auto uptime = static_cast<float>(m_uptime.get());

		glViewport(0, 0, canvasWidth(), canvasHeight());

		glDisable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);

		glColor4f(1,1,1,1);

		glClearColor(0.0f, 0.2f, 0.2f, 0);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		//---------------
		initCamera();

		example1(uptime);

		swapBuffers();
	}

	MyHiResTimer m_uptime;
	float m_cameraX = 30;
	float m_cameraY = 30;
	float m_cameraDistance = 4.0f;

	float m_fovy = 60;

	float m_mouseLastPosX;
	float m_mouseLastPosY;

	MyMatrix4f m_matProj;
	MyMatrix4f m_matModelview;

	MyMesh m_mesh;
	MyMesh m_gridMesh;
	MyMesh m_orignMesh;

	MyShader m_simpleColorShader;
	MyShader m_simpleLightingShader;
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