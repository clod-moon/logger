#include "CLogfile.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

AppendFile::AppendFile(std::string filename)
#ifndef _WIN32
	: m_fp(::fopen(filename.c_str(), "ae")), m_writtenBytes(0)
#else
	: m_writtenBytes(0)
#endif // !_WIN32
{

#ifdef _WIN32
	auto err =fopen_s(&m_fp, filename.c_str(), "ae");
	if (err == 0) {
		printf("The file 'crt_fopen_s.c' was opened\n");
	}
#endif // _WIN32
	assert(m_fp);
	setvbuf(m_fp, m_buffer, _IOLBF,sizeof m_buffer);
}

AppendFile::~AppendFile()
{
	::fclose(m_fp);
}

void AppendFile::append(const char* logline, const size_t len)
{
	size_t n = write(logline, len);
	size_t remain = len - n;
	while (remain > 0)
	{
		size_t x = write(logline + n, remain);
		if (x == 0)
		{
			int err = ferror(m_fp);
			if (err)
			{
#ifdef _WIN32
				char szErrBuf[64];
				strerror_s(szErrBuf, sizeof szErrBuf, err);
				fprintf_s(stderr, "AppendFile::append() failed %s\n", szErrBuf);
#else

				fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
#endif 
			}
			break;
		}
		n += x;
		remain = len - n; // remain -= x
	}

	m_writtenBytes += len;
}

void AppendFile::flush()
{
	::fflush(m_fp);
}

size_t AppendFile::write(const char* logline, size_t len)
{
	// #undef fwrite_unlocked
#ifdef _WIN32
	return fwrite(logline, 1, len, m_fp);
#else
	return fwrite_unlocked(logline, 1, len, m_fp);
#endif // !_WIN32
	
	
}

CLogFile::CLogFile(const std::string& basename,
	const std::string& logPath,
	off_t rollSize,
	std::size_t saveDays,
	int flushInterval,
	int checkEveryN)
	:m_strPath(logPath),
	m_strBasename(basename),
	m_nRollSize(rollSize),
	m_nSaveDays(saveDays),
	m_nFlushInterval(flushInterval),
	m_nCheckEvery(checkEveryN),
	m_nCount(0),
	m_mutex(new std::mutex),
	m_startOfPeriod(0),
	m_lastFlush(0),
	m_lastRoll(0)
{
	rollFile();
}

bool CLogFile::SetOption(
	const std::string& logPath,
	const std::string& basename,
	off_t rollSize,
	std::size_t saveDays,
	int flushInterval,
	int checkEveryN)
	
{
	m_strPath = logPath,
	m_strBasename = basename;
	m_nRollSize = rollSize;
	m_nSaveDays = saveDays;
	m_nFlushInterval = flushInterval;
	m_nCheckEvery = checkEveryN;
	rollFile();
	return true;
}

CLogFile::~CLogFile() = default;

void CLogFile::append(const std::string& strLogLine)
{
	if (m_mutex)
	{
		std::lock_guard<std::mutex> lock(*m_mutex);
		append_unlocked(strLogLine);
	}
	else
	{
		append_unlocked(strLogLine);
	}
}

void CLogFile::flush()
{
	if (m_mutex)
	{
		std::lock_guard<std::mutex> lock(*m_mutex);
		m_file->flush();
	}
	else
	{
		m_file->flush();
	}
}

void CLogFile::append_unlocked(const std::string & strLogLine)
{
	m_file->append(strLogLine.c_str(), strLogLine.length());

	if (m_file->writtenBytes() > m_nRollSize)
	{
		rollFile();
	}
	else
	{
		++m_nCount;
		if (m_nCount >= m_nCheckEvery)
		{
			m_nCount = 0;
			time_t now = ::time(NULL);
			time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
			if (thisPeriod_ != m_startOfPeriod)
			{
				rollFile();
			}
			else if (now - m_lastFlush > m_nFlushInterval)
			{
				m_lastFlush = now;
				m_file->flush();
			}
		}
	}
}

bool CLogFile::rollFile()
{
	time_t now = 0;
	std::string filename = getLogFileName(m_strBasename, &now);
	time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

	if (now > m_lastRoll)
	{
		m_lastRoll = now;
		m_lastFlush = now;
		m_startOfPeriod = start;
		m_file.reset(new AppendFile(filename));
		return true;
	}
	return false;
}

std::string CLogFile::getLogFileName(const std::string& basename, time_t* now)
{
	std::string filename;
	filename.reserve(basename.size() + 64);
	filename = basename;

	char timebuf[32];
	struct tm tm;
	*now = time(NULL);
#ifdef _WIN32
	gmtime_s(&tm, now);
#else
	gmtime_r(now, &tm);
#endif
	strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
	filename += timebuf;
	filename += ".log";
	return filename;
}

