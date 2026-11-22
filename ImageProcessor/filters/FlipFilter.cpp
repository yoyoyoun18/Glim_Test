/**
 * @file FlipFilter.cpp
 */

#include "FlipFilter.h"

#include <cstdint>

namespace ip {

FlipFilter::FlipFilter(Direction direction)
    : m_direction(direction)
{
}

void FlipFilter::apply(ImageBuffer& image) {
    if (m_direction == Direction::Horizontal) {
        flipHorizontal(image);
    }
    else {
        flipVertical(image);
    }
}

std::string FlipFilter::name() const {
    return (m_direction == Direction::Horizontal) ? "flip:h" : "flip:v";
}

void FlipFilter::flipHorizontal(ImageBuffer& image) const {
    const int width  = image.width();
    const int height = image.height();

    for (int y = 0; y < height; ++y) {
        std::uint8_t* row = image.rowPtr(y);

        // 폭의 절반만 순회한다. 홀수 폭이면 가운데 픽셀은 그대로 둔다.
        for (int x = 0; x < width / 2; ++x) {
            std::uint8_t* left  = row + x * ImageBuffer::CHANNELS;
            std::uint8_t* right = row + (width - 1 - x) * ImageBuffer::CHANNELS;

            // 바이트 단위로 행 전체를 뒤집으면 BGR 이 RGB 로 바뀌므로
            // 픽셀(3바이트) 묶음을 통째로 교환한다.
            for (int c = 0; c < ImageBuffer::CHANNELS; ++c) {
                const std::uint8_t temp = left[c];
                left[c]  = right[c];
                right[c] = temp;
            }
        }
    }
}

void FlipFilter::flipVertical(ImageBuffer& image) const {
    const int height = image.height();
    const int stride = image.rowStride();

    // 높이의 절반만 순회한다. 홀수 높이면 가운데 행은 그대로 둔다.
    for (int y = 0; y < height / 2; ++y) {
        std::uint8_t* top    = image.rowPtr(y);
        std::uint8_t* bottom = image.rowPtr(height - 1 - y);

        for (int i = 0; i < stride; ++i) {
            const std::uint8_t temp = top[i];
            top[i]    = bottom[i];
            bottom[i] = temp;
        }
    }
}

} // namespace ip
