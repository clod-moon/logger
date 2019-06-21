#include "CLogger.h"
#include <assert.h>
#include <thread>

CLogger::LogLevel initLogLevel()
{
	if (::getenv("MUDUO_LOG_TRACE"))
		return CLogger::TRACE;
	else if (::getenv("MUDUO_LOG_DEBUG"))
		return CLogger::DEBUG_;
	else
		return CLogger::INFO;
}

CLogger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[CLogger::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

// helper class for known string length at compile time
class T
{
public:
	T(const char* str, unsigned len)
		:str_(str),
		len_(len)
	{
		assert(strlen(str) == len_);
	}

	const char* str_;
	const unsigned len_;
};

inline CLogStream& operator<<(CLogStream& s, T v)
{
	s.append(v.str_, v.len_);
	return s;
}

inline CLogStream& operator<<(CLogStream& s, const CLogger::SourceFile& v)
{
	s.append(v.data_, v.size_);
	return s;
}

void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
	//FIXME check n
	(void)n;
}

void defaultFlush()
{
	fflush(stdout);
}

CLogger::OutputFunc g_output = defaultOutput;
CLogger::FlushFunc g_flush = defaultFlush;
std::string g_logTimeZone;

CLogger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
	: m_time(CMilliTimestamp::Now()),
	m_sstream(),
	m_nLevel(level),
	m_nLine(line),
	m_strBasename(file)
{
	formatTime();
	m_sstream << std::this_thread::get_id();;
	m_sstream << LogLevelName[level];
	if (savedErrno != 0)
	{
		char buf[64];
#ifdef _WIN32
		strerror_s(buf, 64, savedErrno);
#else
		strerror_r(savedErrno,buf, 64);
#endif 
		m_sstream << buf << " (errno=" << savedErrno << ") ";

	}
}

void CLogger::Impl::formatTime()
{
	int64_t microSecondsSinceEpoch = m_time.GetMilliseconds();
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / CMilliTimestamp::kMilliSecondsPerSecond);
	int microseconds = static_cast<int>(microSecondsSinceEpoch % CMilliTimestamp::kMilliSecondsPerSecond);
	if (seconds != t_lastSecond)
	{
		t_lastSecond = seconds;
		struct tm tm_time;
		if (g_logTimeZone.valid())
		{
			tm_time = g_logTimeZone.toLocalTime(seconds);
		}
		else
		{
			::gmm_timer(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime
		}

		int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		assert(len == 17); (void)len;
	}

	if (g_logTimeZone.valid())
	{
		Fmt us(".%06d ", microseconds);
		assert(us.length() == 8);
		m_sstream << T(t_time, 17) << T(us.data(), 8);
	}
	else
	{
		Fmt us(".%06dZ ", microseconds);
		assert(us.length() == 9);
		m_sstream << T(t_time, 17) << T(us.data(), 9);
	}
}

void CLogger::Impl::finish()
{
	m_sstream << " - " << m_strBasename << ':' << m_nLine << '\n';
}

CLogger::CLogger(SourceFile file, int line)
	: m_impl(INFO, 0, file, line)
{
}

CLogger::CLogger(SourceFile file, int line, LogLevel level, const char* func)
	: m_impl(level, 0, file, line)
{
	m_impl.m_sstream << func << ' ';
}

CLogger::CLogger(SourceFile file, int line, LogLevel level)
	: m_impl(level, 0, file, line)
{
}

CLogger::CLogger(SourceFile file, int line, bool toAbort)
	: m_impl(toAbort ? FATAL : ERROR, errno, file, line)
{
}

CLogger::~CLogger()
{
	m_impl.finish();
	const std::string& buf(stream().str());
	g_output(buf.data(), buf.length());
	if (m_impl.m_nLevel == FATAL)
	{
		g_flush();
		abort();
	}
}

void CLogger::setLogLevel(CLogger::LogLevel level)
{
	g_logLevel = level;
}

void CLogger::setOutput(OutputFunc out)
{
	g_output = out;
}

void CLogger::setFlush(FlushFunc flush)
{
	g_flush = flush;
}

void CLogger::setTimeZone(const std::string tz)
{
	g_logTimeZone = tz;
}
