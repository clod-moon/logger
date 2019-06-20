#ifndef _C_TIMESTAMP__
#define _C_TIMESTAMP__

#include <string>
#include <inttypes.h>

class CMilliTimestamp
{
public:
	CMilliTimestamp() :m_millisecondsSinceEpoch(0)
	{
		
	}

	explicit CMilliTimestamp(int64_t millisecondsSinceEpoch)
		:m_millisecondsSinceEpoch(millisecondsSinceEpoch)
	{

	}

	void Swap(CMilliTimestamp& other)
	{
		std::swap(m_millisecondsSinceEpoch, other.m_millisecondsSinceEpoch);
	}

	std::string ToString() const;

	
	std::string ToFormattedString(bool showMilliseconds = true) const;

	bool valid() const 
	{ 
		return m_millisecondsSinceEpoch > 0;
	}

	int64_t GetMilliseconds() const
	{ 
		return m_millisecondsSinceEpoch;
	}

	time_t SecondsSinceEpoch() const
	{
		return static_cast<time_t>(m_millisecondsSinceEpoch / kMilliSecondsPerSecond);
	}

	static CMilliTimestamp Now();
	static CMilliTimestamp Invalid()
	{
		return CMilliTimestamp();
	}

	static CMilliTimestamp FromUnixTime(time_t t)
	{
		return FromUnixTime(t, 0);
	}

	static CMilliTimestamp FromUnixTime(time_t t, int milliseconds)
	{
		return CMilliTimestamp(static_cast<int64_t>(t) * kMilliSecondsPerSecond + milliseconds);
	}

	~CMilliTimestamp()
	{

	}

public:
	static const int kMilliSecondsPerSecond = 1000 * 1000;
private:
	int64_t m_millisecondsSinceEpoch; //微秒时间戳
	//int64_t m_nanosecondsSinceEpoch;  //纳秒时间戳
};


#endif //_C_TIMESTAMP__
