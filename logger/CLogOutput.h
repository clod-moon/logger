#ifndef _C_LOG_OUTPUT__
#define _C_LOG_OUTPUT__

#include <string>
#include <mutex>
#include <memory>
#include "CLogFile.h"
#include "CLogNetHttp.h"
#include "CLogNetTcp.h"

const std::size_t OUTPUT_C	 = 0x0001; //ֻ���������̨console
const std::size_t OUTPUT_CF	 = 0x0002; //���������̨����־�ļ�
const std::size_t OUTPUT_CNT  = 0x0003; //���������̨��TCP����
const std::size_t OUTPUT_CNH	 = 0x0004; //���������̨��HTTP����
const std::size_t OUTPUT_CFNT = 0x0005; //�������̨����־�ļ���TCP����
const std::size_t OUTPUT_CFNH = 0x0006; //�������̨����־�ļ���HTTP����


const static std::size_t kLogSaveDays = 7;
const static std::size_t kLogFileSize = 1024*1024*100;
const static std::string kLogBaseName = "service.log";
#ifdef _WIN32
	const static std::string kLogBasePath = "C:";
#else
	const static std::string kLogBasePath = "/var/log/";
#endif


class CLogOutput
{

public:
	CLogOutput(std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);
	
	CLogOutput(const CLogOutput& other);
	~CLogOutput();

public:
	bool SetOutputOption(std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);

	bool SetOutputOption(std::string http,std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);

	bool SetOutputOption(std::string destIp,size_t destPort);

	bool SetOutputOption(std::string url);

	bool SetOutputOption(std::string destIp, size_t destPort,
		std::string http, std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);

	bool SetOutputOption(std::string url,
		std::string http, std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);

	void WriteLog(const std::string& strLog);

	void Flush();

private:
	CLogFile m_logFile;
	CLogNetHttp m_logNetHttp;
	CLogNetTcp m_logNetTcp;
	std::size_t m_outputMode;
};



#endif   //_C_LOG_OUTPUT__
