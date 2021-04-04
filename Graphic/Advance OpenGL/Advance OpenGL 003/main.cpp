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

		m_mesh.loadObjFile("../models/test.obj");
		m_mesh.doubleSided = true;
		//m_mesh.wireframe = true;

		m_simpleColorShader.loadFile(   "../shaders/simpleColor");
		m_simpleLightingShader.loadFile("../shaders/simpleLighting");
		m_simpleTextureShader.loadFile( "../shaders/simpleTexture");

		m_tex0.loadPngFile("../models/Kingfisher.png", false);
		m_tex1.loadPngFile("../models/happymask.png", false);
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
				m_simpleTextureShader.reload();
			}break;
		}

		m_mouseLastPosX = ev.x;
		m_mouseLastPosY = ev.y;
	}

	void initCamera() {
		float aspect = static_cast<float>(canvasWidth()) / canvasHeight();

		m_matProj.setPerspective(my_deg_to_rad(m_fovy), aspect, 0.01f, 1000.0f);

		m_matModelview.setTranslate(0, 0, -m_cameraDistance);
		m_matModelview.rotateX(my_deg_to_rad(m_cameraX));
		m_matModelview.rotateY(my_deg_to_rad(m_cameraY));
	}

	void example1(float uptime) {
		auto matMVP = m_matProj * m_matModelview;

		{
			auto& s = m_simpleColorShader;
			s.bind();
			s.setUniform("matMVP", matMVP);

			s.draw(m_gridMesh);
			s.draw(m_orignMesh);

			s.unbind();
		}

		{
			auto& s = m_simpleTextureShader;
			s.bind();

//			s.dumpActiveAttrib();

			s.setUniform("uptime", uptime);
			s.setUniform("matMVP", matMVP);
			s.setUniform("sampler0", m_tex0);
			s.setUniform("sampler1", m_tex1);

			s.draw(m_mesh);
			s.unbind();
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

	MyTexture2D m_tex0;
	MyTexture2D m_tex1;

	MyShader m_simpleColorShader;
	MyShader m_simpleLightingShader;
	MyShader m_simpleTextureShader;
};

int main() {
	MyDemoWindow w;
	w.create();

    MSG msg;

	while(GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);  
		DispatchMessage(&msg);  
	}	

	return static_cast<int>(msg.wParam);
}