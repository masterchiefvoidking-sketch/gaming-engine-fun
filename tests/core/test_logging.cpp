#include <eve/core/logging/logger.hpp>
#include <gtest/gtest.h>
#include <sstream>

namespace {

class StringLogSink final : public eve::LogSink {
public:
    void write(eve::LogLevel level, std::string_view message) override {
        (void)level;
        buffer_ << message;
    }

    std::string text() const { return buffer_.str(); }

private:
    std::ostringstream buffer_;
};

} // namespace

TEST(LoggingTest, FiltersByLevel) {
    auto& logger = eve::Logger::instance();
    logger.clear_sinks();
    auto sink = std::make_unique<StringLogSink>();
    auto* sink_ptr = sink.get();
    logger.add_sink(std::move(sink));
    logger.set_level(eve::LogLevel::Warn);

    logger.log(eve::LogLevel::Info, "Test", "hidden");
    logger.log(eve::LogLevel::Warn, "Test", "visible");

    EXPECT_EQ(sink_ptr->text().find("hidden"), std::string::npos);
    EXPECT_NE(sink_ptr->text().find("visible"), std::string::npos);

    logger.clear_sinks();
    logger.add_sink(std::make_unique<eve::ConsoleLogSink>());
}
