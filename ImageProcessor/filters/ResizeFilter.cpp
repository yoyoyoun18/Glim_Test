/**
 * @file ResizeFilter.cpp
 */

#include "ResizeFilter.h"
#include "../Exceptions.h"

#include <cstdint>
#include <stdexcept>
#include <utility>

namespace ip {

ResizeFilter::ResizeFilter(int width, int height)
    : m_width(width), m_height(height)
{
    if (width <= 0 || height <= 0) {
        throw FilterError("resize: --width and --height must be positive (got " +
                          std::to_string(width) + "x" + std::to_string(height) + ")");
    }
}

void ResizeFilter::apply(ImageBuffer& image) {
    const int srcWidth  = image.width();
    const int srcHeight = image.height();

    ImageBuffer result;
    try {
        result = ImageBuffer(m_width, m_height);
    }
    catch (const std::invalid_argument& e) {  // ImageBuffer 의 최대 크기 초과
        throw FilterError(std::string("resize: ") + e.what());
    }

    for (int y = 0; y < m_height; ++y) {
        // 곱셈 결과가 int 범위를 넘지 않도록 64비트로 계산한다.
        // y < m_height 이므로 srcY 는 항상 srcHeight - 1 이하다.
        const int srcY = static_cast<int>(static_cast<std::int64_t>(y) * srcHeight / m_height);

        const std::uint8_t* srcRow = image.rowPtr(srcY);
        std::uint8_t*       dstRow = result.rowPtr(y);

        for (int x = 0; x < m_width; ++x) {
            const int srcX = static_cast<int>(static_cast<std::int64_t>(x) * srcWidth / m_width);

            const std::uint8_t* src = srcRow + srcX * ImageBuffer::CHANNELS;
            std::uint8_t*       dst = dstRow + x * ImageBuffer::CHANNELS;

            dst[0] = src[0];  // B
            dst[1] = src[1];  // G
            dst[2] = src[2];  // R
        }
    }

    image = std::move(result);
}

std::string ResizeFilter::name() const {
    return "resize:" + std::to_string(m_width) + ":" + std::to_string(m_height);
}

} // namespace ip
