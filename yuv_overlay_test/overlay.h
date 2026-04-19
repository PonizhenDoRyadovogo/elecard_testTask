#ifndef OVERLAY
#define OVERLAY

#include "yuv420.h"

void overlayYuv420(
    Yuv420Image& dst,
    const Yuv420Image& src,
    int offsetX,
    int offsetY
);

#endif // OVERLAY
