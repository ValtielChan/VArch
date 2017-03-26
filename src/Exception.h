#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
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

#endif // EXCEPTION_H