#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "Exception.h"

#define EXCEPTION_ID "[Texture1D]"

template <typename T>
class Texture1D
{
protected:

	GLuint m_texture;

	T * m_matrix;

	T m_min;
	T m_max;

	int m_width;

public:
	Texture1D(int width) : m_width(width), m_min(INT16_MAX), m_max(INT16_MIN)
	{
		m_matrix = new T[m_width];
	}
	virtual ~Texture1D() { delete[] m_matrix; }

	virtual void set(int i, T value)
	{
		try {
			if (i >= 0 && i < m_width)
				m_matrix[i] = value;
			else
				throw Exception(ErrorLevel::ERROR, std::string(EXCEPTION_ID) + " Out of bound texel access");
		}
		catch (Exception const &e) {
			e.print();
		}
	}
	virtual T get(int i) { return m_matrix[i]; }

	virtual T min() const& { return m_min; }
	virtual T max() const& { return m_max; }

	virtual void setMin(const T &min) { m_min = min; }
	virtual void setMax(const T &max) { m_max = max; }

	virtual GLuint genGLTexture(GLuint = 0) = 0;

	virtual int sizeOf()
	{
		return m_width * sizeof(T);
	}

	int width() { return m_width; }
	int height() { return m_height; }

	GLuint texture() { return m_texture; }
};

#endif // !TEXTURE1D_H