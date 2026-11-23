/**
 * @file Pipeline.cpp
 */

#include "Pipeline.h"
#include "../Exceptions.h"
#include "../logging/Stopwatch.h"

#include <string>
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

void Pipeline::run(ImageBuffer& image, Logger& logger) const {
    const std::string total = std::to_string(m_filters.size());

    for (std::size_t i = 0; i < m_filters.size(); ++i) {
        const FilterBase& filter = *m_filters[i];
        const std::string step = "Step " + std::to_string(i + 1) + "/" + total + " " + filter.name();

        const Stopwatch sw;
        try {
            m_filters[i]->apply(image);
        }
        catch (...) {
            logger.error(step + " FAILED (" + sw.elapsedText() + ")");
            throw;
        }
        logger.info(step + " OK (" + sw.elapsedText() + ")");
    }
}

} // namespace ip
