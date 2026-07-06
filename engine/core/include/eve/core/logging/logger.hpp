#pragma once

#include <chrono>
#include <eve/core/types.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace eve {

enum class LogLevel : u8 {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Fatal = 5,
};

class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void write(LogLevel level, std::string_view message) = 0;
};

class ConsoleLogSink final : public LogSink {
public:
    void write(LogLevel level, std::string_view message) override;
};

class FileLogSink final : public LogSink {
public:
    explicit FileLogSink(std::string path);
    ~FileLogSink() override;

    void write(LogLevel level, std::string_view message) override;

private:
    std::ofstream stream_;
    std::mutex mutex_;
};

class Logger {
public:
    static Logger& instance();

    void set_level(LogLevel level);
    void add_sink(std::unique_ptr<LogSink> sink);
    void clear_sinks();

    void log(LogLevel level, std::string_view category, std::string_view message);

    template <typename... Args>
    void log_format(LogLevel level, std::string_view category, Args&&... args) {
        std::ostringstream stream;
        (stream << ... << std::forward<Args>(args));
        log(level, category, stream.str());
    }

private:
    Logger();

    LogLevel level_ = LogLevel::Info;
    std::vector<std::unique_ptr<LogSink>> sinks_;
    std::mutex mutex_;
};

const char* to_string(LogLevel level);

#define EVE_LOG(level, category, ...)                                                              \
    ::eve::Logger::instance().log_format(::eve::LogLevel::level, category, __VA_ARGS__)

} // namespace eve
