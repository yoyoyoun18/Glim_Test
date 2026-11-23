#pragma once

/**
 * @file PipelineParser.h
 * @brief 파이프라인 문자열을 단계 목록으로 나누는 파서.
 */

#include <string>
#include <vector>

namespace ip {

/// 파이프라인의 단계 하나. 예: "threshold:128" → { "threshold", { "128" } }
struct PipelineStep {
    std::string name;
    std::vector<std::string> params;
};

/**
 * @brief "name[:param[:param...]]" 단계를 ',' 로 이어 붙인 문자열을 파싱한다.
 *
 * ',' 로 단계를 나누고, 각 단계를 ':' 로 나눠 첫 조각은 이름, 나머지는 파라미터로 쓴다.
 * 각 단계와 파라미터의 앞뒤 공백은 잘라낸다.
 *
 * 예시)
 *   "grayscale, brightness:20:1.2, threshold:128"
 *     → [0] name="grayscale",  params={}
 *       [1] name="brightness", params={"20", "1.2"}
 *       [2] name="threshold",  params={"128"}
 *
 *   " crop : 10 : 20 :100:80 "
 *     → [0] name="crop", params={"10", "20", "100", "80"}
 *
 *   "flip:h"
 *     → [0] name="flip", params={"h"}
 *
 *   "", "grayscale,,threshold", ":128", "threshold:"
 *     → ArgumentError
 *
 * 문법만 검사하며, 필터 이름과 파라미터 값의 검증은 FilterFactory 가 한다.
 * (예: "sepia", "threshold:abc" 는 여기서는 통과하고 FilterFactory 에서 오류)
 */
class PipelineParser {
public:
    /**
     * @throws ArgumentError 빈 문자열, 빈 단계(",,"), 빈 이름, 빈 파라미터인 경우.
     */
    static std::vector<PipelineStep> parse(const std::string& text);
};

} // namespace ip
