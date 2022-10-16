#pragma once

namespace vz
{
	class UserInterface
	{
	public:
		virtual ~UserInterface() = default;

		// Drawing
		virtual void DrawLoginMenu() = 0;
		virtual void DrawRegisterMenu() = 0;


		// User
		virtual void DrawBookList() = 0;
		virtual void DrawSearchBook() = 0;
		virtual void DrawDonateBook() = 0;

		// Admin
		virtual void DrawAddBook() = 0;
		virtual void DrawUpdateBook() = 0;
		virtual void DrawDeleteBook() = 0;
		virtual void DrawBorrowedBookList() = 0;
		virtual void DrawDonatedBookList() = 0;
	};
}