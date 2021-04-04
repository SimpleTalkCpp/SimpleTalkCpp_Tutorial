#include "precompiledHeader.h"
#include "MyOpenGLWindow.h"
#include "MyMesh.h"
#include "MyRayTracer.h"
#include "MyTexture2D.h"

class MyDemoWindow : public MyOpenGLWindow {
public:
	virtual void onGLinit() override {
		m_mesh.loadObjFile("../models/test.obj");
//		m_mesh.loadObjFile("../models/test2.obj");
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
				m_cameraDistance += ev.zDelta * 0.01f;
				if (m_cameraDistance < 0.01f)
					m_cameraDistance = 0.01f;
				m_tex.destroy();
			}break;

			case MouseEventType::LButtonDown: {
				rayTracing(ev.x, ev.y);
			}break;

			case MouseEventType::RButtonDown: {
				m_tex.destroy();
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

		int cw = canvasWidth();
		int ch = canvasHeight();

		MyVec2f screenSize(static_cast<float>(cw), static_cast<float>(ch));
		m_rayTracer.init(cw, ch, projMatrix, modelview);
		auto ray = m_rayTracer.getRay(x, y);

		m_debugRay = ray;

		m_result.reset();

		MyPlane plane(MyVec3f(0,1,0), 0);
		ray.raycast(m_result, plane, m_result.distance);

		MySphere sphere(MyVec3f(0,0,0), 1);
		ray.raycast(m_result, sphere, m_result.distance);

		ray.raycast(m_result, m_mesh, m_result.distance);

		m_rayTracer.render(m_mesh);

		{
			int tw = my_next_pow2(cw);
			int th = my_next_pow2(ch);
			m_tex.create(tw, th, nullptr);
			m_tex.subImage(0, 0, cw, ch, m_rayTracer.pixels());
		}
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

		m_debugRay.draw();
		drawLightPos();

		m_mesh.draw();
		m_mesh.aabb.draw();

		drawRaycastResult();
		drawTextureOnScreen();
	}

	void drawTextureOnScreen() {
		if (!m_tex.valid())
			return;

		float cw = static_cast<float>(canvasWidth());
		float ch = static_cast<float>(canvasHeight());

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	
		Scoped_glDisable disableDepthTest(GL_DEPTH_TEST);
		{

			float tw = cw / m_tex.width();
			float th = ch / m_tex.height();
			m_tex.bind();
			glBegin(GL_QUADS);
				glTexCoord2f(0,  th); glVertex3f(-1, -1, 1);
				glTexCoord2f(tw, th); glVertex3f( 1, -1, 1);
				glTexCoord2f(tw, 0 ); glVertex3f( 1,  1, 1);
				glTexCoord2f(0,  0 ); glVertex3f(-1,  1, 1);
			glEnd();
			m_tex.unbind();
		}

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void drawLightPos() {
		Scoped_glColor color(1,1,0,1);
		glBegin(GL_POINTS);
			glVertex3fv(m_rayTracer.lightPos().data);
		glEnd();
	}

	void drawRaycastResult() {
		if (!m_result.hasResult)
			return;

		Scoped_glColor color(1,0,0,1);
		glBegin(GL_POINTS);
			glVertex3fv(m_result.point.data);
		glEnd();
		glBegin(GL_LINES);
			glVertex3fv(m_result.point.data);
			glVertex3fv((m_result.point + m_result.normal).data);
		glEnd();
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
		drawGrid();
		drawOriginAxis();

		example1(uptime);

		swapBuffers();
	}

	MyRayTracer m_rayTracer;

	MyHiResTimer m_uptime;
	float m_cameraX = 30;
	float m_cameraY = 30;
	float m_cameraDistance = 4.0f;

	float m_fovy = 60;

	float m_mouseLastPosX;
	float m_mouseLastPosY;

	MyMesh m_mesh;

	MyRay3f::HitResult m_result;
	MyRay3f m_debugRay;
	MyTexture2D m_tex;
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