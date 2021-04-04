#include "precompiledHeader.h"
#include "MyOpenGLWindow.h"
#include "MyMesh.h"
#include "MyRayTracer.h"

class MyDemoWindow : public MyOpenGLWindow {
public:
	virtual void onGLinit() override {
		m_mesh.loadObjFile("../models/test.obj");
//		m_mesh.loadObjFile("../models/sphere_smooth.obj");
		m_mesh.wireframe = true;

		m_debugTriangle.v0.set(1,1,1);
		m_debugTriangle.v1.set(2,2,1);
		m_debugTriangle.v2.set(1,2,0);
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
				rayTracing(ev.x, ev.y);
			}break;
		}

		m_mouseLastPosX = ev.x;
		m_mouseLastPosY = ev.y;
	}

	void rayTracing(float x, float y) {
		MyMatrix4f projMatrix;
		MyMatrix4f modelview;

		glGetFloatv(GL_PROJECTION_MATRIX, projMatrix.data);
		glGetFloatv(GL_MODELVIEW_MATRIX,  modelview.data);

		MyVec2f screenSize(static_cast<float>(canvasWidth()), static_cast<float>(canvasHeight()));
		m_rayTracer.init(screenSize, projMatrix, modelview);
		auto ray = m_rayTracer.getRay(x, y);

		m_debugRay = ray;

		m_result.reset();

		MyPlane plane(MyVec3f(0,1,0), 0.5f);
		ray.raycast(m_result, plane, m_result.distance);

//		MySphere sphere(MyVec3f(0,0,0), 1.0f);
//		ray.raycast(m_result, sphere, m_result.distance);

		ray.raycast(m_result, m_debugTriangle, m_result.distance);
		ray.raycast(m_result, m_mesh, m_result.distance);
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
		glPointSize(10);
		drawDebugRay();

		m_mesh.draw();
		drawDebugTriangle();
		drawRaycastResult();
	}

	void drawRaycastResult() {
		if (!m_result.hasResult)
			return;
		glColor4f(1,0,0,1);
		glBegin(GL_POINTS);
			glVertex3fv(m_result.point.data);
		glEnd();
		glBegin(GL_LINES);
			glVertex3fv(m_result.point.data);
			glVertex3fv((m_result.point + m_result.normal).data);
		glEnd();
	}

	void drawDebugRay() {
		glColor4f(1,0,1,1);
		glBegin(GL_POINTS);
			glVertex3fv(m_debugRay.origin.data);
		glEnd();
		glBegin(GL_LINES);
			glVertex3fv(m_debugRay.origin.data);
			auto v = m_debugRay.origin + m_debugRay.direction;
			glVertex3fv(v.data);
		glEnd();
	}

	void drawDebugTriangle() {
		glColor4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
			glVertex3fv(m_debugTriangle.v0.data);
			glVertex3fv(m_debugTriangle.v1.data);
			glVertex3fv(m_debugTriangle.v2.data);
		glEnd();

		glBegin(GL_LINES);
			auto c = m_debugTriangle.calcCenter();
			auto nl = c + m_debugTriangle.calcNormal();
			glVertex3fv(c.data);
			glVertex3fv(nl.data);
		glEnd();
	}

	virtual void onPaint() override {
		auto uptime = static_cast<float>(m_uptime.get());

		glViewport(0, 0, canvasWidth(), canvasHeight());

		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor4f(1,1,1,1);

		glClearColor(0.0f, 0.2f, 0.2f, 0);
		glClearDepth(1);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		//---------------
		initCamera();
		drawGrid();
		drawOriginAxis();

		example1(uptime);

		swapBuffers();
	}

	MyHiResTimer m_uptime;
	float m_cameraX = 30;
	float m_cameraY = 30;
	float m_cameraDistance = 6.0f;

	float m_fovy = 60;

	float m_mouseLastPosX;
	float m_mouseLastPosY;

	MyMesh m_mesh;

	MyRayTracer m_rayTracer;

	MyRay3f::HitResult m_result;
	MyRay3f m_debugRay;
	MyTriangle m_debugTriangle;
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