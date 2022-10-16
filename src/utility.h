#pragma once
#include <iostream>


namespace vz
{
	template<typename... Args>
	static void print(Args&&... args_)
	{
		((std::cout << std::forward<Args>(args_)), ...);
	}

	template<typename... Args>
	static void println(Args&&... args_)
	{
		print(std::forward<Args>(args_)..., "\n");
	}
}
