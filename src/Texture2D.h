#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "Exception.h"

#define EXCEPTION_ID "[Texture2D]"

template <typename T>
class Texture2D
{
protected:

	GLuint m_texture;

	T * m_matrix;

	T m_min;
	T m_max;

	int m_width;
	int m_height;

public:
	Texture2D(int width, int height) : m_width(width), m_height(height), m_min(INT16_MAX), m_max(INT16_MIN)
	{
		m_texture = 0;
		m_matrix = new T[m_width*m_height];
	}

	virtual ~Texture2D() { delete[] m_matrix; }

	virtual void set(int x, int y, T value)
	{
		try {
			if (x >= 0 || x < m_width || y >= 0 || y < m_height)
				m_matrix[y * m_width + x] = value;
			else
				throw Exception(ErrorLevel::ERROR, std::string(EXCEPTION_ID) + " Out of bound texel access");
		}
		catch (Exception const &e) {
			e.print();
		}
	}

	virtual void set(int i, T value) { m_matrix[i] = value; }
	virtual T get(int x, int y) { return m_matrix[y * m_width + x]; }
	virtual T get(int i) { return m_matrix[i]; }

	virtual T min() const& { return m_min; }
	virtual T max() const& { return m_max; }

	virtual void setMin(const T &min) { m_min = min; }
	virtual void setMax(const T &max) { m_max = max; }

	virtual GLuint genGLTexture(GLuint = 0) = 0;
	virtual GLuint getTextureId()
	{
		if (m_texture == 0)
			genGLTexture();

		return m_texture;
	}

	virtual int sizeOf()
	{
		return m_width * m_height * sizeof(T);
	}

	int width() { return m_width; }
	int height() { return m_height; }

	GLuint texture() { return m_texture; }
};

#endif // !TEXTURE2D_H