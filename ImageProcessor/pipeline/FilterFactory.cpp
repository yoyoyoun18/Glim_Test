/**
 * @file FilterFactory.cpp
 */

#include "FilterFactory.h"
#include "../Exceptions.h"

#include "../filters/GrayscaleFilter.h"
#include "../filters/FlipFilter.h"
#include "../filters/ThresholdFilter.h"
#include "../filters/BrightnessContrastFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/HistogramFilter.h"

#include <utility>

namespace ip {

FilterFactory::FilterFactory() {
    // ── 기본 제공 필터 등록 (새 필터는 여기에 한 줄 추가) ──
    registerFilter("grayscale", [](const ProgramOptions&) {
        return std::make_unique<GrayscaleFilter>();
    });
    registerFilter("flip:h", [](const ProgramOptions&) {
        return std::make_unique<FlipFilter>(FlipFilter::Direction::Horizontal);
    });
    registerFilter("flip:v", [](const ProgramOptions&) {
        return std::make_unique<FlipFilter>(FlipFilter::Direction::Vertical);
    });
    registerFilter("threshold", [](const ProgramOptions& o) {
        return std::make_unique<ThresholdFilter>(o.threshold);
    });
    registerFilter("brightness", [](const ProgramOptions& o) {
        return std::make_unique<BrightnessContrastFilter>(o.brightness, o.contrast);
    });
    registerFilter("crop", [](const ProgramOptions& o) {
        return std::make_unique<CropFilter>(o.x, o.y, o.width, o.height);
    });
    registerFilter("resize", [](const ProgramOptions& o) {
        return std::make_unique<ResizeFilter>(o.width, o.height);
    });
    registerFilter("histogram", [](const ProgramOptions&) {
        return std::make_unique<HistogramFilter>();
    });
}

void FilterFactory::registerFilter(const std::string& name, Creator creator) {
    m_creators[name] = std::move(creator);
}

std::unique_ptr<FilterBase> FilterFactory::create(const std::string& name,
                                                  const ProgramOptions& options) const {
    const auto it = m_creators.find(name);
    if (it == m_creators.end()) {
        throw FilterError("Unknown filter: " + name);
    }
    return it->second(options);
}

} // namespace ip
