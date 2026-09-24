#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <curl/curl.h>

// #include <opencv2/opencv.hpp>

// Libcurl callback to write downloaded data into a memory buffer
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::vector<unsigned char>* mem = (std::vector<unsigned char>*)userp;
    
    // Append new data to the vector
    auto* data = static_cast<unsigned char*>(contents);
    mem->insert(mem->end(), data, data + realsize);
    
    return realsize;
}

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
    if (args.find("lat") == args.end() || args.find("lng") == args.end() || args.find("apikey") == args.end()) {
        std::cerr << "Error: Missing required args (--lat, --lng, --apikey)" << std::endl;
        return 1;
    }

    std::string lat = args["lat"];
    std::string lng = args["lng"];
    std::string apikey = args["apikey"];
    std::string zoom = args.count("zoom") ? args["zoom"] : "19";

    // Extract metadata with defaults
    std::string course = args.count("course") ? args["course"] : "UnknownCourse";
    std::string hole = args.count("hole") ? args["hole"] : "0";
    std::string feature = args.count("feature") ? args["feature"] : "bunker";
    std::string number = args.count("number") ? args["number"] : "1";

    // Step 1 - Fetch image with libcurl
    std::string url = "https://maps.googleapis.com/maps/api/staticmap?center=" 
                      + lat + "," + lng + "&zoom=" + zoom 
                      + "&size=512x512&maptype=satellite&key=" + apikey;

    std::vector<unsigned char> image_data;

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Tell curl to send the data to our callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        // Pass our vector to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&image_data);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl error: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_cleanup(curl);
    }

    // Test print to stderr (so it doesn't mess up our SVG stdout later)
    std::cerr << "Success! Downloaded " << image_data.size() << " bytes of satellite imagery." << std::endl;

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
              << "       d=\"M 0 0 L 10 10 L 10 0 Z\" \n" 
              << "       fill=\"none\" \n"
              << "       stroke=\"black\" />\n"
              << "  </g>\n"
              << "</svg>\n";

    return 0;
}
