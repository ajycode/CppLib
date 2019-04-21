#pragma once

namespace Ajy
{

	template<class T>
	size_t GetHashCode(T const& in)
	{
		return in.GetHashCode();
	};
	template<class T>
	size_t GetHashCode(T* in)
	{
		return GetHashCode(*in);
	};

	template<class T>
	bool Equals(T const& a, T const& b)
	{
		return a == b;
	};
	template<class T>
	bool Equals(T* a, T* b)
	{
		return *a == *b;
	};
	template<class T>
	void Swap(T& left, T& right)
	{
		T tmp = left;
		left = right;
		right = tmp;
	}

}
