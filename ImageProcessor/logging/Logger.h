#pragma once

/**
 * @file Logger.h
 * @brief 처리 기록을 CSV 로그 파일에 남기는 로거.
 */

#include <filesystem>
#include <fstream>
#include <string>

namespace ip {

/**
 * @brief Log/<yyyy-MM-dd>/ImageProcessingLog.csv 에 한 줄씩 기록한다.
 *
 * - 경로는 실행한 위치(현재 작업 디렉터리) 기준이다.
 * - 날짜 폴더가 없으면 만들고, 같은 날 여러 번 실행하면 같은 파일 뒤에 이어 쓴다.
 * - 파일을 새로 만들 때만 헤더(Time,Level,Message) 줄을 쓴다.
 *
 * 예시)
 *   logger.info("Step 1/3 grayscale OK (0.9 ms)");
 *     → 2026-11-23 14:03:12.352,INFO,"Step 1/3 grayscale OK (0.9 ms)"
 *
 * 로그 파일을 열 수 없으면 경고만 출력하고 기록 없이 계속 진행한다.
 * (로그 실패 때문에 이미지 처리가 실패하지 않도록)
 */
class Logger {
public:
    /// 현재 작업 디렉터리 아래 Log/<오늘 날짜>/ 에 로그 파일을 연다.
    Logger();

    /// baseDir 아래 <오늘 날짜>/ 에 로그 파일을 연다.
    explicit Logger(const std::filesystem::path& baseDir);

    void info(const std::string& message);
    void error(const std::string& message);

    bool isOpen() const { return m_file.is_open(); }
    const std::filesystem::path& path() const { return m_path; }

private:
    void write(const std::string& level, const std::string& message);

    std::filesystem::path m_path;
    std::ofstream m_file;
};

} // namespace ip
