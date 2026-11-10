#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>
#include <fstream>

enum LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry {
	LogType type;
	std::string message;
};

class Logger {
public:
	static std::vector<LogEntry> messages;
	static std::ofstream file;

	static void Init(const std::string& filename);
	static void Shutdown();
	static void Log(const std::string& message);
	static void Warn(const std::string& message);
	static void Err(const std::string& message);

private:
	static void AddEntry(LogType type, const std::string& message);

	static std::string ToString(LogType type);
};

#endif // !LOGGER_H