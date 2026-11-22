#pragma once

/**
 * @file BrightnessContrastFilter.h
 * @brief 밝기와 대비를 조절하는 필터.
 */

#include "FilterBase.h"

#include <array>
#include <cstdint>

namespace ip {

/**
 * @brief 밝기/대비 조절을 수행한다.
 *
 *   out = contrast * (p - 128) + 128 + brightness
 *
 * 중간 밝기(128)를 기준으로 대비를 조절하므로 대비만 바꿔도
 * 이미지 전체 밝기가 크게 변하지 않는다.
 * 결과는 0 ~ 255 로 클램핑하며, B/G/R 채널에 각각 적용한다.
 *
 * 입력값은 0 ~ 255 뿐이므로 생성자에서 256칸 LUT 를 미리 계산하고
 * apply() 에서는 표를 조회만 한다.
 */
class BrightnessContrastFilter final : public FilterBase {
public:
    /**
     * @param brightness 밝기 (-255 ~ 255, 0 이면 변화 없음)
     * @param contrast   대비 (0.0 ~ 10.0, 1.0 이면 변화 없음)
     * @throws FilterError 범위를 벗어난 경우.
     */
    BrightnessContrastFilter(int brightness, double contrast);

    void apply(ImageBuffer& image) override;
    std::string name() const override;

private:
    static constexpr int LUT_SIZE = 256;

    int    m_brightness;
    double m_contrast;
    std::array<std::uint8_t, LUT_SIZE> m_lut{};
};

} // namespace ip
