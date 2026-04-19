#include "rgb_to_yuv.h"
#include "support_function.h"

#include <stdexcept>

namespace {
    inline uint8_t rgbToY(uint8_t r, uint8_t g, uint8_t b) {
        const int y = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b + 0.5);
        return clampToByte(y);
    }

    inline uint8_t rgbToU(uint8_t r, uint8_t g, uint8_t b) {
        const int u = static_cast<int>(-0.169 * r - 0.331 * g + 0.5 * b + 128.0 + 0.5);
        return clampToByte(u);
    }

    inline uint8_t rgbToV(uint8_t r, uint8_t g, uint8_t b) {
        const int v = static_cast<int>(0.5 * r - 0.419 * g - 0.081 * b + 128.0 + 0.5);
        return clampToByte(v);
    }
}

Yuv420Image convertRgbToYuv420(const RgbImage& rgb) {
    if (rgb.width <= 0 || rgb.height <= 0) {
        throw std::runtime_error("Invalid RGB image size");
    }
    if ((rgb.width % 2) != 0 || (rgb.height % 2) != 0) {
        throw std::runtime_error("RGB image dimensions must be even for YUV420");
    }
    if (rgb.data.size() != static_cast<size_t>(rgb.width) * rgb.height * 3) {
        throw std::runtime_error("Invalid RGB image buffer size");
    }

    Yuv420Image yuv = createYuv420Image(rgb.width, rgb.height);

    for (int j = 0; j < rgb.height; j += 2) {
        for (int i = 0; i < rgb.width; i += 2) {
            int uSum = 0;
            int vSum = 0;

            for (int dy = 0; dy < 2; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    const int x = i + dx;
                    const int y = j + dy;
                    const size_t rgbIdx = static_cast<size_t>(y) * rgb.width * 3 + x * 3;

                    const uint8_t r = rgb.data[rgbIdx + 0];
                    const uint8_t g = rgb.data[rgbIdx + 1];
                    const uint8_t b = rgb.data[rgbIdx + 2];

                    yuv.y[static_cast<size_t>(y) * rgb.width + x] = rgbToY(r, g, b);
                    uSum += rgbToU(r, g, b);
                    vSum += rgbToV(r, g, b);
                }
            }

            const size_t uvIdx = static_cast<size_t>(j / 2) * (rgb.width / 2) + (i / 2);
            yuv.u[uvIdx] = static_cast<uint8_t>(uSum / 4);
            yuv.v[uvIdx] = static_cast<uint8_t>(vSum / 4);
        }
    }

    return yuv;
}