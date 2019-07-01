#include "CLogFile.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif // _WIN32


std::once_flag CLogFile::s_flag;

AppendFile::AppendFile(std::string filename)
#ifndef _WIN32
	: m_fp(fopen(filename.c_str(), "ae")), m_writtenBytes(0)
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

CLogFile::CLogFile(
	const std::string& logPath,
	const std::string& basename,
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
	m_vDaysDir(),
	m_mutex(new std::mutex),
	m_startOfPeriod(0),
	m_lastFlush(0),
	m_lastRoll(0)
{
	//获取日志存放目录下的所有日期文件夹
	//并删除超出保存日期的文件
	getDirDays();

	init();
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
			time_t thisPeriod_ = now / kRollPerSeconds * kRollPerSeconds;
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

void CLogFile::init()
{
	std::string strDate = getDate();
	std::string strLogDir = m_strPath + strDate;
	checkDir(strLogDir,strDate);

	std::string fileName = strLogDir + "/";
	fileName += m_strBasename;
	m_file.reset(new AppendFile(fileName));
	m_file->setWrittenBytes(ftell(m_file->getHandle()));
	time_t now = time(nullptr);
	m_lastFlush = now;
	m_lastRoll = now;
	time_t start = now / kRollPerSeconds * kRollPerSeconds; //
	m_startOfPeriod = start;
}

bool CLogFile::rollFile()
{
	time_t now = time(nullptr);
	time_t start = now / kRollPerSeconds * kRollPerSeconds; //

	std::string strDate = getDate();
	std::string strLogDir = m_strPath + strDate;
	checkDir(strLogDir, strDate);

	std::string fileName = strLogDir + "/";
	fileName += m_strBasename;
	std::string strRollName  = getRollLogFileName(fileName);
#ifndef _WIN32
	std::string strCmd = "mv " + fileName;
	strCmd += " ";
	strCmd += strRollName;
	system(strCmd.c_str());
#else

#endif // !_WIN32

	m_lastFlush = now;
	m_lastRoll = now;
	m_startOfPeriod = start;
	m_file.reset(new AppendFile(fileName));

	return false;
}

std::string CLogFile::getRollLogFileName(std::string& fileName)
{
	int post = fileName.find_last_of('.');
	std::string strRollFileName(fileName,0, post);
	char timebuf[32];
	struct tm tm;
	
#ifdef _WIN32
	gmtime_s(&tm,&m_lastRoll);
#else
	gmtime_r(&m_lastRoll, &tm);
#endif
	strftime(timebuf, sizeof timebuf, "%Y%m%d-%H%M%S", &tm);
	strRollFileName += timebuf;
	strRollFileName += ".log";
	return strRollFileName;
}

void CLogFile::getDirDays()
{
#ifdef WIN32
	_finddata_t file;
	long lf;
	//输入文件夹路径
	if ((lf = _findfirst(m_strPath.c_str(), &file)) != -1)
	{
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			m_vDaysDir.insert(file.name);
			if (m_vDaysDir.size() > m_nSaveDays)
			{
				//后续补全
				//std::string strCmd = "rm -rf " + *m_vDaysDir.begin();
				//system(strCmd.c_str());
			}
		}
	}
	_findclose(lf);

#else
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(m_strPath.c_str())) == NULL)
	{
		perror("Open dir error...");
		return;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 || ptr->d_type < LOG_DT_DIR)
			continue;
		else if (ptr->d_type == LOG_DT_REG)
		{
			m_vDaysDir.insert(ptr->d_name);
		}
		else if (ptr->d_type == LOG_DT_LNK)
			continue;
		else if (ptr->d_type == LOG_DT_DIR)
		{
			m_vDaysDir.insert(ptr->d_name);
		}

		if (m_vDaysDir.size() > m_nSaveDays)
		{
			std::string strCmd = "rm -rf " + *m_vDaysDir.begin();
			system(strCmd.c_str());
		}
	}
	closedir(dir);
#endif
	return;
}

std::string CLogFile::getDate()
{
	char timebuf[32] = {0};
	time_t now = time(NULL);
	struct tm tm;

#ifdef _WIN32
	gmtime_s(&tm, &now);
#else
	gmtime_r(&now, &tm);
#endif

	strftime(timebuf, sizeof timebuf, "%Y-%m-%d", &tm);
	return timebuf;
}

void CLogFile::checkDir(std::string& strLogDir,std::string& strDate)
{
#ifdef WIN32
	return;
#else
	if (access(strLogDir.c_str(), F_OK) == 0)
		return;
	else
	{	
		std::string strCmd = "mkdir -p ";
		strCmd += strLogDir;
		system(strCmd.c_str());

		m_vDaysDir.insert(strDate);
		if (m_vDaysDir.size() > m_nSaveDays)
		{
			strCmd = "rm -rf " + *m_vDaysDir.begin();
			system(strCmd.c_str());
		}
	}
#endif
}