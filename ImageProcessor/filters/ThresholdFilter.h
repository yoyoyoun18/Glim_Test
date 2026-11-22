#pragma once

/**
 * @file ThresholdFilter.h
 * @brief 임계값 기준으로 흑백 이진화하는 필터.
 */

#include "FilterBase.h"

namespace ip {

/**
 * @brief 회색값이 임계값보다 크면 255(흰색), 아니면 0(검정)으로 만든다.
 *
 * 컬러 입력이면 BT.601 회색값을 기준으로 판단한다.
 * 결과는 24비트를 유지하며 B, G, R 채널에 같은 값을 기록한다.
 */
class ThresholdFilter final : public FilterBase {
public:
    /**
     * @param threshold 임계값 (0 ~ 255)
     * @throws FilterError 범위를 벗어난 경우.
     */
    explicit ThresholdFilter(int threshold);

    void apply(ImageBuffer& image) override;
    std::string name() const override;

private:
    int m_threshold;
};

} // namespace ip
