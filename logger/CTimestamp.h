/*****************************************************
** CMicroTimestamp.h
** Copyright(c) 2019 enst.org.cn. All rights reserved.
** Description:��΢��ʱ����ķ�װ
** Ver:0.1
** Created:2019/06/29
** Modified:2019/07/03
** Author: ��ӭ��
******************************************************/

#ifndef _C_TIMESTAMP__
#define _C_TIMESTAMP__

#include <string>
#include <inttypes.h>

class CMicroTimestamp
{
public:
	CMicroTimestamp() :m_microsecondsSinceEpoch(0)
	{
		
	}

	explicit CMicroTimestamp(int64_t MicrosecondsSinceEpoch)
		:m_microsecondsSinceEpoch(MicrosecondsSinceEpoch)
	{

	}

	void Swap(CMicroTimestamp& other)
	{
		std::swap(m_microsecondsSinceEpoch, other.m_microsecondsSinceEpoch);
	}

	std::string ToString() const;

	
	std::string ToFormattedString(bool showMicroseconds = true) const;

	bool valid() const 
	{ 
		return m_microsecondsSinceEpoch > 0;
	}

	int64_t GetMicroseconds() const
	{ 
		return m_microsecondsSinceEpoch;
	}

	time_t SecondsSinceEpoch() const
	{
		return static_cast<time_t>(m_microsecondsSinceEpoch / kMicroSecondsPerSecond);
	}

	static CMicroTimestamp Now();
	static CMicroTimestamp Invalid()
	{
		return CMicroTimestamp();
	}

	static CMicroTimestamp FromUnixTime(time_t t)
	{
		return FromUnixTime(t, 0);
	}

	static CMicroTimestamp FromUnixTime(time_t t, int Microseconds)
	{
		return CMicroTimestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + Microseconds);
	}

	~CMicroTimestamp()
	{

	}

public:
	static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
	int64_t m_microsecondsSinceEpoch; //΢��ʱ���
	//int64_t m_nanosecondsSinceEpoch;  //����ʱ���
};


#endif //_C_TIMESTAMP__
