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

	enum LogStringType
	{
		STRING,
		JSON,
	};

	CLogger(int line, LogLevel level, const char* func);

	~CLogger();

	CLogStream& stream() { return m_LogStream; }

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);

	static void setOutput();
	static void setTimeZone(const std::string tz);
	static void setLogStringType(const LogStringType t);

private:
	void fmtToJson();
	void writeLog();
private:
	CMilliTimestamp&		m_time;
	CLogStream&				m_LogStream;
	LogLevel&				m_nLevel;
	int&					m_nLine;
	std::string&			m_strFunc;
	std::stringstream       m_head;
	bool					m_isFlush;
public:
	static LogStringType  s_logStringType; 
	static LogLevel		  s_WriteLogLevel;
	static CLogOutput	  s_Output;
	static std::string    s_strLogTimeZone;

};

CLogger::LogStringType  CLogger::s_logStringType = CLogger::STRING;
CLogger::LogLevel		CLogger::s_WriteLogLevel = CLogger::DEBUG_;
CLogOutput	  CLogger::s_Output = CLogOutput();
std::string    CLogger::s_strLogTimeZone = "";

inline CLogger::LogLevel CLogger::logLevel()
{
	return s_WriteLogLevel;
}

#define LOG_TRACE if (CLogger::logLevel() <= CLogger::TRACE) \
  CLogger(__FILE__, __LINE__, CLogger::TRACE, __func__).stream()
#define LOG_DEBUG if (CLogger::logLevel() <= CLogger::DEBUG_) \
  CLogger(__FILE__, __LINE__, CLogger::DEBUG_, __func__).stream()
#define LOG_INFO if (CLogger::logLevel() <= CLogger::INFO) \
  CLogger(__FILE__, __LINE__).stream()
#define LOG_WARN CLogger(__FILE__, __LINE__, CLogger::WARN).stream()
#define LOG_ERROR CLogger(__FILE__, __LINE__, CLogger::ERROR).stream()
#define LOG_FATAL CLogger(__FILE__, __LINE__, CLogger::FATAL).stream()
#define LOG_SYSERR CLogger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL CLogger(__FILE__, __LINE__, true).stream()


#define CHECK_NOTNULL(val) \
  ::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

template <typename T>
T* CheckNotNull(int line, const char *names, T* ptr)
{
	if (ptr == NULL)
	{
		CLogger(file, line, CLogger::FATAL).stream() << names;
	}
	return ptr;
}

#endif  //_C_CLogger__