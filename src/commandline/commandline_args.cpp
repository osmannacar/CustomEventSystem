#include "commandline_args.h"

CommandLineArgs::CommandLineArgs(int argc, char *argv[]) {
    parseArguments(argc, argv);
}

std::string CommandLineArgs::getModelPath() const {
    return modelPath;
}

std::string CommandLineArgs::getVideoPath() const {
    return videoPath;
}

double CommandLineArgs::getConfidenceThreshold() const {
    return confidenceThreshold;
}

bool CommandLineArgs::validateArguments() const {
    return validatePath(modelPath) && validatePath(videoPath);
}

void CommandLineArgs::printUsage(const char *programName) {
    std::cerr << "Usage: " << programName << " --modelPath:<path> --videoPath:<path> --threshold:<value>" << std::endl;
}

void CommandLineArgs::parseArguments(int argc, char *argv[]) {
    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        size_t pos = arg.find(':');
        if (pos != std::string::npos) {
            std::string key = arg.substr(0, pos);
            std::string value = arg.substr(pos + 1);
            args[key] = value;
        }
    }

    // Check for required arguments
    if (args.find("--modelPath") != args.end()) {
        modelPath = args["--modelPath"];
    }
    if (args.find("--videoPath") != args.end()) {
        videoPath = args["--videoPath"];
    }
    if (args.find("--threshold") != args.end()) {
        try {
            confidenceThreshold = std::stod(args["--threshold"]);
            std::cout << confidenceThreshold << " is a valid threshold." << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid threshold value." << std::endl;
            confidenceThreshold = 0.3; // Default to 0.3 if invalid
        }
    }
}

bool CommandLineArgs::validatePath(const std::string &path) const {
    if (isValidUrl(path)) {
        std::cout << path << " is a valid URL." << std::endl;
        return true;
    } else {
        if (fileExists(path)) {
            std::cout << path << " is a valid file." << std::endl;
            return true;
        } else {
            std::cerr << "Error: path(" << path << ") does not exist." << std::endl;
            return false;
        }
    }
}

bool CommandLineArgs::isValidUrl(const std::string &url) {
    const std::regex urlPattern(R"(^(https?|ftp)://[^\s/$.?#].[^\s]*$)");
    return std::regex_match(url, urlPattern);
}

bool CommandLineArgs::fileExists(const std::string &path) {
    return std::filesystem::exists(path);
}
