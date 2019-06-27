#include "CLogStream.h"
#include <limits>
#include <iostream>

CLogStream::CLogStream():m_stream()
{
	std::cout << &m_stream << std::endl;
}


CLogStream::~CLogStream()
{
}


void CLogStream::staticCheck()
{
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
                "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
                "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10,
                "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10,
                "kMaxNumericSize is large enough");
}

CLogStream& CLogStream::operator<<(short v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(unsigned short v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(int v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(unsigned int v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(long v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(unsigned long v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(long long v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(unsigned long long v)
{
	m_stream << v;
	return *this;
}

CLogStream& CLogStream::operator<<(double v)
{
	m_stream << v;
	return *this;
}
