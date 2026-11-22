/**
 * @file CommandLineParser.cpp
 */

#include "CommandLineParser.h"
#include "Exceptions.h"

#include <iostream>
#include <string>

namespace ip {

namespace {
    /// argv 의 다음 인자를 안전하게 가져온다.
    std::string nextArg(int argc, char* argv[], int& i, const std::string& flag) {
        if (i + 1 >= argc) {
            throw ArgumentError(flag + ": missing value");
        }
        return argv[++i];
    }

    /// 문자열 전체가 정수인지 확인하고 변환한다. ("12abc" 같은 값은 거부)
    int toInt(const std::string& value, const std::string& flag) {
        try {
            std::size_t pos = 0;
            const int result = std::stoi(value, &pos);
            if (pos != value.size()) {
                throw ArgumentError(flag + ": not an integer: " + value);
            }
            return result;
        }
        catch (const std::logic_error&) {  // std::invalid_argument, std::out_of_range
            throw ArgumentError(flag + ": not an integer: " + value);
        }
    }

    /// 문자열 전체가 실수인지 확인하고 변환한다. ("1.5x" 같은 값은 거부)
    double toDouble(const std::string& value, const std::string& flag) {
        try {
            std::size_t pos = 0;
            const double result = std::stod(value, &pos);
            if (pos != value.size()) {
                throw ArgumentError(flag + ": not a number: " + value);
            }
            return result;
        }
        catch (const std::logic_error&) {  // std::invalid_argument, std::out_of_range
            throw ArgumentError(flag + ": not a number: " + value);
        }
    }
} // anonymous namespace

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {
    ProgramOptions options;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--input" || arg == "-i") {
            options.inputPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--output" || arg == "-o") {
            options.outputPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--filter" || arg == "-f") {
            options.filterName = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--threshold" || arg == "-t") {
            options.threshold = toInt(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--brightness" || arg == "-b") {
            options.brightness = toInt(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--contrast" || arg == "-c") {
            options.contrast = toDouble(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--x") {
            options.x = toInt(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--y") {
            options.y = toInt(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--width") {
            options.width = toInt(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--height") {
            options.height = toInt(nextArg(argc, argv, i, arg), arg);
        }
        else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            std::exit(0);
        }
        else {
            throw ArgumentError("Unknown option: " + arg);
        }
    }

    // 필수 인자 검증
    if (options.inputPath.empty()) {
        throw ArgumentError("--input is required");
    }
    if (options.outputPath.empty()) {
        throw ArgumentError("--output is required");
    }
    if (options.filterName.empty()) {
        throw ArgumentError("--filter is required");
    }

    return options;
}

void CommandLineParser::printUsage(const std::string& exeName) {
    std::cout
        << "Usage:\n"
        << "  " << exeName << " --input <path> --output <path> --filter <name>\n\n"
        << "Options:\n"
        << "  -i, --input   <path>   Input BMP file (24-bit, uncompressed)\n"
        << "  -o, --output  <path>   Output BMP file\n"
        << "  -f, --filter  <name>   Filter to apply (grayscale, flip:h, flip:v, threshold, brightness, crop)\n"
        << "  -t, --threshold <0-255> Threshold value for 'threshold' filter (default 128)\n"
        << "  -b, --brightness <-255-255> Brightness for 'brightness' filter (default 0)\n"
        << "  -c, --contrast <0.0-10.0>   Contrast for 'brightness' filter (default 1.0)\n"
        << "      --x, --y <int>          Top-left corner for 'crop' filter (default 0, 0)\n"
        << "      --width, --height <int> Output size for 'crop' filter\n"
        << "  -h, --help             Show this message\n\n"
        << "Examples:\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f grayscale\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f threshold -t 128\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f brightness -b 20 -c 1.2\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f crop --x 10 --y 20 --width 100 --height 80\n";
}

} // namespace ip
