#include "application.h"

namespace vz
{
	int Application::Run(UserInterface* ui_)
	{
		ui_->DrawLoginMenu();
		return 0;
	}

	void Application::SetStorage(const std::string& filedir_)
	{

	}
}
