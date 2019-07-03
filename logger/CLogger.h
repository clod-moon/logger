/*****************************************************
** CLogger.h
** Copyright(c) 2019 enst.org.cn. All rights reserved.
** Description:日志库暴露在外部的头文件
** Ver:0.1
** Created:2019/06/29
** Modified:2019/07/03
** Author: 张迎春
******************************************************/

#ifndef _C_CLogger__
#define _C_CLogger__

#include "CLogStream.h"
#include "CTimestamp.h"
#include "CLogOutput.h"
#include <string>

class CLogger
{
public:
	enum LogLevel
	{
		TRACE,
		DEBUG_,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};

	enum LogFmtType
	{
		STRING,
		JSON,
	};

	CLogger(const char* file,int line, LogLevel level, const char* func);

	~CLogger();

	/*********************************************
	**说明：通过该函数可以获取logger中stream类型
	**param：
	**		无
	**return:
	**		type ：CLogStream& 
	********************************************/
	CLogStream& stream() { return m_LogStream; }

	/*********************************************
	**说明：通过该函数可以获取日志输出的级别
	**param：
	**		level：日志的输出级别
	**return:
	**		type ：LogLevel 日志级别
	********************************************/
	static LogLevel logLevel();

	/*********************************************
	**说明：通过该函数可以设置日志输出的级别
	**param：
	**		level：日志的输出级别
	********************************************/
	static void setLogLevel(LogLevel level);

	/*********************************************
	**说明：通过该函数可以设置日志写文件的参数
	**param：
	**		strLogPath：设置日志的输出路径，该目录可不存在
	**		strLogBaseName： 设置日志的基本名称
	**		logLevel：设置日志的输出级别
	**		logFileSize： 设置每个日志文件的最大文件大小
	**		logSaveDays：设置日志的保存天数
	**		logFmtType：设置日志的输出格式，默认普通字符串
	********************************************/
	static void setOutputFileOptions(const std::string&strLogPath,const std::string& strLogBaseName
		,const LogLevel logLevel,const size_t logFileSize,const size_t logSaveDays = 7,const LogFmtType logFmtType = STRING
		);

	/*********************************************
	**说明：通过该函数可以设置日志是否输出到控制台
	**param：
	**		isPrint：输出到控制台true，否则false
	********************************************/
	static void setOutputConsoleOptions(const bool isPrint);

	/*********************************************
	**说明：通过该函数可以设置日志输出
	**param：
	**		strDestIp：设置日志接收端ip
	**		nDestPort：设置日志接收端端口号
	********************************************/
	static void setOutputNetOfTcpOptions(const std::string& strDestIp,const int nDestPort);

	/*********************************************
	**说明：通过该函数可以设置日志输出http地址
	**param：
	**		strUrl：http url地址
	********************************************/
	static void setOutputNetOfHttpOptions(const std::string strUrl);

	/*********************************************
	**说明：通过该函数可以设置日志写文件时缓冲区刷新的参数
	**param：
	**		nFlushInterval：日志每隔多少秒刷新一次
	**		checkEveryN： 日志没写入多少条是尝试刷新缓冲区
	********************************************/
	static void setLogFileFlushOption(const size_t nFlushInterval, const size_t checkEveryN);

	/*********************************************
	**说明：通过该函数可以设置设置日志的时间戳时区
	**param：
	**		strLogPath：时区
	********************************************/
	static void setTimeZone(const std::string tz);

	/*********************************************
	**说明：通过该函数可以设置日志写文件的参数
	**param：
	**		LogFmtType：设置日志的输出类型 string或者json
	********************************************/
	static void setLogFmtType(const LogFmtType t);

private:
	void fmtToJson();
	void writeLog();
private:
	CMicroTimestamp&&		m_time;
	CLogStream  			m_LogStream;
	const char*				m_strFile;
	LogLevel				m_nLevel;
	int						m_nLine;
	const char*				m_strFunc;
	std::stringstream       m_head;
	bool					m_isFlush;
public:
	static LogFmtType	  s_logFmtType;
	static LogLevel		  s_writeLogLevel;
	static CLogOutput	  s_Output;
	static std::string    s_strLogTimeZone;

};

inline CLogger::LogLevel CLogger::logLevel()
{
	return s_writeLogLevel;
}

#define LOG_TRACE if (CLogger::logLevel() <= CLogger::TRACE) \
  CLogger(__FILE__, __LINE__, CLogger::TRACE, __FUNCTION__).stream()
#define LOG_DEBUG if (CLogger::logLevel() <= CLogger::DEBUG_) \
  CLogger(__FILE__, __LINE__, CLogger::DEBUG_, __FUNCTION__).stream()
#define LOG_INFO if (CLogger::logLevel() <= CLogger::INFO) \
  CLogger(__FILE__, __LINE__).stream()
#define LOG_WARN CLogger(__FILE__, __LINE__, CLogger::WARN,__FUNCTION__).stream()
#define LOG_ERROR CLogger(__FILE__, __LINE__, CLogger::ERROR,__FUNCTION__).stream()
#define LOG_FATAL CLogger(__FILE__, __LINE__, CLogger::FATAL,__FUNCTION__).stream()
#define LOG_SYSERR CLogger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL CLogger(__FILE__, __LINE__, true).stream()


#define CHECK_NOTNULL(val) \
  ::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

template <typename T>
T* CheckNotNull(int line, const char *szNames, T* ptr)
{
	if (ptr == NULL)
	{
		CLogger(line, CLogger::FATAL, szNames).stream() << szNames;
	}
	return ptr;
}

#endif  //_C_CLogger__
