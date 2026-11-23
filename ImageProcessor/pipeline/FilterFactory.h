#pragma once

/**
 * @file FilterFactory.h
 * @brief 필터 이름으로 필터 객체를 만들어 주는 팩토리.
 */

#include "../CommandLineParser.h"
#include "../filters/FilterBase.h"

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace ip {

/**
 * @brief 필터 이름 → 생성 함수 매핑을 보관하고, 이름으로 필터를 생성한다.
 *
 * 사용하는 쪽(main)은 구체 필터 클래스를 몰라도 되고,
 * FilterBase 포인터로만 필터를 다룬다.
 *
 * 새 필터를 추가할 때는 필터 클래스를 작성하고
 * FilterFactory.cpp 의 생성자에 registerFilter() 한 줄만 추가한다.
 */
class FilterFactory {
public:
    /// 필터 생성 함수. 필터 파라미터는 CLI 옵션에서 읽는다.
    using Creator = std::function<std::unique_ptr<FilterBase>(const ProgramOptions&)>;

    /// 기본 제공 필터들을 등록한다.
    FilterFactory();

    /// 이름과 생성 함수를 등록한다. 같은 이름이면 덮어쓴다.
    void registerFilter(const std::string& name, Creator creator);

    /**
     * @brief 이름에 맞는 필터를 생성한다.
     * @throws FilterError 등록되지 않은 이름이거나, 필터 파라미터가 잘못된 경우.
     */
    std::unique_ptr<FilterBase> create(const std::string& name,
                                       const ProgramOptions& options) const;

private:
    std::map<std::string, Creator> m_creators;
};

} // namespace ip
