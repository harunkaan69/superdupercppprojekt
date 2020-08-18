#pragma once
#include <exception>

class parameterError : public std::exception
{
	std::string msg;
public:
	parameterError(const std::string& pMsg) : msg("[" + pMsg + "]::Invalid parameter") {}
	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};

class fileOperationError : public std::exception
{
	std::string msg;
public:
	fileOperationError(const std::string& pMsg) : msg("[" + pMsg + "]::File operation failed") {}
	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};

class corruptDataError : public std::exception
{
	std::string msg;
public:
	corruptDataError(const std::string& pMsg) : msg("[" + pMsg + "]::Corrput Data") {}
	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};