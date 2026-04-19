#include "bmp_reader.h"
#include "overlay.h"
#include "rgb_to_yuv.h"
#include "yuv420.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    try {
        if (argc != 8) {
            std::cerr << "Usage:\n";
            std::cerr << argv[0] << " <input.yuv> <width> <height> <overlay.bmp> <output.yuv> <x> <y>\n";
            return 1;
        }

        const std::string inputYuvPath = argv[1];
        const int videoWidth = std::stoi(argv[2]);
        const int videoHeight = std::stoi(argv[3]);
        const std::string bmpPath = argv[4];
        const std::string outputYuvPath = argv[5];
        const int overlayX = std::stoi(argv[6]);
        const int overlayY = std::stoi(argv[7]);

        if ((videoWidth % 2) != 0 || (videoHeight % 2) != 0) {
            throw std::runtime_error("Video width and height must be even");
        }

        RgbImage bmp = readBmp24(bmpPath);
        Yuv420Image overlay = convertRgbToYuv420(bmp);

        std::ifstream in(inputYuvPath.c_str(), std::ios::binary);
        if (!in) {
            throw std::runtime_error("Failed to open input YUV file");
        }

        std::ofstream out(outputYuvPath.c_str(), std::ios::binary);
        if (!out) {
            throw std::runtime_error("Failed to open output YUV file");
        }

        Yuv420Image frame = createYuv420Image(videoWidth, videoHeight);

        size_t frameCount = 0;
        while (readYuv420Frame(in, frame)) {
            overlayYuv420(frame, overlay, overlayX, overlayY);
            writeYuv420Frame(out, frame);
            ++frameCount;
        }

        std::cout << "Done. Processed frames: " << frameCount << "\n";
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}