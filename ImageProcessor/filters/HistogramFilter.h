#pragma once

/**
 * @file HistogramFilter.h
 * @brief 밝기 히스토그램을 계산해 콘솔에 출력하는 분석 필터.
 */

#include "FilterBase.h"

namespace ip {

/**
 * @brief 픽셀의 밝기(BT.601 회색값) 분포를 0 ~ 255 구간별로 세어 출력한다.
 *
 * 이미지는 변경하지 않는다. 다른 필터와 같은 인터페이스를 사용하므로
 * 필터 처리 흐름(및 이후 파이프라인) 중간에 그대로 끼워 넣을 수 있다.
 *
 * 출력: 밝기 0 ~ 255 각 값의 막대그래프 256줄 + 최소/최대/평균 밝기.
 */
class HistogramFilter final : public FilterBase {
public:
    void apply(ImageBuffer& image) override;
    std::string name() const override { return "histogram"; }
};

} // namespace ip
