#pragma once

/**
 * @file ResizeFilter.h
 * @brief 이미지 크기를 바꾸는 필터 (Nearest Neighbor).
 */

#include "FilterBase.h"

namespace ip {

/**
 * @brief 최근접(Nearest Neighbor) 보간으로 이미지를 width x height 로 바꾼다.
 *
 * 결과 픽셀 (x, y) 는 원본의 (x * srcW / dstW, y * srcH / dstH) 픽셀 값을 그대로 가져온다.
 * 결과 이미지 크기가 바뀌므로 새 버퍼에 쓴 뒤 교체한다.
 */
class ResizeFilter final : public FilterBase {
public:
    /**
     * @throws FilterError width, height 가 0 이하인 경우.
     */
    ResizeFilter(int width, int height);

    void apply(ImageBuffer& image) override;
    std::string name() const override;

private:
    int m_width;
    int m_height;
};

} // namespace ip
