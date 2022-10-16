#pragma once
#include <concepts>
#include <string>

#include "system/user_interface.h"

namespace vz
{
	//class UserInterface;

	class Application
	{
	public:
		int Run(UserInterface* ui_);
		void SetStorage(const std::string& filedir_);
	};
}