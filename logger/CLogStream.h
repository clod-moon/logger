#ifndef __C_LOGSTREAM__
#define __C_LOGSTREAM__ 

#include <sstream>
#include <string>
#include <string.h>
#include <iostream>



class CLogStream
{
public:
	CLogStream();
	~CLogStream();
	typedef CLogStream self;
public:
	self& operator<<(bool v)
	{
		m_stream << (v ? "1" : "0");
		return *this;
	}

	self& operator<<(short);
	self& operator<<(unsigned short);
	self& operator<<(int);
	self& operator<<(unsigned int);
	self& operator<<(long);
	self& operator<<(unsigned long);
	self& operator<<(long long);
	self& operator<<(unsigned long long);

	self& operator<<(float v)
	{
		*this << static_cast<double>(v);
		return *this;
	}
	self& operator<<(double);
	// self& operator<<(long double);


	self& operator<<(char v)
	{
		m_stream << v;
		return *this;
	}

	self& operator<<(signed char v)
	{
		m_stream << v;
		return *this;
	}

	self& operator<<(unsigned char v)
	{
		m_stream << v;
		return *this;
	}

	self& operator<<(const char* str)
	{
		if (str)
		{
			m_stream << str;
		}
		else
		{
			m_stream << "(null)";
		}
		return *this;
	}

	self& operator<<(const unsigned char* str)
	{
		return operator<<(reinterpret_cast<const char*>(str));
	}

	self& operator<<(const std::string& v)
	{
		m_stream << v;
		return *this;
	}

	const std::stringstream& GetStream()const
	{
		return m_stream;
	}

	const std::string ToString()
	{
		return m_stream.str();
	}

	void clearStream() 
	{ 
		m_stream.clear();
	}

private:
	void staticCheck();

	std::stringstream m_stream;
	static const int kMaxNumericSize = 32;
};

#endif  //__C_LOGSTREAM__