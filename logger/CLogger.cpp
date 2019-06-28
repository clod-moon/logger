#include "CLogger.h"
#include <assert.h>
#include <thread>
#include "CTimestamp.h"
#include <iostream>
#include <iomanip>

CLogger::LogStringType  CLogger::s_logStringType = CLogger::STRING;
CLogger::LogLevel		CLogger::s_WriteLogLevel = CLogger::DEBUG_;
CLogOutput	  CLogger::s_Output;
std::string    CLogger::s_strLogTimeZone = "";

void initLogLevel()
{
	CLogger::setLogLevel(CLogger::INFO);
	return ;
}


const char* LogLevelName[CLogger::NUM_LOG_LEVELS] =
{
	"TRACE ",
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};


CLogger::CLogger(const char* file,int line, LogLevel level, const char* func)
	:m_strFile(file),m_nLevel(level), m_nLine(line), 
	m_strFunc(func),
	m_time(std::move(CMilliTimestamp::Now())),
	m_isFlush(false)
{
}

CLogger::~CLogger()
{
	if (!m_isFlush)
	{
		writeLog();
		s_Output.WriteLog(m_head.str());
	}
	if (m_nLevel == FATAL)
	{
		s_Output.Flush();
		abort();
	}
}


void CLogger::fmtToJson()
{
	 m_head <<"{ \"timestamp\":";
	 m_head << '"' << m_time.ToFormattedString() << '",';
	 m_head << "\"file\":" << m_strFile << '",';
	 m_head << "\"logLevel\":" << '"' << LogLevelName[m_nLevel] << '",';
	 m_head << "\"line\":" << m_nLine << ',';
	 m_head << "\"func\":" << '"' << m_strFunc << '",';
	 m_head << "\content\":" << '"' << m_LogStream.ToString() << '"}';
}

void CLogger::writeLog()
{
	switch (s_logStringType)
	{
	case CLogger::STRING:
		m_head << "["<<m_time.ToFormattedString() << "] ["<<m_strFile<<"] [" << 
			std::setw(6) << std::setfill(' ') << std::left << LogLevelName[m_nLevel]
			<< "] ["<<std::setw(5) << std::setfill(' ') << std::left << m_nLine << "] [" 
			<< m_strFunc << "] " << m_LogStream.ToString()<<"\n";
		break;
	case CLogger::JSON:
		fmtToJson();
		break;
	default:
		break;
	}
}

void CLogger::setLogLevel(CLogger::LogLevel level)
{
	s_WriteLogLevel = level;
}

void CLogger::setOutput()
{
	s_Output.SetOutputOption();
}

void CLogger::setTimeZone(const std::string tz)
{
	s_strLogTimeZone = tz;
}

void CLogger::setLogStringType(const LogStringType t)
{
	s_logStringType = t;
}
