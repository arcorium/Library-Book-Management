#include "application.h"

#include "user_interface.h"
#include "utility.h"
#include "toml++/toml.h"

namespace vz
{
	Application::Application(UserInterface* ui_)
		: m_ui{ ui_ }, m_state{State::None}
	{
		m_ui->SetApplication(this);
	}

	Application::~Application()
	{
		// Delete Users
		auto currentUser = m_users.head();
		while(currentUser)
		{
			delete currentUser->data;
			currentUser = currentUser->next;
		}

		// Delete Books
		auto currentBook = m_books.head();
		while (currentBook)
		{
			delete currentBook->data;
			currentBook = currentBook->next;
		}
	}

	int Application::Run()
	{
		LoadUsers();
		LoadBooks();

		while(m_state != State::Stop)
		{
			switch (m_state)
			{
			case State::None:
				m_ui->DrawLoginOrRegister();
				break;
			case State::Login:
				m_ui->DrawLoginMenu();
				break;
			case State::Register:
				m_ui->DrawRegisterMenu();
				break;
			case State::Dashboard:
				m_ui->DrawDashboard();
				break;
			case State::Add:
				m_ui->DrawAddBook();
				break;
			case State::Delete:
				m_ui->DrawDeleteBook();
				break;
			case State::Update:
				m_ui->DrawUpdateBook();
				break;
			case State::Search:
				m_ui->DrawSearchBook();
				break;
			case State::List:
				m_ui->DrawBookList();
				break;
			case State::Error:
				m_ui->DrawErrorMessage("ERROR", m_message);
				SetState(m_futureState);
				m_futureState = State::None;
				break;
			case State::Success:
				m_ui->DrawErrorMessage("SUCCESS", m_message);
				SetState(m_futureState);
				m_futureState = State::None;
				break;
			case State::Book:
				m_ui->DrawBook();
				break;
			case State::Stop:
				
				break;
			}
		}
		return 0;
	}

	bool Application::LoadBooks()
	{
		auto path = GetPath("books.toml");
		auto file = toml::parse_file(path);

		if (file.succeeded())
		{
			auto res = file.table().at("book").as_array();

			for (auto& data : *res)
			{
				auto table = data.as_table();

				Book* book = new Book{ table->at("title").as_string()->value_or(""),
					table->at("description").as_string()->value_or(""),
					table->at("author").as_string()->value_or(""),
					table->at("years").as_integer()->value_or(0),
					table->at("publisher").as_string()->value_or(""),
					table->at("genre").as_string()->value_or("")
				};

				m_books.push_back(book);
			}

			return true;
		}

		std::ofstream fileCreate{ GetPath("books.toml")};
		fileCreate.flush();

		return false;
	}

	void Application::SaveBooks()
	{
		auto current = m_books.head();
		std::ofstream file{ GetPath("books.toml") };
		while(current)
		{
			auto data = current->data;

			file << "[[book]]" << "\n"
				<< "title = \"" << data->title << "\"\n"
				<< "description = \"" << data->description << "\"\n"
				<< "author = \"" << data->author << "\"\n"
				<< "years = " << data->years << "\n"
				<< "publisher = \"" << data->publisher << "\"\n"
				<< "genre = \"" << data->genre << "\"\n"
				<< "\n";

			current = current->next;
		}

		file.flush();
	}

	void Application::AddBook(Book* book_)
	{
		m_books.push_back(book_);
		SaveBooks();
	}

	void Application::DeleteBook(Book* book_)
	{
		// Search book
		auto current = m_books.head();
		size_t index = 0;
		while (current)
		{
			auto data = current->data;
			if (book_->title == data->title && book_->years == data->years && book_->author == data->author)
			{
				break;
			}
			current = current->next;
			++index;
		}

		auto deleted = m_books.remove(index);
		if (deleted)
		{
			delete deleted->data;
		}

		SaveBooks();
	}

	bool Application::LoadUsers()
	{
		auto path = GetPath("users.toml");
		auto file = toml::parse_file(path);

		if (file.succeeded())
		{
			auto res = file.table().at("user").as_array();

			for (auto& data : *res)
			{
				auto table = data.as_table();

				Access privilege = Access::User;
				{
					auto access = table->at("privilege").as_string()->value_or("");
					if (!std::strcmp(access, "Admin"))
					{
						privilege = Access::Admin;
					}
				}

				User* user = new User{ table->at("username").as_string()->value_or(""),
					table->at("password").as_string()->value_or(""),
					privilege
				};

				m_users.push_back(user);
			}

			return true;
		}

		std::ofstream fileCreate{ GetPath("users.toml") };
		fileCreate.flush();

		return false;
	}

	void Application::SaveUsers()
	{
		auto current = m_users.head();
		std::ofstream file{ GetPath("users.toml") };
		while (current)
		{
			auto data = current->data;
			std::string privilege{""};
			switch (data->privilege)
			{
			case Access::User:
				privilege = "User";
				break;
			case Access::Admin: 
				privilege = "Admin";
				break;
			}


			file << "[[user]]" << "\n"
				<< "username = \"" << data->username << "\"\n"
				<< "password = \"" << data->password << "\"\n"
				<< "privilege = \"" << privilege << "\"\n"
				<< "\n";

			current = current->next;
		}

		file.flush();
	}

	void Application::AddUser(User* user_)
	{
		m_users.push_back(user_);
		SaveUsers();
	}

	void Application::SetPath(const std::string& folderpath_)
	{
		m_folderpath = folderpath_;
	}

	void Application::SetState(State state_)
	{
		m_state = state_;
	}

	void Application::SetErrorState(State state_, std::string&& message_)
	{
		m_state = State::Error;
		m_futureState = state_;
		m_message = message_;
	}

	void Application::SetSuccessState(State state_, std::string&& message_)
	{
		m_state = State::Success;
		m_futureState = state_;
		m_message = message_;
	}

	void Application::SetBook(Book* book_)
	{
		m_book = book_;
	}

	bool Application::IsAdmin() const
	{
		return m_user->privilege == Access::Admin;
	}

	LinkedList<Book*>& Application::GetBooks()
	{
		return m_books;
	}

	LinkedList<User*> Application::GetUsers()
	{
		return m_users;
	}

	Book* Application::GetBook() const
	{
		return m_book;
	}

	bool Application::ValidateUsernamePassword(std::string&& username_, std::string&& password_)
	{
		auto path = GetPath("users.toml");
		auto file = toml::parse_file(path);
		if (file.succeeded())
		{
			auto res = file.table().at("user").as_array();

			auto current = m_users.head();
			while (current)
			{
				auto user = current->data;
				if (username_ == user->username && password_ == user->password)
				{
					m_user = user;
					return true;
				}

				current = current->next;
			}
		}

		return false;
	}

	bool Application::IsUserExist(const std::string& username_)
	{
		auto path = GetPath("users.toml");
		auto file = toml::parse_file(path);
		if (file.succeeded())
		{
			auto res = file.table().at("user").as_array();

			for (auto& data : *res)
			{
				auto table = data.as_table();

				if (username_ == table->at("username").as_string()->value_or(""))
				{
					return true;
				}
			}
		}
		else
		{
			vz::println("Omaga");
		}

		return false;
	}

	bool Application::IsBookExist(const std::string& title_, const std::string& author_, int years_)
	{
		auto current = m_books.head();
		while (current)
		{
			auto data = current->data;
			if (title_ == data->title && years_ == data->years && author_ == data->author)
				return true;
			current = current->next;
		}

		return false;
	}

	std::string Application::GetPath(const std::string& append_) const
	{
		auto result = m_folderpath + append_;
		return result;
	}
}
