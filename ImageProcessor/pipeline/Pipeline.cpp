/**
 * @file Pipeline.cpp
 */

#include "Pipeline.h"
#include "../Exceptions.h"

#include <utility>

namespace ip {

Pipeline Pipeline::build(const std::vector<PipelineStep>& steps,
                         const FilterFactory& factory,
                         const ProgramOptions& options) {
    Pipeline pipeline;
    for (const PipelineStep& step : steps) {
        pipeline.add(factory.create(step.name, step.params, options));
    }
    return pipeline;
}

void Pipeline::add(std::unique_ptr<FilterBase> filter) {
    if (!filter) {
        throw FilterError("Pipeline: cannot add a null filter");
    }
    m_filters.push_back(std::move(filter));
}

void Pipeline::run(ImageBuffer& image) const {
    for (const std::unique_ptr<FilterBase>& filter : m_filters) {
        filter->apply(image);
    }
}

} // namespace ip
