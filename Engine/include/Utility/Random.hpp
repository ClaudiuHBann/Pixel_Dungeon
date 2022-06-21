#pragma once

#include <random>

class Random {
public:
	Random() {
		mt.seed(rd());
	}
	~Random() {}

	inline uint32_t GetUInt32(const uint32_t min, const uint32_t max) {
		return std::uniform_int_distribution<std::mt19937::result_type>(min, max)(mt);
	}

private:
	std::random_device rd;
	std::mt19937 mt;
};