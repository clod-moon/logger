#include "CLogOutput.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

CLogOutput::CLogOutput(std::string filePath,
	std::string fileName,
	std::size_t logSaveDays,
	std::size_t fileSize)
	:m_logFile(filePath, fileName, fileSize,logSaveDays),
	m_outputMode(OUTPUT_F)
{
	getOutputCallback();
}


CLogOutput::~CLogOutput()
{

}




bool CLogOutput::SetFlieOption(std::string filePath,
	std::string fileName ,
	std::size_t logSaveDays,
	std::size_t fileSize)
{
	m_logFile.SetOption(filePath, fileName, fileSize, logSaveDays);
	getOutputCallback();
}


bool CLogOutput::SetNetOfTcpOption(std::string destIp, size_t destPort)
{
	m_outputMode += OUTPUT_NT;
	getOutputCallback();
}

bool CLogOutput::SetNetOfHttpOption(std::string url)
{
	m_outputMode += OUTPUT_NH;
	getOutputCallback();
}

bool CLogOutput::SetConsoleOption(bool isPrint)
{
	m_outputMode += OUTPUT_C;
	getOutputCallback();
}

bool CLogOutput::SetLogFileFlushOption(const size_t nFlushInterval, const size_t checkEveryN)
{
	m_logFile.SetLogFileFlushOption(nFlushInterval, checkEveryN);
}

void CLogOutput::WriteLog(const std::string& strLog)
{
	m_pOutputCallback(strLog);
}


void CLogOutput::getOutputCallback()
{
	switch (m_outputMode)
	{
	case OUTPUT_C:
		m_pOutputCallback = std::bind(&CLogOutput::writeToC,this,std::placeholders::_1);
		break;
	case OUTPUT_F:
		m_pOutputCallback = std::bind(&CLogOutput::writeToF, this, std::placeholders::_1);
		break;
	case OUTPUT_NT:
		m_pOutputCallback = std::bind(&CLogOutput::writeToNT, this, std::placeholders::_1);
		break;
	case OUTPUT_NH:
		m_pOutputCallback = std::bind(&CLogOutput::writeToNH, this, std::placeholders::_1);
		break;
	case OUTPUT_CF:
		m_pOutputCallback = std::bind(&CLogOutput::writeToCF, this, std::placeholders::_1);
		break;
	case OUTPUT_CNH:
		m_pOutputCallback = std::bind(&CLogOutput::writeToCNH, this, std::placeholders::_1);
		break;
	case OUTPUT_CNT:
		m_pOutputCallback = std::bind(&CLogOutput::writeToCNT, this, std::placeholders::_1);
		break;
	case OUTPUT_FNT:
		m_pOutputCallback = std::bind(&CLogOutput::writeToFNT, this, std::placeholders::_1);
		break;
	case OUTPUT_FNH:
		m_pOutputCallback = std::bind(&CLogOutput::writeToFNH, this, std::placeholders::_1);
		break;
	case OUTPUT_CFNT:
		m_pOutputCallback = std::bind(&CLogOutput::writeToCFNT, this, std::placeholders::_1);
		break;
	case OUTPUT_CFNH:
		m_pOutputCallback = std::bind(&CLogOutput::writeToCFNH, this, std::placeholders::_1);
		break;
	default:
		break;
	}
}


void CLogOutput::writeToC(const std::string& strLog)
{
	fwrite(strLog.c_str(), 1, strLog.length(), stdout);
}

void CLogOutput::writeToF(const std::string& strLog)
{
	m_logFile.append(strLog);
}

void CLogOutput::writeToNT(const std::string& strLog)
{

}

void CLogOutput::writeToNH(const std::string& strLog)
{

}

void CLogOutput::writeToCF(const std::string& strLog)
{
	fwrite(strLog.c_str(), 1, strLog.length(), stdout);
	m_logFile.append(strLog);
}

void CLogOutput::writeToCNT(const std::string& strLog)
{
	fwrite(strLog.c_str(), 1, strLog.length(), stdout);

}

void CLogOutput::writeToCNH(const std::string& strLog)
{
	fwrite(strLog.c_str(), 1, strLog.length(), stdout);
}

void CLogOutput::writeToFNT(const std::string& strLog)
{
	m_logFile.append(strLog);
}

void CLogOutput::writeToFNH(const std::string& strLog)
{
	m_logFile.append(strLog);
}

void CLogOutput::writeToCFNT(const std::string& strLog)
{
	fwrite(strLog.c_str(), 1, strLog.length(), stdout);
	m_logFile.append(strLog);
}

void CLogOutput::writeToCFNH(const std::string& strLog)
{
	fwrite(strLog.c_str(), 1, strLog.length(), stdout);
	m_logFile.append(strLog);
}


void CLogOutput::Flush()
{

}
