/**
 * @file GrayscaleFilter.cpp
 */

#include "GrayscaleFilter.h"

#include <cstdint>

namespace ip {

namespace {
    // BT.601 가중치 x 256 (합계 256)
    constexpr int WEIGHT_R = 77;   // 0.299
    constexpr int WEIGHT_G = 150;  // 0.587
    constexpr int WEIGHT_B = 29;   // 0.114

    constexpr int SHIFT    = 8;    // / 256
    constexpr int ROUNDING = 128;  // 반올림용 (0.5 * 256)
} // anonymous namespace

void GrayscaleFilter::apply(ImageBuffer& image) {
    const int width  = image.width();
    const int height = image.height();

    for (int y = 0; y < height; ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < width; ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            // ImageBuffer 의 채널 순서는 B, G, R
            const int b = pixel[0];
            const int g = pixel[1];
            const int r = pixel[2];

            // 최댓값: (256 * 255 + 128) >> 8 = 255 이므로 클램핑 불필요
            const int gray = (WEIGHT_R * r + WEIGHT_G * g + WEIGHT_B * b + ROUNDING) >> SHIFT;

            pixel[0] = static_cast<std::uint8_t>(gray);
            pixel[1] = static_cast<std::uint8_t>(gray);
            pixel[2] = static_cast<std::uint8_t>(gray);
        }
    }
}

} // namespace ip
