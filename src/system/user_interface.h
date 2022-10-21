#pragma once
#include <string>

namespace vz
{
	enum class State;
	class Application;

	class UserInterface
	{
	public:
		virtual ~UserInterface() = default;

		virtual void SetApplication(Application* app_) = 0;

		virtual void DrawErrorMessage(std::string&& header_, const std::string& message_) = 0;

		// Drawing
		virtual void DrawLoginMenu() = 0;
		virtual void DrawRegisterMenu() = 0;
		virtual void DrawDashboard() = 0;
		virtual void DrawLoginOrRegister() = 0;

		// User
		virtual void DrawBookList() = 0;
		virtual void DrawBook() = 0;
		virtual void DrawSearchBook() = 0;

		// Admin
		virtual void DrawAddBook() = 0;
		virtual void DrawUpdateBook() = 0;
		virtual void DrawDeleteBook() = 0;
	};
}
