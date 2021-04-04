#include "MyOpenGLWindow.h"
#include "MyMesh.h"
#include "MyTexture2D.h"

void checkGLError() {
	auto e = glGetError();
	if (e != GL_NO_ERROR) {
		printf("glGetError %d\n", e);
	}	
}

class Scoped_glEnable {
public:
	Scoped_glEnable(GLenum v) {
		m_v = v;
		glEnable(m_v);
	}

	~Scoped_glEnable() {
		glDisable(m_v);
	}

private:
	GLenum m_v;
};

class Scoped_glColor {
public:
	Scoped_glColor(float r, float g, float b, float a) {
		glColor4f(r,g,b,a);
	}
	~Scoped_glColor() {
		glColor4f(1,1,1,1);
	}
};

class Scoped_glPushMatrix {
public:
	Scoped_glPushMatrix() {
		glPushMatrix();
	}
	~Scoped_glPushMatrix() {
		glPopMatrix();
	}
};

class MyDemoWindow : public MyOpenGLWindow {
public:
	virtual void onGLinit() override {
		m_mesh.loadObjFile("models/sphere_smooth.obj");
//		m_mesh.loadObjFile("models/sphere_flat.obj");
//		m_mesh.loadObjFile("models/sphere_high.obj");

		createDisplayNormals();
	}

	void createDisplayNormals() {
		float normalLength = 0.2f;

		auto n = m_mesh.vertices.size();
		m_displayNormals.resize(n * 2);
		auto* dst = m_displayNormals.data();
		int i = 0;
		for (auto& v : m_mesh.vertices) {
			*dst = v.pos; dst++;
			*dst = v.pos + v.normal * normalLength; dst++;
		}
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

	void drawDisplayNormals() {
		if (!m_displayNormals.size())
			return;

		Scoped_glColor color(1,1,0,1);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, m_displayNormals[0].data);

		glDrawArrays(GL_LINES, 0, m_displayNormals.size());

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void example1(float uptime) {
		m_mesh.wireframe = true;
		m_mesh.draw();
		drawDisplayNormals();
	}

	void example2(float uptime) {
		{
			bool directionalLight = false;
			bool isPointLight = true;

			float globalAmbient[4] = {0.2f, 0.2f, 0.2f, 1};

			float lightPos[4] = {0, 3, 0, directionalLight ? 0.0f : 1.0f};
			float ambient[4]  = {0,0,0,1};
			float diffuse[4]  = {1,1,1,1};
			float specular[4] = {1,1,1,1};

			float spotDir[3] = {0,0,-1};
			float spotExponent = 5;
			float spotCutoff = isPointLight ? 180.0f : 60.0f;

			glPointSize(12);
			Scoped_glColor color(1,0,0,1);

			glBegin(GL_POINTS);
				glVertex3fv(lightPos);
			glEnd();
			glColor4f(1,1,1,1);

			Scoped_glPushMatrix pushMatrix;
			glRotatef(uptime * 180, 0, 1, 0);

			glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

			glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
			glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, &spotExponent);
			glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &spotCutoff);

//			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  0);
//			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    0);
//			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1);

			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
		}

		{
			Scoped_glEnable lighting(GL_LIGHTING);
			Scoped_glEnable light0(GL_LIGHT0);

			float ambient[4]  = {0,0,1,1};
			float diffuse[4]  = {0,1,0,1};
			float specular[4] = {1,0,0,1};
			float shininess = 64;
			float emission[4] = {0,0,0,0};

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  emission);

			#if 0 // normalize normal
				Scoped_glEnable normalize(GL_NORMALIZE);
				Scoped_glPushMatrix pushMatrix;
				float scale = 0.25f;
				glScalef(scale, scale, scale);
			#endif

			int nx = 12;
			int nz = 12;

			float d = 3;

			glTranslatef(-nx/2 *d, 1, -nz/2 * d);

			for (int z = 0; z < nz; z++) {
				for (int x = 0; x < nx; x++) {
					Scoped_glPushMatrix pushMatrix;
					glTranslatef(x * d, 0, z * d);
					m_mesh.draw();
				}
			}
		}

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

		initCamera();
		drawGrid();
		drawOriginAxis();

		//example1(uptime);
		example2(uptime);

		swapBuffers();
	}

	MyHiResTimer m_uptime;
	float m_cameraX = 30;
	float m_cameraY = 30;
	float m_cameraDistance = 30.0f;

	float m_fovy = 60;

	float m_mouseLastPosX;
	float m_mouseLastPosY;

	MyMesh m_mesh;

	std::vector<MyVec3f> m_displayNormals;
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