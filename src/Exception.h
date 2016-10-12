#pragma once

#include <exception>
#include <string>

enum ErrorLevel
{
	WARNING, ERROR
};

class Exception : public std::exception
{
public:
	Exception(ErrorLevel errorLevel, std::string const & message = "") throw();
	virtual ~Exception() throw();

	virtual const char* what() const throw();

	virtual void print() const throw();

private:

	ErrorLevel m_level;
	std::string m_message;
};

Exception::Exception(ErrorLevel errorLevel, std::string const & message) 
	: m_message(message),
	m_level(errorLevel)
{
	switch (errorLevel)
	{
		case WARNING:
			m_message = "[WARNING] " + message;
			break;
		case ERROR:
			m_message = "[ERROR] " + message;
			break;
	}
}

Exception::~Exception()
{

}

const char* Exception::what() const throw()
{
	return m_message.c_str();
}

void Exception::print() const throw()
{
	switch (m_level)
	{
	case WARNING:
		std::cout << m_message << std::endl;
		break;
	case ERROR:
		std::cerr << m_message << std::endl;
		break;
	}
}