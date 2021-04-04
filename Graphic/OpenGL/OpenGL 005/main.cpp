#include "MyOpenGLWindow.h"

class Vec3 {
public:
	union {
		struct{
			float x, y, z;
		};
		float data[3];
	};

	Vec3() {}

	Vec3(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	void set(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	Vec3 operator*(const Vec3& r) const { return Vec3(x * r.x, y * r.y, z * r.z); }

	Vec3 operator+(float s) const { return Vec3(x + s, y + s, z + s); }
	Vec3 operator-(float s) const { return Vec3(x - s, y - s, z - s); }
	Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
	Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }
};

class Color4b {
public:
	union {
		struct {
			uint8_t r,g,b,a;
		};
		uint8_t data[4];
	};

	Color4b() {}

	Color4b(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	void set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}
};

class Color4f {
public:
	union {
		struct {
			float r,g,b,a;
		};
		float data[4];
	};

	Color4f() {}

	Color4f(float r_, float g_, float b_, float a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	void set(float r_, float g_, float b_, float a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}
};


class alignas(4) Vertex_PosColor {
public:
	Vec3 pos;
	Color4b color;
};

void my_glColorPointer(Color4b* p, int stride) {
	glColorPointer(4, GL_UNSIGNED_BYTE, stride, p);
}

void my_glColorPointer(Color4f* p, int stride) {
	glColorPointer(4, GL_FLOAT, stride, p);
}

class MyDemoWindow : public MyOpenGLWindow {
public:
	MyDemoWindow() {
		m_quadric = gluNewQuadric();
	}

	~MyDemoWindow() {
		gluDeleteQuadric(m_quadric);
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
		
		glTranslatef(0, 0, -3.0f);
		glRotatef(m_cameraX, 1,0,0);
		glRotatef(m_cameraY, 0,1,0);
	}

	void drawSphere(float radius, int subAxis, int subHeight) {
		if (subHeight < 2 || subAxis < 2)
			return;

		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_TRIANGLES);

		Vec3 top(0, radius, 0);
		Vec3 bottom(0, -radius, 0);

		//top
		for (int a = 0; a < subAxis; a++) {
			float ty0 = my_PI / subHeight;
			float r0 = sin(ty0);

			float th0 = my_PI * 2 * a     / subAxis;
			float th1 = my_PI * 2 * (a+1) / subAxis;
			Vec3 p0(cos(th0), 0, sin(th0));
			Vec3 p1(cos(th1), 0, sin(th1));

			Vec3 v0 = p0 * radius * r0;
			Vec3 v1 = p1 * radius * r0;
			v0.y = v1.y = cos(ty0);

			glVertex3fv(top.data);
			glVertex3fv(v1.data);
			glVertex3fv(v0.data);
		}

		//body
		for (int y = 1; y < subHeight - 1; y++) {
			float ty0 = my_PI *  y    / subHeight;
			float ty1 = my_PI * (y+1) / subHeight;

			float r0 = sin(ty0);
			float r1 = sin(ty1);

			for (int a = 0; a < subAxis; a++) {
				float th0 = my_PI * 2 * a     / subAxis;
				float th1 = my_PI * 2 * (a+1) / subAxis;

				Vec3 p0(cos(th0), 0, sin(th0));
				Vec3 p1(cos(th1), 0, sin(th1));

				Vec3 v0 = p0 * radius * r0;
				Vec3 v1 = p1 * radius * r0;

				Vec3 v2 = p0 * radius * r1;
				Vec3 v3 = p1 * radius * r1;

				v0.y = v1.y = cos(ty0);
				v2.y = v3.y = cos(ty1);

				glVertex3fv(v0.data);
				glVertex3fv(v1.data);
				glVertex3fv(v3.data);

				glVertex3fv(v0.data);
				glVertex3fv(v3.data);
				glVertex3fv(v2.data);
			}
		}

		//bottom
		for (int a = 0; a < subAxis; a++) {
			float ty0 = my_PI * (subHeight - 1) / subHeight;
			float r0 = sin(ty0);

			float th0 = my_PI * 2 * a     / subAxis;
			float th1 = my_PI * 2 * (a+1) / subAxis;
			Vec3 p0(cos(th0), 0, sin(th0));
			Vec3 p1(cos(th1), 0, sin(th1));

			Vec3 v0 = p0 * radius * r0;
			Vec3 v1 = p1 * radius * r0;
			v0.y = v1.y = cos(ty0);

			glVertex3fv(bottom.data);
			glVertex3fv(v0.data);
			glVertex3fv(v1.data);
		}

		glEnd();
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	void example1(float uptime) {
		initCamera();
		drawGrid();
		drawOriginAxis();

		//drawSphere(1, 40, 60);

//		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
//		gluSphere(m_quadric, 1, 24, 16);
//		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);


		float d = 0.5f;
		Vec3 pos[8] = {
			Vec3(-d,-d, d),
			Vec3( d,-d, d),
			Vec3( d, d, d),
			Vec3(-d, d, d),
		//---
			Vec3(-d,-d,-d),
			Vec3( d,-d,-d),
			Vec3( d, d,-d),
			Vec3(-d, d,-d),
		};

		Color4b col[8] {
			Color4b(255, 0, 0, 255),
			Color4b(255, 0, 0, 255),
			Color4b(255, 0, 0, 255),
			Color4b(255, 0, 0, 255),

			Color4b(0, 0, 255, 255),
			Color4b(0, 0, 255, 255),
			Color4b(0, 0, 255, 255),
			Color4b(0, 0, 255, 255),
		};

//		glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(3, GL_FLOAT, 0, pos);
//		glEnableClientState(GL_COLOR_ARRAY);
//		glColorPointer(4, GL_FLOAT, 0, col);

		Vertex_PosColor vtx[8];
		vtx[0].pos.set(-d,-d, d); vtx[0].color.set(255, 255, 255, 255);
		vtx[1].pos.set( d,-d, d); vtx[1].color.set(255, 255, 255, 255);
		vtx[2].pos.set( d, d, d); vtx[2].color.set(255, 255, 255, 255);
		vtx[3].pos.set(-d, d, d); vtx[3].color.set(255, 255, 255, 255);
						  
		vtx[4].pos.set(-d,-d,-d); vtx[4].color.set(0, 255, 0, 255);
		vtx[5].pos.set( d,-d,-d); vtx[5].color.set(0, 255, 0, 255);
		vtx[6].pos.set( d, d,-d); vtx[6].color.set(0, 255, 0, 255);
		vtx[7].pos.set(-d, d,-d); vtx[7].color.set(0, 255, 0, 255);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(vtx[0]), vtx[0].pos.data);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx[0]), vtx[0].color.data);
	

		//glDrawArrays(GL_TRIANGLES, 0, 3);

//		glBegin(GL_TRIANGLES);
//		glArrayElement(0);
//		glArrayElement(1);
//		glArrayElement(2);
//		glArrayElement(0);
//		glArrayElement(2);
//		glArrayElement(3);
//		glEnd();

		uint16_t indices[] = {
			0,1,2, 
			0,2,3,
			2,1,5,
			2,5,6,
			3,2,6,
			3,6,7,
			0,3,7,
			0,7,4,
			1,0,4,
			1,4,5,
			4,7,6,
			4,6,5,
		};

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_SHORT, indices);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	virtual void onPaint() override {
		auto uptime = static_cast<float>(m_uptime.get());

		glViewport(0, 0, canvasWidth(), canvasHeight());

		glDisable(GL_SCISSOR_TEST);

		glClearColor(0.0f, 0.2f, 0.2f, 0);
		glClearDepth(1);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

	GLUquadric* m_quadric = nullptr;
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