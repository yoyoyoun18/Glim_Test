#pragma once

/**
 * @file CropFilter.h
 * @brief 이미지의 사각형 영역을 잘라내는 필터.
 */

#include "FilterBase.h"

namespace ip {

/**
 * @brief (x, y) 를 왼쪽 위 꼭짓점으로 하는 width x height 영역을 잘라낸다.
 *
 * 좌표는 top-down 기준이다 (y = 0 이 이미지 맨 위).
 * 결과 이미지 크기가 바뀌므로 새 버퍼에 복사한 뒤 교체한다.
 */
class CropFilter final : public FilterBase {
public:
    /**
     * @throws FilterError x, y 가 음수이거나 width, height 가 0 이하인 경우.
     */
    CropFilter(int x, int y, int width, int height);

    /**
     * @throws FilterError 잘라낼 영역이 이미지 범위를 벗어나는 경우.
     */
    void apply(ImageBuffer& image) override;
    std::string name() const override;

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};

} // namespace ip
