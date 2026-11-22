/**
 * @file BrightnessContrastFilter.cpp
 */

#include "BrightnessContrastFilter.h"
#include "../Exceptions.h"

#include <cmath>
#include <sstream>

namespace ip {

namespace {
    constexpr int    MIN_BRIGHTNESS = -255;
    constexpr int    MAX_BRIGHTNESS = 255;
    constexpr double MIN_CONTRAST   = 0.0;
    constexpr double MAX_CONTRAST   = 10.0;

    constexpr int MID_VALUE = 128;  // 대비 조절의 기준점
    constexpr int MIN_PIXEL = 0;
    constexpr int MAX_PIXEL = 255;
} // anonymous namespace

BrightnessContrastFilter::BrightnessContrastFilter(int brightness, double contrast)
    : m_brightness(brightness), m_contrast(contrast)
{
    if (brightness < MIN_BRIGHTNESS || brightness > MAX_BRIGHTNESS) {
        throw FilterError("brightness must be in [-255, 255] (got " +
                          std::to_string(brightness) + ")");
    }
    // NaN 은 모든 비교가 false 이므로 "범위 안에 있지 않으면" 형태로 검사한다.
    if (!(contrast >= MIN_CONTRAST && contrast <= MAX_CONTRAST)) {
        throw FilterError("contrast must be in [0.0, 10.0] (got " +
                          std::to_string(contrast) + ")");
    }

    // 0 ~ 255 각 입력값에 대한 결과를 미리 계산한다.
    for (int i = 0; i < LUT_SIZE; ++i) {
        const double value = contrast * (i - MID_VALUE) + MID_VALUE + brightness;
        int rounded = static_cast<int>(std::lround(value));

        if (rounded < MIN_PIXEL) {
            rounded = MIN_PIXEL;
        }
        else if (rounded > MAX_PIXEL) {
            rounded = MAX_PIXEL;
        }

        m_lut[i] = static_cast<std::uint8_t>(rounded);
    }
}

void BrightnessContrastFilter::apply(ImageBuffer& image) {
    const int height = image.height();
    const int stride = image.rowStride();

    // 채널마다 같은 표를 적용하므로 B/G/R 구분 없이 행의 모든 바이트를 변환한다.
    for (int y = 0; y < height; ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int i = 0; i < stride; ++i) {
            row[i] = m_lut[row[i]];
        }
    }
}

std::string BrightnessContrastFilter::name() const {
    std::ostringstream oss;
    oss << "brightness:" << m_brightness << ":" << m_contrast;
    return oss.str();
}

} // namespace ip
