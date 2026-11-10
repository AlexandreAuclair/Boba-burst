#include "Logger.h"

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

std::vector<LogEntry> Logger::messages;
std::ofstream Logger::file;

std::string CurrentDateTimeToString() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
#ifdef _WIN32
    struct tm t;
    localtime_s(&t, &now);
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &t);
#else
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
#endif
    return output;
}

void Logger::Init(const std::string& filename) {
    file.open(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Logger: Impossible d'ouvrir " << filename << std::endl;
    }
}

void Logger::Shutdown() {
    if (file.is_open()) {
        file.close();
    }
}

void Logger::Log(const std::string& message) {
    AddEntry(LOG_INFO, message);
}

void Logger::Warn(const std::string& message) {
    AddEntry(LOG_WARNING, message);
}

void Logger::Err(const std::string& message) {
    AddEntry(LOG_ERROR, message);

}

void Logger::AddEntry(LogType type, const std::string& message) {

    LogEntry logEntry;
    logEntry.type = type;
    logEntry.message = "[" + CurrentDateTimeToString() + "]: " + message;
    messages.push_back(logEntry);

    // sortie console
    switch (type) {
    case LOG_INFO:    std::cout << "\x1B[32m" << "[INFO] " << logEntry.message << "\033[0m" << std::endl; break;
    case LOG_WARNING: std::cout << "\x1B[33m" << "[WARNING] " << logEntry.message << "\033[0m" << std::endl; break;
    case LOG_ERROR:   std::cerr << "\x1B[91m" << "[ERROR] " << logEntry.message << "\033[0m" << std::endl; break;
    }

    // sortie fichier
    if (file.is_open()) {
        file << "[" << ToString(logEntry.type) << "] " << logEntry.message << std::endl;
    }
}

std::string Logger::ToString(LogType type) {
    switch (type) {
    case LOG_INFO: return "INFO";
    case LOG_WARNING: return "WARNING";
    case LOG_ERROR: return "ERROR";
    }
    return "UNKNOWN";
}