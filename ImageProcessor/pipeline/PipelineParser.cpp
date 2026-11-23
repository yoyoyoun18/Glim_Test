/**
 * @file PipelineParser.cpp
 */

#include "PipelineParser.h"
#include "../Exceptions.h"

namespace ip {

namespace {
    bool isSpace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    /// 앞뒤 공백을 잘라낸 문자열을 돌려준다.
    std::string trim(const std::string& s) {
        std::size_t begin = 0;
        while (begin < s.size() && isSpace(s[begin])) {
            ++begin;
        }
        std::size_t end = s.size();
        while (end > begin && isSpace(s[end - 1])) {
            --end;
        }
        return s.substr(begin, end - begin);
    }

    /// 구분자로 나눈다. 빈 조각도 그대로 남긴다. ("a,,b" → "a", "", "b")
    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> parts;
        std::size_t start = 0;
        while (true) {
            const std::size_t pos = s.find(delimiter, start);
            if (pos == std::string::npos) {
                parts.push_back(s.substr(start));
                break;
            }
            parts.push_back(s.substr(start, pos - start));
            start = pos + 1;
        }
        return parts;
    }
}

std::vector<PipelineStep> PipelineParser::parse(const std::string& text) {
    if (trim(text).empty()) {
        throw ArgumentError("pipeline is empty");
    }

    std::vector<PipelineStep> steps;
    for (const std::string& rawStep : split(text, ',')) {
        const std::string stepText = trim(rawStep);
        if (stepText.empty()) {
            throw ArgumentError("pipeline has an empty step: \"" + text + "\"");
        }

        const std::vector<std::string> tokens = split(stepText, ':');

        PipelineStep step;
        step.name = trim(tokens[0]);
        if (step.name.empty()) {
            throw ArgumentError("pipeline step has no filter name: \"" + stepText + "\"");
        }
        for (std::size_t i = 1; i < tokens.size(); ++i) {
            const std::string param = trim(tokens[i]);
            if (param.empty()) {
                throw ArgumentError("pipeline step has an empty parameter: \"" + stepText + "\"");
            }
            step.params.push_back(param);
        }
        steps.push_back(step);
    }
    return steps;
}

} // namespace ip
