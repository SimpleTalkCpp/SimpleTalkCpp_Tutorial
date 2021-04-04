#include "MyTexture2D.h"
#include "../external/libpng/png.h"

void MyTexture2D::create() {
	destroy();

	glGenTextures(1, &m_tex);

	std::vector<MyColor4b> pixels;
	pixels.resize(256 * 256);

	auto* p = pixels.data();
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			p->set(x, y, 0, 255);
			p++;
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MyTexture2D::destroy() {
	if (m_tex) {
		glDeleteTextures(1, &m_tex);
		m_tex = 0;
	}
}

void MyTexture2D::bind() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MyTexture2D::unbind() {
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

class PngReader {
public:
	~PngReader() {
		destroy();
	}

	void destroy() {
		if (m_file) {
			fclose(m_file);
			m_file = nullptr;
		}

		if (m_png) {
			png_destroy_read_struct(&m_png, &m_info, nullptr);
			m_png = nullptr;
			m_info = nullptr;
		}
	}

	static void _onRead(png_structp png_ptr, png_bytep data, png_uint_32 length) {
		auto* p = reinterpret_cast<PngReader*>(png_get_io_ptr(png_ptr));
		p->onRead(data, length);
	}

	void onRead(png_bytep data, png_uint_32 length) {
		if (!m_file)
			throw MyError("read png file");

		int ret = fread(data, length, 1, m_file);
		if (ret < 0)
			throw MyError("read png file");
	}

	void loadFile(const char* filename, bool mulAlpha) {
		destroy();

		m_file = fopen(filename, "rb");
		if (!m_file)
			throw MyError("error open png file");

		m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!m_png)
			throw MyError("png");

		m_info = png_create_info_struct(m_png);
		if (!m_info)
			throw MyError("png info");

		png_set_read_fn(m_png, this, _onRead);
		png_read_info(m_png, m_info);

		png_uint_32 w, h;
		int bit;
		int colorType;
		int interlace;
		int compression;
		int filter;

		png_get_IHDR(m_png, m_info, &w, &h, &bit, &colorType, &interlace, &compression, &filter);

		m_width = w;
		m_height = h;

		switch (colorType) {
			case PNG_COLOR_TYPE_RGB_ALPHA: {
			}break;

			case PNG_COLOR_TYPE_RGB: {
				png_set_filler(m_png, 255, PNG_FILLER_AFTER);
			}break;

			case PNG_COLOR_TYPE_GRAY: {
				png_set_gray_to_rgb(m_png);
				png_set_filler(m_png, 255, PNG_FILLER_AFTER);
			}break;

			case PNG_COLOR_TYPE_GRAY_ALPHA: {
				png_set_gray_to_rgb(m_png);
			}break;

			case PNG_COLOR_TYPE_PALETTE: {
				png_set_palette_to_rgb(m_png);

				bool hasAlpha = false;

				{
					//get palette transparency table
					png_bytep		trans_alpha = nullptr;
					int				num_trans   = 0;
					png_color_16p	trans_color = nullptr;
					png_get_tRNS(m_png, m_info, &trans_alpha, &num_trans, &trans_color);
					if (trans_alpha)
						hasAlpha = true;
				}

				if (!hasAlpha)
					png_set_filler(m_png, 255, PNG_FILLER_AFTER);
			}break;
		}

		m_pixels.resize(w * h);

		std::vector<uint8_t*> rows;
		rows.resize(h);

		for (size_t i = 0; i < h; i++) {
			rows[i] = m_pixels[i * w].data;
		}

		png_read_image(m_png, rows.data());

		if (mulAlpha) {
			for (auto& p : m_pixels) {
				 p.r = static_cast<uint8_t>(static_cast<int>(p.r) * p.a / 255);
				 p.g = static_cast<uint8_t>(static_cast<int>(p.g) * p.a / 255);
				 p.b = static_cast<uint8_t>(static_cast<int>(p.b) * p.a / 255);
			}
		}
	}

	int width () const { return m_width; }
	int height() const { return m_height; }

	const MyColor4b* pixels() const { return m_pixels.data(); }

private:
	int m_width = 0;
	int m_height = 0;
	png_structp m_png = nullptr;
	png_infop m_info = nullptr;
	std::vector<MyColor4b> m_pixels;
	FILE* m_file = nullptr;
};

void MyTexture2D::loadPngFile(const char* filename, bool mulAlpha) {
	PngReader reader;
	reader.loadFile(filename, mulAlpha);

	glGenTextures(1, &m_tex);

	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, reader.width(), reader.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, reader.pixels());

	glBindTexture(GL_TEXTURE_2D, 0);
}
