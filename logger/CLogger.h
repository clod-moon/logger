#ifndef _C_CLogger__
#define _C_CLogger__

#include "CLogStream.h"
#include "CTimestamp.h"
#include "CLogfile.h"

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

	typedef std::stringstream CLogStream;

	// compile time calculation of basename of source file
	class SourceFile
	{
	public:
		template<int N>
		SourceFile(const char(&arr)[N])
			: data_(arr),
			size_(N - 1)
		{
			const char* slash = strrchr(data_, '/'); // builtin function
			if (slash)
			{
				data_ = slash + 1;
				size_ -= static_cast<int>(data_ - arr);
			}
		}

		explicit SourceFile(const char* filename)
			: data_(filename)
		{
			const char* slash = strrchr(filename, '/');
			if (slash)
			{
				data_ = slash + 1;
			}
			size_ = static_cast<int>(strlen(data_));
		}

		const char* data_;
		int size_;
	};

	CLogger(SourceFile file, int line);
	CLogger(SourceFile file, int line, LogLevel level);
	CLogger(SourceFile file, int line, LogLevel level, const char* func);
	CLogger(SourceFile file, int line, bool toAbort);
	~CLogger();

	CLogStream& stream() { return m_impl.m_sstream; }

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);

	typedef void(*OutputFunc)(const char* msg, int len);
	typedef void(*FlushFunc)();
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);
	static void setTimeZone(const std::string tz);

private:

	class Impl
	{
	public:
		typedef CLogger::LogLevel LogLevel;
		Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
		void formatTime();
		void finish();

		CMilliTimestamp m_time;
		std::stringstream m_sstream;
		LogLevel m_nLevel;
		int m_nLine;
		SourceFile m_strBasename;
	};

	Impl m_impl;

};

extern CLogger::LogLevel g_logLevel;

inline CLogger::LogLevel CLogger::logLevel()
{
	return g_logLevel;
}

#define LOG_TRACE if (CLogger::logLevel() <= CLogger::TRACE) \
  CLogger(__FILE__, __LINE__, CLogger::TRACE, __func__).stream()
#define LOG_DEBUG if (CLogger::logLevel() <= CLogger::DEBUG) \
  CLogger(__FILE__, __LINE__, CLogger::DEBUG, __func__).stream()
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
T* CheckNotNull(CLogger::SourceFile file, int line, const char *names, T* ptr)
{
	if (ptr == NULL)
	{
		CLogger(file, line, CLogger::FATAL).stream() << names;
	}
	return ptr;
}

#endif  //_C_CLogger__