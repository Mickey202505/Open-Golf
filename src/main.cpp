#include <iostream>
#include <string>
#include <map>

// #include <opencv2/opencv.hpp>
// #include <curl/curl.h>

int main(int argc, char* argv[]) {
    // 1. Parse Command Line Arguments
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--") == 0 && i + 1 < argc) {
            args[arg.substr(2)] = argv[++i];
        }
    }

    // Check required args
    if (args.find("lat") == args.end() || args.find("lng") == args.end()) {
        std::cerr << "Error: Missing required coordinates (--lat, --lng)" << std::endl;
        return 1;
    }

    // Extract metadata with defaults
    std::string course = args.count("course") ? args["course"] : "UnknownCourse";
    std::string hole = args.count("hole") ? args["hole"] : "0";
    std::string feature = args.count("feature") ? args["feature"] : "bunker";
    std::string number = args.count("number") ? args["number"] : "1";

    // TODO: Step 1 - Fetch image with libcurl
    // TODO: Step 2 - Process image with OpenCV (Flood fill & Contours)
    // TODO: Step 3 - Translate pixel contours to World Space X/Y coordinates
    
    // 4. Output the SVG structure directly to stdout
    std::cout << "<svg xmlns=\"http://www.w3.org/2000/svg\">\n"
              << "  <g id=\"" << course << "-Hole-" << hole << "\">\n"
              << "    <path \n"
              << "       data-course=\"" << course << "\"\n"
              << "       data-hole=\"" << hole << "\"\n"
              << "       data-feature=\"" << feature << "\"\n"
              << "       data-number=\"" << number << "\"\n"
              << "       d=\"M 0 0 L 10 10 L 10 0 Z\" \n" // Placeholder world space coordinates
              << "       fill=\"none\" \n"
              << "       stroke=\"black\" />\n"
              << "  </g>\n"
              << "</svg>\n";

    return 0;
}
