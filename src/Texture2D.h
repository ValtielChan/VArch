#pragma once

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
	Texture2D(int width, int height);
	virtual ~Texture2D() { delete[] m_matrix; }

	virtual void set(int x, int y, T value);
	virtual void set(int i, T value) { m_matrix[i] = value; }
	virtual T get(int x, int y) { return m_matrix[y * m_width + x]; }
	virtual T get(int i) { return m_matrix[i]; }

	virtual T min() const& { return m_min; }
	virtual T max() const& { return m_max; }

	virtual void setMin(const T &min) { m_min = min; }
	virtual void setMax(const T &max) { m_max = max; }

	virtual GLuint genGLTexture() = 0;

	virtual int sizeOf();

	int width() { return m_width; }
	int height() { return m_height; }

	GLuint texture() { return m_texture; }
};

template <typename T>
Texture2D<T>::Texture2D(int width, int height) : m_width(width), m_height(height), m_min(2542.f), m_max(-2542.f)
{
	m_matrix = new T[m_width*m_height];
}

template<typename T>
inline void Texture2D<T>::set(int x, int y, T value)
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

template<typename T>
int Texture2D<T>::sizeOf()
{
	return m_width * m_height * sizeof(T);
}
