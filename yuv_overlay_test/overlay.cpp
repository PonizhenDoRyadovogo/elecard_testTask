#include <stdexcept>

#include "overlay.h"

void overlayYuv420(
    Yuv420Image& dst,
    const Yuv420Image& src,
    int offsetX,
    int offsetY
) {
    if ((offsetX % 2) != 0 || (offsetY % 2) != 0) {
        throw std::runtime_error("Overlay position must be even for YUV420");
    }

    if (offsetX < 0 || offsetY < 0) {
        throw std::runtime_error("Overlay position must be non-negative");
    }

    if (offsetX + src.width > dst.width || offsetY + src.height > dst.height) {
        throw std::runtime_error("Overlay image is out of destination bounds");
    }

    for (int j = 0; j < src.height; ++j) {
        for (int i = 0; i < src.width; ++i) {
            const size_t dstIdx = static_cast<size_t>(offsetY + j) * dst.width + (offsetX + i);
            const size_t srcIdx = static_cast<size_t>(j) * src.width + i;
            dst.y[dstIdx] = src.y[srcIdx];
        }
    }

    const int dstUvWidth = dst.width / 2;
    const int srcUvWidth = src.width / 2;

    for (int j = 0; j < src.height / 2; ++j) {
        for (int i = 0; i < src.width / 2; ++i) {
            const size_t dstIdx = static_cast<size_t>(offsetY / 2 + j) * dstUvWidth + (offsetX / 2 + i);
            const size_t srcIdx = static_cast<size_t>(j) * srcUvWidth + i;

            dst.u[dstIdx] = src.u[srcIdx];
            dst.v[dstIdx] = src.v[srcIdx];
        }
    }
}