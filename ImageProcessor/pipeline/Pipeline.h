#pragma once

/**
 * @file Pipeline.h
 * @brief 여러 필터를 순서대로 적용하는 파이프라인.
 */

#include "FilterFactory.h"
#include "PipelineParser.h"
#include "../CommandLineParser.h"
#include "../ImageBuffer.h"
#include "../filters/FilterBase.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace ip {

/**
 * @brief 필터 목록을 보관하고, run() 에서 추가된 순서대로 적용한다.
 *
 * 예시)
 *   "grayscale, brightness:20:1.2, threshold:128"
 *     → build() 가 필터 3개를 먼저 전부 만든다. (하나라도 잘못되면 여기서 실패)
 *     → run(image) 가 image 에 grayscale → brightness → threshold 순서로 적용한다.
 */
class Pipeline {
public:
    /**
     * @brief 단계 목록으로 파이프라인을 만든다.
     *
     * 이미지를 처리하기 전에 모든 단계의 필터를 생성해 보므로,
     * 잘못된 필터 이름이나 파라미터는 처리를 시작하기 전에 오류가 난다.
     *
     * @throws FilterError 단계 중 하나라도 필터 생성에 실패한 경우.
     */
    static Pipeline build(const std::vector<PipelineStep>& steps,
                          const FilterFactory& factory,
                          const ProgramOptions& options);

    /// 필터를 맨 뒤에 추가한다.
    void add(std::unique_ptr<FilterBase> filter);

    /// 추가된 순서대로 image 에 필터를 적용한다.
    void run(ImageBuffer& image) const;

    std::size_t size() const { return m_filters.size(); }

private:
    std::vector<std::unique_ptr<FilterBase>> m_filters;
};

} // namespace ip
