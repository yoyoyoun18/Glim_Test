#pragma once

/**
 * @file CommandLineParser.h
 * @brief 커맨드라인 인자 파싱.
 *
 * 기본 옵션(--input, --output, --filter), 필터 파라미터 옵션, --pipeline 을 파싱한다.
 */

#include <string>

namespace ip {

/// 파싱 결과를 담는 단순 구조체.
struct ProgramOptions {
    std::string inputPath;   ///< --input  / -i
    std::string outputPath;  ///< --output / -o
    std::string filterName;  ///< --filter / -f
    std::string pipeline;    ///< --pipeline / -p (예: "grayscale, threshold:128"). --filter 와 둘 중 하나만 사용
    int         threshold = 128;  ///< --threshold / -t (threshold 필터의 임계값)
    int         brightness = 0;   ///< --brightness / -b (brightness 필터의 밝기)
    double      contrast = 1.0;   ///< --contrast / -c (brightness 필터의 대비)
    int         x = 0;            ///< --x      (crop 시작 x 좌표)
    int         y = 0;            ///< --y      (crop 시작 y 좌표)
    int         width = 0;        ///< --width  (crop/resize 결과 너비, 0 = 미지정)
    int         height = 0;       ///< --height (crop/resize 결과 높이, 0 = 미지정)
};

class CommandLineParser {
public:
    CommandLineParser() = delete;  // 인스턴스화 금지 (정적 메서드만 제공)

    /**
     * @brief argv 를 파싱하여 ProgramOptions 를 반환한다.
     * @throws ArgumentError 필수 인자 누락, 알 수 없는 옵션, 형식 오류 등.
     */
    static ProgramOptions parse(int argc, char* argv[]);

    /// 사용법을 표준 출력에 출력한다.
    static void printUsage(const std::string& exeName);
};

} // namespace ip
