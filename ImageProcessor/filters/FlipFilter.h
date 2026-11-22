#pragma once

/**
 * @file FlipFilter.h
 * @brief 이미지를 상하 또는 좌우로 반전하는 필터.
 */

#include "FilterBase.h"

namespace ip {

/**
 * @brief 상하/좌우 반전을 수행한다.
 *
 * 별도 버퍼 없이 제자리에서 픽셀(또는 행)을 맞바꾼다.
 * 좌우 반전은 BGR 채널 순서가 유지되도록 픽셀(3바이트) 단위로 교환한다.
 */
class FlipFilter final : public FilterBase {
public:
    enum class Direction {
        Horizontal,  ///< 좌우 반전
        Vertical     ///< 상하 반전
    };

    explicit FlipFilter(Direction direction);

    void apply(ImageBuffer& image) override;
    std::string name() const override;

private:
    void flipHorizontal(ImageBuffer& image) const;
    void flipVertical(ImageBuffer& image) const;

    Direction m_direction;
};

} // namespace ip
