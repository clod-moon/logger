/*****************************************************
** CLogFile.h
** Copyright(c) 2019 enst.org.cn. All rights reserved.
** Description:主要实现对日志文件实际写入等操作的封装
** Ver:0.1
** Created:2019/06/29
** Modified:2019/07/03
** Author: 张迎春
******************************************************/

#ifndef _C_LOGFILE__
#define _C_LOGFILE__

#include <string>
#include <mutex>
#include <memory>
#include <set>

enum 
{
	LOG_DT_TYPE_ALL = 0,
	LOG_DT_FIFO = 1,
	LOG_DT_CHR = 2,
	LOG_DT_DIR = 4,
	LOG_DT_BLK = 6,
	LOG_DT_REG = 8,
	LOG_DT_LNK = 10,
	LOG_DT_SOCK = 12,
	LOG_DT_WHT = 14
};


/*****************************************************
** 类名：AppendFile
** 说明：
**		该类是文件实质的打开，写入操作，缓冲区刷新，
**		只提供给CLogFile使用
******************************************************/
class AppendFile
{
public:
	explicit AppendFile(std::string strFileName);

	~AppendFile();

	void append(const char* szLogline, size_t len);

	void flush();

	off_t writtenBytes() const
	{ 
		return m_writtenBytes; 
	}

	FILE* getHandle()const
	{
		return m_pFp;
	}

	void setWrittenBytes(off_t written)
	{
		m_writtenBytes = written;
	}

private:

	size_t write(const char* szLogline, size_t len);

	FILE*	m_pFp;
	char	m_szBuffer[64 * 1024];
	off_t	m_writtenBytes;
};


/*****************************************************
** 类名：CLogFile
** 说明：
**		该类是对日志文件的控制，控制日志文件所在文件目录
**		的管理，包括日志路径，日志名称，日志刷新的条件，
**		以及日志保存天数等等参数的管理
******************************************************/
class CLogFile 
{
public:
	CLogFile(
		const std::string& strLogPath,
		const std::string& strBaseName,
		off_t rollSize,
		std::size_t nSaveDays= 7,
		int nFlushInterval = 3,
		int nCheckEveryN = 1024);

	~CLogFile();


	bool SetOption(
		const std::string& strLogPath,
		const std::string& strBaseName,
		off_t rollSize,
		std::size_t nSaveDays = 7,
		int nFlushInterval = 3,
		int nCheckEveryN = 1024);

	void SetLogFileFlushOption(const size_t flushInterval, const size_t checkEveryN);

	void append(const std::string & strLogLine);
	void flush();
	bool rollFile();

	void init();

private:
	void append_unlocked(const std::string & strLogLine);

	std::string getDate();
	
	//如果目录存在不做处理，如果不存在则创建
	void checkDir(std::string&, std::string&);
	void getDirDays();

	//获取日志切换文件时的名称
	std::string getRollLogFileName(const std::string& strFileName);

	std::string m_strPath;
	std::string m_strBasename;
	off_t m_nRollSize;
	int m_nFlushInterval;
	int m_nCheckEvery;
	std::size_t m_nSaveDays;
	int m_nCount;
	//存放日志目录下的日志文件夹
	std::set<std::string> m_vDaysDir; 

	//通过该锁实现线程安全
	std::unique_ptr<std::mutex> m_mutex;
	time_t m_startOfPeriod;
	time_t m_lastRoll;
	time_t m_lastFlush;
	std::unique_ptr<AppendFile> m_file;

	const static int kRollPerSeconds = 60 * 60 * 24;
};


#endif   //_C_LOGFILE__