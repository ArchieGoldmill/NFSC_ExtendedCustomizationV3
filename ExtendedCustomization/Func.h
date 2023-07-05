#pragma once
#define FUNC(address, return_t, callconv, name, ...) return_t (callconv* name)(__VA_ARGS__) = reinterpret_cast<decltype(name)>(address)
#define INLINE_FUNC(address, return_t, callconv, name, ...) inline return_t (callconv* name)(__VA_ARGS__) = reinterpret_cast<decltype(name)>(address)

inline void MoveTowards(float& a, float b, float step)
{
	if (a < b)
	{
		a += step;
		if (a > b)
		{
			a = b;
		}
	}

	if (a > b)
	{
		a -= step;
		if (a < b)
		{
			a = b;
		}
	}
}