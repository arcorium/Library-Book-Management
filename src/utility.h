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

	template<typename... Args>
	static void print_nice(Args&&... args_)
	{
		print("[+] ", std::forward<Args>(args_)...);
	}

	template<typename... Args>
	static void println_nice(Args&&... args_)
	{
		println("[+] ", std::forward<Args>(args_)...);
	}

	static void clear()
	{
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
	}
}
