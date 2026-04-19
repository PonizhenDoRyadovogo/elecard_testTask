#include "support_function.h"

uint8_t clampToByte(int value) {
	if (value < 0) {
		return 0;
	}
	if (value > 255) {
		return 255;
	}
	return static_cast<uint8_t>(value);
}