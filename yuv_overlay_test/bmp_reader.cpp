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
	std::ifstream in(path.c_str(), std::ios::binary);
	if (!in) {
		throw std::runtime_error("Failed to open BMP file: " + path);
	}

	BmpFileHeader fileHeader{};
	BmpInfoHeader infoHeader{};

	in.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	in.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

	if (!in) {
		throw std::runtime_error("Failed to read BMP headers");
	}

	if (fileHeader.bfType != 0x4D42) {
		throw std::runtime_error("Invalid BMP signature");
	}

	if (infoHeader.biBitCount != 24) {
		throw std::runtime_error("Only 24-bit BMP is supported");
	}

	if (infoHeader.biCompression != 0) {
		throw std::runtime_error("Compressed BMP is not supported");
	}

	if (infoHeader.biWidth <= 0 || infoHeader.biHeight == 0) {
		throw std::runtime_error("Invalid BMP dimensions");
	}

	const int width = infoHeader.biWidth;
	const int height = (infoHeader.biHeight > 0) ? infoHeader.biHeight : -infoHeader.biHeight;
	const bool bottomUp = (infoHeader.biHeight > 0);

	const int rowSize = ((width * 3 + 3) / 4) * 4;
	std::vector<uint8_t> row(rowSize);

	RgbImage image;
	image.width = width;
	image.height = height;
	image.data.resize(static_cast<size_t>(width) * height * 3);

	in.seekg(fileHeader.bfOffBits, std::ios::beg);
	if (!in) {
		throw std::runtime_error("Failed to seek to BMP pixel data");
	}

	for (int srcRow = 0; srcRow < height; ++srcRow) {
		in.read(reinterpret_cast<char*>(row.data()), rowSize);
		if (!in) {
			throw std::runtime_error("Failed to read BMP row");
		}
		const int dstRow = bottomUp ? (height - 1 - srcRow) : srcRow;

		for (int x = 0; x < width; ++x) {
			const uint8_t b = row[x * 3 + 0];
			const uint8_t r = row[x * 3 + 1];
			const uint8_t g = row[x * 3 + 2];

			const size_t idx = static_cast<size_t>(dstRow) * width * 3 + x * 3;
			image.data[idx + 0] = r;
			image.data[idx + 1] = g;
			image.data[idx + 2] = b;
		}
	}
	return image;
}