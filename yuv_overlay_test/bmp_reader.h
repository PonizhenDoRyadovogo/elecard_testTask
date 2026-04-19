#ifndef BMP_READER
#define BMP_READER

#include <cstdint>
#include <string>
#include <vector>

struct RgbImage {
	int width;
	int height;
	std::vector<uint8_t> data;

	RgbImage() : width(0), height(0) {}
};

RgbImage readBmp24(const std::string& path);

#endif // BMP_READER
