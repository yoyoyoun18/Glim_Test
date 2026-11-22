#pragma once

/**
 * @file GrayscaleFilter.h
 * @brief 컬러 이미지를 회색조로 변환하는 필터.
 */

#include "FilterBase.h"

namespace ip {

/**
 * @brief BT.601 가중치로 회색조 변환을 수행한다.
 *
 *   Gray = 0.299 R + 0.587 G + 0.114 B
 *
 * 실수 연산 대신 가중치에 256을 곱한 정수(77, 150, 29)를 사용한다.
 * 결과는 24비트를 유지하며 B, G, R 채널에 같은 값을 기록한다.
 */
class GrayscaleFilter final : public FilterBase {
public:
    void apply(ImageBuffer& image) override;
    std::string name() const override { return "grayscale"; }
};

} // namespace ip
