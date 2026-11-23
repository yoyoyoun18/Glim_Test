/**
 * @file main.cpp
 * @brief ImageProcessor 진입점 — 지원자가 작성해야 할 파일입니다.
 *
 * BMP 입출력과 커맨드라인 파싱은 제공된 코드가 처리합니다.
 * 본 과제에서 작성해야 할 것은 단 하나입니다:
 *
 *     ▶ 이미지 처리 필터 2개 이상 구현 + main 의 TODO 위치에 연결
 *
 * 또한 일관된 컨벤션과 예외 처리, 메모리 안정성도 함께 평가됩니다.
 */

#include "BmpParser.h"
#include "CommandLineParser.h"
#include "ImageBuffer.h"
#include "Exceptions.h"

#include <iostream>
#include <string>
#include <vector>

#include "pipeline/FilterFactory.h"
#include "pipeline/Pipeline.h"
#include "pipeline/PipelineParser.h"
#include "logging/Logger.h"
#include "logging/Stopwatch.h"

namespace {
    /// 실패 내용과 종료 코드를 로그에 남기고 종료 코드를 그대로 돌려준다.
    int logFailure(ip::Logger& logger, const std::string& message, int exitCode) {
        logger.error(message);
        logger.error("Result: FAILED (exit code " + std::to_string(exitCode) + ")");
        return exitCode;
    }
}

int main(int argc, char* argv[]) {
    // 인자 파싱 실패도 기록할 수 있도록 로거는 try 밖에서 먼저 만든다.
    ip::Logger logger;
    const ip::Stopwatch totalTime;

    try {
        // ── CLI 인자 파싱 (제공된 코드) ─────────────────────────
        const ip::ProgramOptions options = ip::CommandLineParser::parse(argc, argv);
        logger.info("Start: input=" + options.inputPath + " output=" + options.outputPath);

        // ── BMP 로드 (제공된 코드) ──────────────────────────────
        const ip::Stopwatch loadTime;
        ip::ImageBuffer image = ip::BmpParser::loadFromFile(options.inputPath);
        std::cout << "Loaded: " << image.width() << " x " << image.height() << "\n";
        logger.info("Loaded: " + std::to_string(image.width()) + " x " +
                    std::to_string(image.height()) + " (" + loadTime.elapsedText() + ")");

        // ── 필터 적용 ───────────────────────────────────────────
        // --filter 는 단계 하나짜리 파이프라인으로 처리한다.
        // (예: "threshold:128" → threshold + {"128"})
        const bool usePipeline = !options.pipeline.empty();
        const std::string& pipelineText = usePipeline ? options.pipeline : options.filterName;

        const std::vector<ip::PipelineStep> steps = ip::PipelineParser::parse(pipelineText);
        if (!usePipeline && steps.size() != 1) {
            throw ip::ArgumentError("--filter takes a single filter (use --pipeline for several): " +
                                    options.filterName);
        }

        const ip::FilterFactory factory;
        const ip::Pipeline pipeline = ip::Pipeline::build(steps, factory, options);
        pipeline.run(image, logger);
        std::cout << "Applied: " << pipelineText << "\n";

        // ── BMP 저장 (제공된 코드) ──────────────────────────────
        const ip::Stopwatch saveTime;
        ip::BmpParser::saveToFile(options.outputPath, image);
        std::cout << "Saved:  " << options.outputPath << "\n";
        logger.info("Saved: " + options.outputPath + " (" + saveTime.elapsedText() + ")");

        logger.info("Result: SUCCESS (total " + totalTime.elapsedText() + ")");
        return 0;
    }
    catch (const ip::ArgumentError& e) {
        std::cerr << e.what() << "\n\n";
        ip::CommandLineParser::printUsage(argc > 0 ? argv[0] : "ImageProcessor");
        return logFailure(logger, e.what(), 4);
    }
    catch (const ip::BmpParseError& e) {
        std::cerr << e.what() << std::endl;
        return logFailure(logger, e.what(), 2);
    }
    catch (const ip::FilterError& e) {
        std::cerr << e.what() << std::endl;
        return logFailure(logger, e.what(), 3);
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return logFailure(logger, std::string("Unexpected error: ") + e.what(), 1);
    }
}
