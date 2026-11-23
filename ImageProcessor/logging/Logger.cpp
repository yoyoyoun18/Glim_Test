/**
 * @file Logger.cpp
 */

#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <system_error>

namespace ip {

namespace {
    const char* const kLogFileName = "ImageProcessingLog.csv";

    /// 현재 시각을 지역 시간으로 돌려준다.
    std::tm localNow(std::chrono::system_clock::time_point now) {
        const std::time_t t = std::chrono::system_clock::to_time_t(now);
        return *std::localtime(&t);
    }

    /// "2026-11-23"
    std::string today() {
        std::ostringstream oss;
        const std::tm tm = localNow(std::chrono::system_clock::now());
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }

    /// "2026-11-23 14:03:12.345"
    std::string timestamp() {
        const auto now = std::chrono::system_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now.time_since_epoch()).count() % 1000;
        const std::tm tm = localNow(now);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << ms;
        return oss.str();
    }

    /// CSV 칸 하나로 쓸 수 있게 따옴표로 감싼다. (안의 " 는 "" 로 바꾼다)
    std::string csvQuote(const std::string& value) {
        std::string result = "\"";
        for (const char c : value) {
            if (c == '"') {
                result += "\"\"";
            }
            else {
                result += c;
            }
        }
        result += '"';
        return result;
    }
}

Logger::Logger()
    : Logger(std::filesystem::path("Log")) {
}

Logger::Logger(const std::filesystem::path& baseDir) {
    const std::filesystem::path dir = baseDir / today();
    m_path = dir / kLogFileName;

    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec) {
        std::cerr << "[Log] Cannot create log directory: " << dir.string()
                  << " (" << ec.message() << ")\n";
        return;
    }

    const bool isNewFile = !std::filesystem::exists(m_path, ec);
    m_file.open(m_path, std::ios::out | std::ios::app);
    if (!m_file) {
        std::cerr << "[Log] Cannot open log file: " << m_path.string() << "\n";
        return;
    }
    if (isNewFile) {
        m_file << "Time,Level,Message\n";
        m_file.flush();
    }
}

void Logger::info(const std::string& message) {
    write("INFO", message);
}

void Logger::error(const std::string& message) {
    write("ERROR", message);
}

void Logger::write(const std::string& level, const std::string& message) {
    if (!m_file.is_open()) {
        return;
    }
    m_file << timestamp() << ',' << level << ',' << csvQuote(message) << '\n';
    m_file.flush();  // 비정상 종료 시에도 여기까지는 남도록
}

} // namespace ip
