/**
 * @file HistogramFilter.cpp
 */

#include "HistogramFilter.h"

#include <cstdint>
#include <iomanip>
#include <iostream>

namespace ip {

namespace {
    // BT.601 가중치 x 256 (GrayscaleFilter 와 같은 값)
    constexpr int WEIGHT_R = 77;
    constexpr int WEIGHT_G = 150;
    constexpr int WEIGHT_B = 29;

    constexpr int SHIFT    = 8;
    constexpr int ROUNDING = 128;

    constexpr int BIN_COUNT = 256;  // 밝기 0 ~ 255
    constexpr int BAR_WIDTH = 50;   // 가장 긴 막대의 길이 (문자 수)
} // anonymous namespace

void HistogramFilter::apply(ImageBuffer& image) {
    const int width  = image.width();
    const int height = image.height();

    // ── 1. 밝기별 픽셀 수 세기 ─────────────────────────
    std::int64_t counts[BIN_COUNT] = {};

    for (int y = 0; y < height; ++y) {
        const std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < width; ++x) {
            const std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            // ImageBuffer 의 채널 순서는 B, G, R
            const int b = pixel[0];
            const int g = pixel[1];
            const int r = pixel[2];

            const int gray = (WEIGHT_R * r + WEIGHT_G * g + WEIGHT_B * b + ROUNDING) >> SHIFT;
            ++counts[gray];
        }
    }

    // ── 2. 통계 (최소, 최대, 평균) ─────────────────────
    const std::int64_t totalPixels = static_cast<std::int64_t>(width) * height;

    int minValue = -1;
    int maxValue = -1;
    std::int64_t weightedSum = 0;

    for (int value = 0; value < BIN_COUNT; ++value) {
        if (counts[value] == 0) {
            continue;
        }
        if (minValue < 0) {
            minValue = value;
        }
        maxValue = value;
        weightedSum += value * counts[value];
    }

    const double mean = static_cast<double>(weightedSum) / static_cast<double>(totalPixels);

    // ── 3. 밝기 값마다 막대그래프 출력 (256줄) ─────────
    std::int64_t maxCount = 0;
    for (int value = 0; value < BIN_COUNT; ++value) {
        if (counts[value] > maxCount) {
            maxCount = counts[value];
        }
    }

    std::cout << "Histogram (gray, " << width << " x " << height
              << " = " << totalPixels << " px)\n";

    for (int value = 0; value < BIN_COUNT; ++value) {
        // maxCount 는 1 이상이다 (이미지는 최소 1픽셀).
        const int barLength = static_cast<int>(counts[value] * BAR_WIDTH / maxCount);

        std::cout << "  " << std::setw(3) << value << " | "
                  << std::left << std::setw(BAR_WIDTH) << std::string(barLength, '#')
                  << std::right << " " << std::setw(10) << counts[value] << "\n";
    }

    std::cout << "  min=" << minValue << "  max=" << maxValue
              << "  mean=" << std::fixed << std::setprecision(1) << mean
              << std::defaultfloat << "\n";
}

} // namespace ip
