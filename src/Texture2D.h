#pragma once

template <typename T>
class Texture2D
{
protected:

	GLuint m_texture;

	T * m_matrix;

	int m_width;
	int m_height;

public:
	Texture2D(int width, int height);
	virtual ~Texture2D() { delete[] m_matrix; }

	virtual void set(int x, int y, T value) { m_matrix[y * m_width + x] = value; }
	virtual void set(int i, T value) { m_matrix[i] = value; }
	virtual T get(int x, int y) { return m_matrix[y * m_width + x]; }
	virtual T get(int i) { return m_matrix[i]; }

	virtual GLuint genGLTexture() = 0;

	int width() { return m_width; }
	int height() { return m_height; }

	GLuint texture() { return m_texture; }
};

template <typename T>
Texture2D<T>::Texture2D(int width, int height) : m_width(width), m_height(height)
{
	m_matrix = new T[m_width*m_height];
}