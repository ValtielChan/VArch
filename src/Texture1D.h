#pragma once

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
	Texture1D(int width);
	virtual ~Texture1D() { delete[] m_matrix; }

	virtual void set(int i, T value);
	virtual T get(int i) { return m_matrix[i]; }

	virtual T min() const& { return m_min; }
	virtual T max() const& { return m_max; }

	virtual void setMin(const T &min) { m_min = min; }
	virtual void setMax(const T &max) { m_max = max; }

	virtual GLuint genGLTexture(GLuint = 0) = 0;

	virtual int sizeOf();

	int width() { return m_width; }
	int height() { return m_height; }

	GLuint texture() { return m_texture; }
};

template <typename T>
Texture1D<T>::Texture1D(int width) : m_width(width), m_min(INT16_MAX), m_max(INT16_MIN)
{
	m_matrix = new T[m_width];
}

template<typename T>
inline void Texture1D<T>::set(int i, T value)
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

template<typename T>
int Texture1D<T>::sizeOf()
{
	return m_width * sizeof(T);
}
