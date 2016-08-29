#pragma once

#include <iostream>

template <typename T>
class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}
	
	static T* m_singleton;

public:

	static T* getInstance() 
	{
		if (m_singleton == NULL) 
			m_singleton = new T();
		
		return (static_cast<T*> (m_singleton));
	}

	static void kill()
	{
		if (NULL != m_singleton) {

			delete m_singleton;
			m_singleton = NULL;
		}
	}
};

template <typename T>
T *Singleton<T>::m_singleton = NULL;