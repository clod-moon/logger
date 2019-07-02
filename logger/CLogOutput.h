#ifndef _C_LOG_OUTPUT__
#define _C_LOG_OUTPUT__

#include <string>
#include <mutex>
#include <memory>
#include "CLogFile.h"
#include "CLogNetHttp.h"
#include "CLogNetTcp.h"
#include <functional>

const std::size_t OUTPUT_C	  = 0x0001; //只输出到控制台console
const std::size_t OUTPUT_F	  = 0x0002; //只输出到日志文件
const std::size_t OUTPUT_NT   = 0x0004; //只输出到TCP网络
const std::size_t OUTPUT_NH   = 0x0008; //只输出到HTTP网络
const std::size_t OUTPUT_CF	  = 0x0003; //输出到控制台和日志文件
const std::size_t OUTPUT_CNT  = 0x0005; //输出到控制台和TCP网络
const std::size_t OUTPUT_CNH  = 0x0009; //输出到控制台和HTTP网络
const std::size_t OUTPUT_FNT  = 0x0006;	//输出到日志文件、TCP网络
const std::size_t OUTPUT_FNH  = 0x000A;	//输出到日志文件、HTTP网络
const std::size_t OUTPUT_CFNT = 0x0007; //输出到控台、日志文件、TCP网络
const std::size_t OUTPUT_CFNH = 0x000C; //输出到控台、日志文件、HTTP网络


const static std::size_t kLogSaveDays = 7;
const static std::size_t kLogFileSize = 1024*1024*100;
const static std::string kLogBaseName = "service.log";
#ifdef _WIN32
	const static std::string kLogBasePath = "C:";
#else
	const static std::string kLogBasePath = "./log/";
#endif
	
//typedef  void(*pOutputFunc)(const std::string&);
typedef std::function<void(const std::string&)> pOutputFunc;

class CLogOutput
{

public:
	CLogOutput(std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);
	
	~CLogOutput();

public:
	bool SetFlieOption(std::string filePath = kLogBasePath,
		std::string fileName = kLogBaseName,
		std::size_t logSaveDays = kLogSaveDays,
		std::size_t fileSize = kLogFileSize);


	bool SetNetOfTcpOption(std::string destIp,size_t destPort);

	bool SetNetOfHttpOption(std::string url);

	bool SetConsoleOption(bool isPrint);
	
	bool SetLogFileFlushOption(const size_t nFlushInterval, const size_t checkEveryN);

	void WriteLog(const std::string& strLog);

	void Flush();


private:

	void getOutputCallback();

	void writeToC(const std::string& strLog);
	void writeToF(const std::string& strLog);
	void writeToNT(const std::string& strLog);
	void writeToNH(const std::string& strLog);
	void writeToCF(const std::string& strLog);
	void writeToCNT(const std::string& strLog);
	void writeToCNH(const std::string& strLog);
	void writeToFNT(const std::string& strLog);
	void writeToFNH(const std::string& strLog);
	void writeToCFNT(const std::string& strLog);
	void writeToCFNH(const std::string& strLog);

private:
	CLogFile m_logFile;
	CLogNetHttp m_logNetHttp;
	CLogNetTcp m_logNetTcp;
	std::size_t m_outputMode;
	pOutputFunc m_pOutputCallback;
};



#endif   //_C_LOG_OUTPUT__
