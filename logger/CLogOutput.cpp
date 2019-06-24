#include "CLogOutput.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


CLogOutput::CLogOutput(std::string filePath,
	std::string fileName,
	std::size_t logSaveDays,
	std::size_t fileSize)
	:m_logFile(filePath, fileName, fileSize,logSaveDays)
{

}

CLogOutput::~CLogOutput()
{

}


bool CLogOutput::SetOutputOption(std::string filePath,
	std::string fileName,
	std::size_t logSaveDays,
	std::size_t fileSize)
{
	m_logFile.SetOption(filePath, fileName, fileSize, logSaveDays);
	return true;
}


bool CLogOutput::SetOutputOption(std::string http, std::string filePath,
	std::string fileName,
	std::size_t logSaveDays,
	std::size_t fileSize)
{
	return true;
}



bool CLogOutput::SetOutputOption(std::string destIp, size_t destPort)
{
	return true;
}

bool CLogOutput::SetOutputOption(std::string url)
{
	return true;
}

bool CLogOutput::SetOutputOption(std::string destIp, size_t destPort,
	std::string http, std::string filePath,
	std::string fileName,
	std::size_t logSaveDays,
	std::size_t fileSize)
{
	return true;
}

bool CLogOutput::SetOutputOption(std::string url,
	std::string http, std::string filePath,
	std::string fileName,
	std::size_t logSaveDays,
	std::size_t fileSize)
{
	return true;
}

void CLogOutput::WriteLog(std::string& strLog)
{
	switch (m_outputMode) 
	{
		fwrite(strLog.c_str(),1,strLog.length(),stdout);
	case OUTPUT_C:
		break; 
	case OUTPUT_CF:
		m_logFile.append(strLog);
		break; 
	case OUTPUT_CFNH:
		break;
	case OUTPUT_CFNT:
		break;
	case OUTPUT_CNH:
		break;
	case OUTPUT_CNT:
		break;
	default: 
		break;
	}
}

void CLogOutput::Flush()
{

}