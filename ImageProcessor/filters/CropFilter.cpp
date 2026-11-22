/**
 * @file CropFilter.cpp
 */

#include "CropFilter.h"
#include "../Exceptions.h"

#include <cstdint>
#include <cstring>
#include <utility>

namespace ip {

CropFilter::CropFilter(int x, int y, int width, int height)
    : m_x(x), m_y(y), m_width(width), m_height(height)
{
    if (x < 0 || y < 0) {
        throw FilterError("crop: x, y must be non-negative (got " +
                          std::to_string(x) + ", " + std::to_string(y) + ")");
    }
    if (width <= 0 || height <= 0) {
        throw FilterError("crop: --width and --height must be positive (got " +
                          std::to_string(width) + "x" + std::to_string(height) + ")");
    }
}

void CropFilter::apply(ImageBuffer& image) {
    // 이미지 크기는 apply 시점에만 알 수 있으므로 범위 검증은 여기서 한다.
    // 덧셈 대신 뺄셈으로 비교해 int 오버플로를 피한다.
    if (m_x >= image.width() || m_width > image.width() - m_x ||
        m_y >= image.height() || m_height > image.height() - m_y) {
        throw FilterError(
            "crop: region (" + std::to_string(m_x) + ", " + std::to_string(m_y) + ", " +
            std::to_string(m_width) + "x" + std::to_string(m_height) +
            ") is out of image bounds (" +
            std::to_string(image.width()) + "x" + std::to_string(image.height()) + ")");
    }

    ImageBuffer result(m_width, m_height);

    const std::size_t offset   = static_cast<std::size_t>(m_x) * ImageBuffer::CHANNELS;
    const std::size_t rowBytes = static_cast<std::size_t>(result.rowStride());

    // 원본의 각 행에서 잘라낼 구간만 복사한다. 행 안에서는 메모리가 연속이다.
    for (int y = 0; y < m_height; ++y) {
        const std::uint8_t* src = image.rowPtr(m_y + y) + offset;
        std::uint8_t*       dst = result.rowPtr(y);
        std::memcpy(dst, src, rowBytes);
    }

    image = std::move(result);
}

std::string CropFilter::name() const {
    return "crop:" + std::to_string(m_x) + ":" + std::to_string(m_y) + ":" +
           std::to_string(m_width) + ":" + std::to_string(m_height);
}

} // namespace ip
