#pragma once

/**
 * @file Stopwatch.h
 * @brief 처리 시간 측정용 스톱워치.
 */

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

namespace ip {

/**
 * @brief 만든 시점부터 지난 시간을 잰다.
 *
 * 예시)
 *   Stopwatch sw;
 *   filter->apply(image);
 *   logger.info("... (" + sw.elapsedText() + ")");   // "... (1.2 ms)"
 */
class Stopwatch {
public:
    Stopwatch() : m_start(std::chrono::steady_clock::now()) {}

    /// 지난 시간(ms)
    double elapsedMs() const {
        const auto d = std::chrono::steady_clock::now() - m_start;
        return std::chrono::duration<double, std::milli>(d).count();
    }

    /// 지난 시간을 "1.2 ms" 형태 문자열로
    std::string elapsedText() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << elapsedMs() << " ms";
        return oss.str();
    }

private:
    std::chrono::steady_clock::time_point m_start;
};

} // namespace ip
