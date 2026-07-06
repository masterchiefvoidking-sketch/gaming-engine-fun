#include <ctime>
#include <eve/core/logging/logger.hpp>
#include <iomanip>

namespace eve {

const char* to_string(LogLevel level) {
    switch (level) {
    case LogLevel::Trace:
        return "TRACE";
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warn:
        return "WARN";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Fatal:
        return "FATAL";
    }
    return "UNKNOWN";
}

void ConsoleLogSink::write(LogLevel level, std::string_view message) {
    std::ostream& out = level >= LogLevel::Error ? std::cerr : std::cout;
    out << message << '\n';
}

FileLogSink::FileLogSink(std::string path)
    : stream_(std::move(path), std::ios::app) {}

FileLogSink::~FileLogSink() = default;

void FileLogSink::write(LogLevel level, std::string_view message) {
    std::lock_guard lock(mutex_);
    if (stream_.is_open()) {
        stream_ << message << '\n';
        stream_.flush();
    }
}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

Logger::Logger() {
    add_sink(std::make_unique<ConsoleLogSink>());
}

void Logger::set_level(LogLevel level) {
    std::lock_guard lock(mutex_);
    level_ = level;
}

void Logger::add_sink(std::unique_ptr<LogSink> sink) {
    std::lock_guard lock(mutex_);
    sinks_.push_back(std::move(sink));
}

void Logger::clear_sinks() {
    std::lock_guard lock(mutex_);
    sinks_.clear();
}

void Logger::log(LogLevel level, std::string_view category, std::string_view message) {
    if (level < level_) {
        return;
    }

    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time{};
#if defined(_WIN32)
    localtime_s(&local_time, &time);
#else
    localtime_r(&time, &local_time);
#endif

    std::ostringstream formatted;
    formatted << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << " [" << to_string(level)
              << "] [" << category << "] " << message;

    const std::string output = formatted.str();
    std::lock_guard lock(mutex_);
    for (const auto& sink : sinks_) {
        sink->write(level, output);
    }
}

} // namespace eve
