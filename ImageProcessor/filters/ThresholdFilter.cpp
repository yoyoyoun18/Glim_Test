/**
 * @file ThresholdFilter.cpp
 */

#include "ThresholdFilter.h"
#include "../Exceptions.h"

#include <cstdint>

namespace ip {

namespace {
    // BT.601 가중치 x 256 (GrayscaleFilter 와 같은 값)
    constexpr int WEIGHT_R = 77;
    constexpr int WEIGHT_G = 150;
    constexpr int WEIGHT_B = 29;

    constexpr int SHIFT    = 8;
    constexpr int ROUNDING = 128;

    constexpr int MIN_THRESHOLD = 0;
    constexpr int MAX_THRESHOLD = 255;

    constexpr std::uint8_t WHITE = 255;
    constexpr std::uint8_t BLACK = 0;
} // anonymous namespace

ThresholdFilter::ThresholdFilter(int threshold)
    : m_threshold(threshold)
{
    if (threshold < MIN_THRESHOLD || threshold > MAX_THRESHOLD) {
        throw FilterError("threshold must be in [0, 255] (got " +
                          std::to_string(threshold) + ")");
    }
}

void ThresholdFilter::apply(ImageBuffer& image) {
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

            const int gray = (WEIGHT_R * r + WEIGHT_G * g + WEIGHT_B * b + ROUNDING) >> SHIFT;

            // OpenCV THRESH_BINARY 와 같은 기준: gray > threshold 이면 흰색
            const std::uint8_t value = (gray > m_threshold) ? WHITE : BLACK;

            pixel[0] = value;
            pixel[1] = value;
            pixel[2] = value;
        }
    }
}

std::string ThresholdFilter::name() const {
    return "threshold:" + std::to_string(m_threshold);
}

} // namespace ip
