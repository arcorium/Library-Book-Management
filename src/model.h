#pragma once

namespace vz
{
	enum class Access;

	struct User
	{
		std::string username;
		std::string password;
		Access privilege;
	};

	struct Book
	{
		std::string title;
		std::string description;
		std::string author;
		int years;
		std::string publisher;
		std::string genre;
	};
}
