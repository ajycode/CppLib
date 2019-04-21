#pragma once

class Random
{
public:
	std::mt19937 mt;

	Random()
		: mt(1)
	{
	}

	Random(unsigned int seed)
		: mt(seed)
	{
	}

	// 设置种子的同时, 生成序列也将被重置. 相同的种子总会得到相同的序列
	inline void SetSeed(unsigned int seed)
	{
		mt.seed(seed);
	}

	template<typename T>
	inline T GetReal(T min, T max)
	{
		return std::uniform_real_distribution<T>(min, max)(mt);
	}

	template<typename T>
	inline T GetInt(T min, T max)
	{
		return std::uniform_int_distribution<T>(min, max)(mt);
	}

};
