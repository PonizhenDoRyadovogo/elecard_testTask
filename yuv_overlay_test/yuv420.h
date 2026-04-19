#ifndef YUV420
#define YUV420

#include <cstdint>
#include <fstream>
#include <vector>

struct Yuv420Image {
	int width;
	int height;
	std::vector<uint8_t> y;
	std::vector<uint8_t> u;
	std::vector<uint8_t> v;

	Yuv420Image() : width(0), height(0) {}
};

size_t getYuv420FrameSize(int width, int height);
Yuv420Image createYuv420Image(int width, int height);

bool readYuv420Frame(std::ifstream& in, Yuv420Image& frame);
void writeYuv420Frame(std::ostream& out, const Yuv420Image& frame);

#endif // YUV420
