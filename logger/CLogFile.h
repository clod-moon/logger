#ifndef _C_LOGFILE__
#define _C_LOGFILE__

#include <string>
#include <mutex>
#include <memory>

class AppendFile
{
public:
	explicit AppendFile(std::string filename);

	~AppendFile();

	void append(const char* logline, size_t len);

	void flush();

	off_t writtenBytes() const { return m_writtenBytes; }

private:

	size_t write(const char* logline, size_t len);

	FILE* m_fp;
	char m_buffer[64 * 1024];
	off_t m_writtenBytes;
};

class CLogFile 
{
public:
	CLogFile(
		const std::string& logPath,
		const std::string& basename,
		off_t rollSize,
		std::size_t saveDays= 7,
		int flushInterval = 3,
		int checkEveryN = 1024);

	~CLogFile();


	bool SetOption(
		const std::string& logPath,
		const std::string& basename,
		off_t rollSize,
		std::size_t saveDays = 7,
		int flushInterval = 3,
		int checkEveryN = 1024);


	void append(const std::string & strLogLine);
	void flush();
	bool rollFile();

private:
	void append_unlocked(const std::string & strLogLine);

	static std::string getLogFileName(const std::string& basename, time_t* now);

	std::string m_strPath;
	std::string m_strBasename;
	off_t m_nRollSize;
	int m_nFlushInterval;
	int m_nCheckEvery;
	std::size_t m_nSaveDays;

	int m_nCount;

	std::unique_ptr<std::mutex> m_mutex;
	time_t m_startOfPeriod;
	time_t m_lastRoll;
	time_t m_lastFlush;
	std::unique_ptr<AppendFile> m_file;

	const static int kRollPerSeconds_ = 60 * 60 * 24;
};


#endif   //_C_LOGFILE__