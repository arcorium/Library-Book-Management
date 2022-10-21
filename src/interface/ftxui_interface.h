#pragma once
#include "system/user_interface.h"

namespace vz
{
	class FtxuiInterface : public UserInterface
	{
	public:
		~FtxuiInterface() override = default;
		void DrawLoginMenu() override;
		void DrawRegisterMenu() override;
		void DrawBookList() override;
		void DrawSearchBook() override;
		void DrawAddBook() override;
		void DrawUpdateBook() override;
		void DrawDeleteBook() override;
	};
}