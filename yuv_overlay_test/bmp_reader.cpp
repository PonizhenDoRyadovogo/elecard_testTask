#include <fstream>
#include <stdexcept>

#include "bmp_reader.h"

#pragma pack(push, 1)
struct BmpFileHeader {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};
struct BmpInfoHeader {
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrsUsed;
	uint32_t biClrImportant;
};
#pragma pack(pop)

RgbImage readBmp24(const std::string& path) {

}