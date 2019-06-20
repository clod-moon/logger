#ifndef __C_LOGSTREAM__
#define __C_LOGSTREAM__ 

#include <sstream>
#include <string>

class CLogStream
{
public:
	CLogStream();
	~CLogStream();

	typedef CLogStream self;
	typedef std::string Buffer;
public:
	self& operator<<(bool v)
	{
		m_buffer.append(v ? "1" : "0", 1);
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

	self& operator<<(const void*);

	self& operator<<(float v)
	{
		*this << static_cast<double>(v);
		return *this;
	}
	self& operator<<(double);
	// self& operator<<(long double);

	self& operator<<(char v)
	{
		m_buffer.append(&v, 1);
		return *this;
	}

	// self& operator<<(signed char);
	// self& operator<<(unsigned char);

	self& operator<<(const char* str)
	{
		if (str)
		{
			m_buffer.append(str, strlen(str));
		}
		else
		{
			m_buffer.append("(null)", 6);
		}
		return *this;
	}

	self& operator<<(const unsigned char* str)
	{
		return operator<<(reinterpret_cast<const char*>(str));
	}

	self& operator<<(const std::string& v)
	{
		m_buffer.append(v.c_str(), v.size());
		return *this;
	}

	void append(const char* data, int len) { m_buffer.append(data, len); }
	//const Buffer& buffer() const { return buffer_; }
	void resetBuffer() { m_buffer.reserve(); }

private:
	void staticCheck();

	template<typename T>
	void formatInteger(T);

	Buffer m_buffer;
	static const int kMaxNumericSize = 32;
};

#endif  //__C_LOGSTREAM__