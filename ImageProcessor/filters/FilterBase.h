#pragma once

/**
 * @file FilterBase.h
 * @brief 모든 이미지 필터의 공통 인터페이스.
 */

#include "../ImageBuffer.h"

#include <string>

namespace ip {

/**
 * @brief 이미지 필터 추상 클래스.
 *
 * 새 필터는 이 클래스를 상속하여 apply() 와 name() 을 구현한다.
 * 파라미터 검증은 각 필터의 생성자에서 수행하고,
 * 잘못된 값이면 FilterError 를 던진다.
 */
class FilterBase {
public:
    virtual ~FilterBase() = default;

    /**
     * @brief 이미지에 필터를 적용한다. 결과는 image 에 덮어쓴다.
     *
     * 크롭/리사이즈처럼 크기가 바뀌는 필터는 내부에서 새 버퍼를 만든 뒤
     * image 에 대입(move)한다.
     */
    virtual void apply(ImageBuffer& image) = 0;

    /// 필터 이름 (로그, 에러 메시지용).
    virtual std::string name() const = 0;
};

} // namespace ip
