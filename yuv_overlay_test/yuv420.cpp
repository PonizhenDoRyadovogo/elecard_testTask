#include "yuv420.h"

size_t getYuv420FrameSize(int width, int height) {
	if (width <= 0 || height <= 0 || (width % 2) != 0 || (height % 2) != 0) {
		throw std::runtime_error("YUV420 requires positive even width and height");
	}
	return static_cast<size_t>(width * height * 3 / 2);
}

Yuv420Image createYuv420Image(int width, int height) {
	if (width <= 0 || height <= 0 || (width % 2) != 0 || (height % 2) != 0) {
		throw std::runtime_error("YUV420 requires positive even width and height");
	}

	Yuv420Image img;
	img.width = width;
	img.height = height;
	img.y.resize(static_cast<size_t>(width * height));
	img.u.resize(static_cast<size_t>((width / 2) * (height / 2)));
	img.v.resize(static_cast<size_t>((width / 2) * (height / 2)));
	return img;
}

bool readYuv420Frame(std::ifstream& in, Yuv420Image& frame) {
	const size_t ySize = static_cast<size_t>(frame.width * frame.height);
	const size_t uvSize = static_cast<size_t>((frame.width / 2) * (frame.height / 2));
	
	in.read(reinterpret_cast<char*>(frame.y.data()), ySize);
	if (in.gcount() == 0) {
		return false;
	}

	if (static_cast<size_t>(in.gcount()) != ySize) {
		throw std::runtime_error("Incomplete Y plane in input YUV");
	}

	in.read(reinterpret_cast<char*>(frame.u.data()), uvSize);
	if (static_cast<size_t>(in.gcount()) != uvSize) {
		throw std::runtime_error("Incomplete U plane in input YUV");
	}

	in.read(reinterpret_cast<char*>(frame.v.data()), uvSize);
	if (static_cast<size_t>(in.gcount()) != uvSize) {
		throw std::runtime_error("Incomplete V plane in input YUV");
	}

	return true;
}

void writeYuv420Frame(std::ostream& out, const Yuv420Image& frame) {
	const size_t ySize = static_cast<size_t>(frame.width * frame.height);
	const size_t uvSize = static_cast<size_t>((frame.width / 2) * (frame.height / 2));

	out.write(reinterpret_cast<const char*>(frame.y.data()), ySize);
	out.write(reinterpret_cast<const char*>(frame.u.data()), uvSize);
	out.write(reinterpret_cast<const char*>(frame.v.data()), uvSize);

	if (!out) {
		throw std::runtime_error("Failed to write YUV frame");
	}
}