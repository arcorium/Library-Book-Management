#pragma once
#include <iomanip>

#include "utility.h"
#include "system/user_interface.h"

namespace vz
{
	class Application;

	enum class Location
	{
		Left,
		Center,
		Right
	};

	class TerminalInterace : public UserInterface
	{
	public:
		void DrawLoginMenu() override;
		void DrawRegisterMenu() override;
		void DrawDashboard() override;
		void DrawLoginOrRegister() override;

		void DrawErrorMessage(std::string&& header_, const std::string& message_) override;
		void DrawBook() override;
		void DrawBookList() override;
		void DrawSearchBook() override;
		void DrawAddBook() override;
		void DrawUpdateBook() override;
		void DrawDeleteBook() override;

		void SetApplication(Application* app_) override;

	private:
		template<Location Loc, typename Func, typename... Args>
		void Title(Func&& func_, Args&&... args_);

		template<Location Loc, typename Func, typename... Args>
		void Body(Func&& func_, Args&&... args_);

		void PrintBeauty(const std::string& str_);

		template<Location Loc>
		constexpr size_t GetWidth(std::string_view str_);

		constexpr std::string_view Separator() const
		{
			constexpr std::string_view separator{ "[+]====================================================[+]" };
			return separator;
		}

	private:
		vz::Application* m_parent{};
	};

	template <Location Loc>
	constexpr size_t TerminalInterace::GetWidth(std::string_view str_)
	{
		auto separator = Separator();
		

		if constexpr (Loc == Location::Center)
		{
			if (str_.size() < separator.size())
			{
				size_t middle = separator.size() / 2;
				return middle + (str_.size() / 2);
			}
		}
		else if constexpr (Loc == Location::Right)
		{
			if (str_.size() < separator.size())
				return separator.size();
		}

		return str_.size() + 4;
	}

	// Inline
	template <Location Loc, typename T, typename... Args>
	void TerminalInterace::Title(T&& func_, Args&&... args_)
	{
		auto separator = Separator();


		vz::println(separator);
		auto result = func_(std::forward<Args>(args_)...);
		size_t width = GetWidth<Loc>(result);

		vz::println(std::setw(width), result);
		vz::println(separator);
	}

	template <Location Loc, typename Func, typename ... Args>
	void TerminalInterace::Body(Func&& func_, Args&&... args_)
	{
		auto separator = Separator();

		func_(std::forward<Args>(args_)...);
		//size_t width = GetWidth<Loc>(result);

		//vz::println(result);
		vz::println(separator);
	}
}
