#pragma once
#include <concepts>
#include <string>

#include "linked_list.h"
#include "model.h"

namespace vz
{
	class UserInterface;

	enum class Access
	{
		User,
		Admin
	};

	enum class State
	{
		None,
		Stop,
		Login,
		Register,
		Dashboard,
		Add,
		Delete,
		Update,
		Search,
		List,
		Error,
		Success,
		Book
	};

	class Application
	{

	public:
		Application(UserInterface* ui_);
		~Application();

		int Run();

		// Process
		bool LoadBooks();
		void SaveBooks();
		void AddBook(Book* book_);
		void DeleteBook(Book* book_);

		bool LoadUsers();
		void SaveUsers();
		void AddUser(User* user_);

		bool ValidateUsernamePassword(std::string&& username_, std::string&& password_);
		bool IsUserExist(const std::string& username_);
		bool IsBookExist(const std::string& title_, const std::string& author_, int years_);

		void SetPath(const std::string& folderpath_);
		void SetState(State state_);
		void SetErrorState(State state_, std::string&& message_);
		void SetSuccessState(State state_, std::string&& message_);
		void SetBook(Book* book_);

		bool IsAdmin() const;
		LinkedList<Book*>& GetBooks();
		LinkedList<User*> GetUsers();
		Book* GetBook() const;

		[[nodiscard]] std::string GetPath(const std::string& append_ = {}) const;

	private:
		UserInterface* m_ui{};
		std::string m_folderpath;

		User* m_user;
		Book* m_book;

		State m_state;
		State m_futureState;
		std::string m_message;

		// List
		LinkedList<User*> m_users;
		LinkedList<Book*> m_books;
	};
}