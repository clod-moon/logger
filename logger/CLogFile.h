#ifndef _C_LOGFILE__
#define _C_LOGFILE__

#include <string>
#include <mutex>
#include <memory>
#include <set>

enum 
{
	LOG_DT_TYPE_ALL = 0,
	LOG_DT_FIFO = 1,
	LOG_DT_CHR = 2,
	LOG_DT_DIR = 4,
	LOG_DT_BLK = 6,
	LOG_DT_REG = 8,
	LOG_DT_LNK = 10,
	LOG_DT_SOCK = 12,
	LOG_DT_WHT = 14
};

class AppendFile
{
public:
	explicit AppendFile(std::string filename);

	~AppendFile();

	void append(const char* logline, size_t len);

	void flush();

	off_t writtenBytes() const
	{ 
		return m_writtenBytes; 
	}

	FILE* getHandle()const
	{
		return m_fp;
	}

	void setWrittenBytes(off_t written)
	{
		m_writtenBytes = written;
	}

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

	void init();

private:
	void append_unlocked(const std::string & strLogLine);

	std::string getDate();
	
	//如果目录存在不做处理，如果不存在则创建
	void checkDir(std::string&, std::string&);
	void getDirDays();


	std::string getRollLogFileName(std::string& fileName);

	std::string m_strPath;
	std::string m_strBasename;
	off_t m_nRollSize;
	int m_nFlushInterval;
	int m_nCheckEvery;
	std::size_t m_nSaveDays;
	int m_nCount;
	std::set<std::string> m_vDaysDir;

	std::unique_ptr<std::mutex> m_mutex;
	time_t m_startOfPeriod;
	time_t m_lastRoll;
	time_t m_lastFlush;
	std::unique_ptr<AppendFile> m_file;

	const static int kRollPerSeconds = 60 * 60 * 24;
	static std::once_flag s_flag;
};


#endif   //_C_LOGFILE__