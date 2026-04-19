#ifndef RGB_TO_YUV
#define RGB_TO_YUV

#include "bmp_reader.h"
#include "yuv420.h"

Yuv420Image convertRgbToYuv420(const RgbImage& rgb);

#endif // RGB_TO_YUV