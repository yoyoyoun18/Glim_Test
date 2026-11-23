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

#include <stdexcept>
#include <string>
#include <utility>

namespace ip {

namespace {
    /// 파라미터 개수가 허용된 값(a 또는 b) 중 하나인지 확인한다.
    void expectCount(const std::string& filter, const FilterFactory::Params& params,
                     std::size_t a, std::size_t b) {
        const std::size_t n = params.size();
        if (n != a && n != b) {
            std::string expected = std::to_string(a);
            if (b != a) {
                expected += " or " + std::to_string(b);
            }
            throw FilterError(filter + ": expected " + expected +
                              " parameter(s), got " + std::to_string(n));
        }
    }

    void expectCount(const std::string& filter, const FilterFactory::Params& params,
                     std::size_t count) {
        expectCount(filter, params, count, count);
    }

    /// 문자열 전체가 정수인지 확인하고 변환한다. ("12abc" 같은 값은 거부)
    int toInt(const std::string& filter, const std::string& value) {
        try {
            std::size_t pos = 0;
            const int result = std::stoi(value, &pos);
            if (pos == value.size()) {
                return result;
            }
        }
        catch (const std::logic_error&) {
            // 아래에서 FilterError 로 처리
        }
        throw FilterError(filter + ": not an integer: " + value);
    }

    /// 문자열 전체가 실수인지 확인하고 변환한다.
    double toDouble(const std::string& filter, const std::string& value) {
        try {
            std::size_t pos = 0;
            const double result = std::stod(value, &pos);
            if (pos == value.size()) {
                return result;
            }
        }
        catch (const std::logic_error&) {
            // 아래에서 FilterError 로 처리
        }
        throw FilterError(filter + ": not a number: " + value);
    }
}

FilterFactory::FilterFactory() {
    // ── 기본 제공 필터 등록 (새 필터는 여기에 한 줄 추가) ──
    // p: 단계 파라미터, o: CLI 옵션. 파라미터가 없으면 CLI 옵션 값을 쓴다.

    registerFilter("grayscale", [](const Params& p, const ProgramOptions&) {
        expectCount("grayscale", p, 0);
        return std::make_unique<GrayscaleFilter>();
    });

    // flip:h / flip:v
    registerFilter("flip", [](const Params& p, const ProgramOptions&) {
        expectCount("flip", p, 1);
        if (p[0] == "h") {
            return std::make_unique<FlipFilter>(FlipFilter::Direction::Horizontal);
        }
        if (p[0] == "v") {
            return std::make_unique<FlipFilter>(FlipFilter::Direction::Vertical);
        }
        throw FilterError("flip: direction must be h or v (got " + p[0] + ")");
    });

    // threshold / threshold:128
    registerFilter("threshold", [](const Params& p, const ProgramOptions& o) {
        expectCount("threshold", p, 0, 1);
        const int t = p.empty() ? o.threshold : toInt("threshold", p[0]);
        return std::make_unique<ThresholdFilter>(t);
    });

    // brightness / brightness:20 / brightness:20:1.2
    registerFilter("brightness", [](const Params& p, const ProgramOptions& o) {
        if (p.size() > 2) {
            throw FilterError("brightness: expected 0 to 2 parameter(s), got " +
                              std::to_string(p.size()));
        }
        const int    b = p.size() >= 1 ? toInt("brightness", p[0])    : o.brightness;
        const double c = p.size() >= 2 ? toDouble("brightness", p[1]) : o.contrast;
        return std::make_unique<BrightnessContrastFilter>(b, c);
    });

    // crop / crop:x:y:w:h
    registerFilter("crop", [](const Params& p, const ProgramOptions& o) {
        expectCount("crop", p, 0, 4);
        if (p.empty()) {
            return std::make_unique<CropFilter>(o.x, o.y, o.width, o.height);
        }
        return std::make_unique<CropFilter>(toInt("crop", p[0]), toInt("crop", p[1]),
                                            toInt("crop", p[2]), toInt("crop", p[3]));
    });

    // resize / resize:w:h
    registerFilter("resize", [](const Params& p, const ProgramOptions& o) {
        expectCount("resize", p, 0, 2);
        if (p.empty()) {
            return std::make_unique<ResizeFilter>(o.width, o.height);
        }
        return std::make_unique<ResizeFilter>(toInt("resize", p[0]), toInt("resize", p[1]));
    });

    registerFilter("histogram", [](const Params& p, const ProgramOptions&) {
        expectCount("histogram", p, 0);
        return std::make_unique<HistogramFilter>();
    });
}

void FilterFactory::registerFilter(const std::string& name, Creator creator) {
    m_creators[name] = std::move(creator);
}

std::unique_ptr<FilterBase> FilterFactory::create(const std::string& name,
                                                  const Params& params,
                                                  const ProgramOptions& options) const {
    const auto it = m_creators.find(name);
    if (it == m_creators.end()) {
        throw FilterError("Unknown filter: " + name);
    }
    return it->second(params, options);
}

} // namespace ip
