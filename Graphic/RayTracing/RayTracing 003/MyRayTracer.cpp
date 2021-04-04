#include "precompiledHeader.h"
#include "MyRayTracer.h"
#include "MyMesh.h"

MyRayTracer::MyRayTracer() {
	m_lightPos.set(-1.5f, 2, 1);
}

void MyRayTracer::init(int canvasWidth, int canvasHeight, const MyMatrix4f& projMatrix, const MyMatrix4f& modelview) {
	m_projMatrix = projMatrix;
	m_modelview = modelview;

	m_invProjMatrix = projMatrix.inverse();
	m_invModelview  = modelview.inverse();

	m_canvasWidth  = canvasWidth;
	m_canvasHeight = canvasHeight;

	m_pixels.resize(m_canvasWidth * m_canvasHeight);
}

MyRay3f MyRayTracer::getRay(float x, float y) {
	MyVec2f screenSize(static_cast<float>(m_canvasWidth), static_cast<float>(m_canvasHeight));

	return MyRay3f::unprojectFromInverseMatrix(m_invProjMatrix, m_invModelview, MyVec2f(x,y), screenSize);
}

void MyRayTracer::render(const MyMesh& mesh) {
	MyRay3f::HitResult hit;

	MyTimer timer;

	for (int y = 0; y < m_canvasHeight; y++) {
		printf("\rrendering %d / %d                   ", y+1, m_canvasHeight);

		for (int x = 0; x < m_canvasWidth; x++) {
			auto ray = getRay(x + 0.5f, y + 0.5f);

			hit.reset();

//			MySphere sphere(MyVec3f(0,0,0), 1);
//			ray.raycast(hit, sphere, hit.distance);

			ray.raycast(hit, mesh, hit.distance);

			MyColor4f color(0.2f, 0, 0, 1);

			if (hit.hasResult) {
				MyRay3f::HitResult shadowHit;
				MyRay3f shadowRay(hit.point, (m_lightPos - hit.point).normalize());

				color = onPixelShading(hit);
				if (shadowRay.raycast(shadowHit, mesh, shadowHit.distance)) {
					float s = 0.25f;
					color.r *= s;
					color.g *= s;
					color.b *= s;
				}
			}

			m_pixels[y * m_canvasWidth + x] = color.toColor4b();
		}
	}
	printf("\rrender is done!  time=%0.1lfs            \n", timer.get());
}

MyColor4f MyRayTracer::onPixelShading(const MyRay3f::HitResult& hit) {
	auto lightDir = (m_lightPos - hit.point).normalize();
	auto c = hit.normal.dot(lightDir);
	return MyColor4f(c,c,c,1);
}
