#include "terminal_interface.h"

#include <conio.h>
#include <iomanip>
#include <thread>
#include <toml++/toml.h>

#include "system/application.h"

namespace vz
{
	enum class Result
	{
		None,
		Failed,
		Success
	};

	void TerminalInterace::DrawLoginMenu()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "LOGIN" };
				});

			Body<Location::Left>([&]
				{
					if (condition == Result::Failed)	vz::println("    Failed to Login!");

					std::string username, password;

					vz::print_nice("Username: ");
					std::cin >> username;
					vz::print_nice("Password: ");
					std::cin >> password;

					if (!m_parent->ValidateUsernamePassword(std::move(username), std::move(password)))
					{
						condition = Result::Failed;
					}
					else
					{
						condition = Result::Success;
					}
				});

		}

		m_parent->SetState(State::Dashboard);
	}

	void TerminalInterace::DrawRegisterMenu()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "REGISTER" };
				});

			Body<Location::Left>([&]
				{
					std::string username, password;
					Access privilege = Access::User;

					vz::print_nice("Username: ");
					std::cin >> username;
					vz::print_nice("Password: ");
					std::cin >> password;

					if (m_parent->IsUserExist(std::move(username)))
					{
						condition = Result::Success;
						m_parent->SetErrorState(State::Register, "Failed to register");
					}
					else
					{
						condition = Result::Success;
						auto user = new User{ username, password, privilege };
						m_parent->AddUser(user);
						m_parent->SetState(State::Login);
					}
				});

		}

	}

	void TerminalInterace::DrawDashboard()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "DASHBOARD" };
				});

			Body<Location::Left>([&]
				{
					if (condition == Result::Failed)	vz::println("    Wrong number!");

					int choose = -1;

					PrintBeauty("1. Book List");
					PrintBeauty("2. Search Book");
					if (m_parent->IsAdmin())
					{
						PrintBeauty("");
						PrintBeauty("3. Add Book");
					}
					PrintBeauty("");
					PrintBeauty("99. Exit");
					vz::print_nice("Choose: ");
					std::cin >> choose;

					if ((choose > 0 && choose < 6) || choose == 99)
					{
						condition = Result::Success;
						switch (choose)
						{
						case 1:
							m_parent->SetState(State::List);
							break;
						case 2:
							m_parent->SetState(State::Search);
							break;
						case 3:
							if (m_parent->IsAdmin())	m_parent->SetState(State::Add);
							break;
						case 99:
							m_parent->SetState(State::Stop);
							break;
						default:
							condition = Result::Failed;
						}
					}
				});
		}
	}

	void TerminalInterace::DrawLoginOrRegister()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "LIBRARY BOOK MANAGEMENT" };
				});

			Body<Location::Left>([&]
				{
					if (condition == Result::Failed)	vz::println("    Wrong number!");

					int choose = -1;

					vz::println_nice("1. Login");
					vz::println_nice("2. Register");
					vz::print_nice("Choose: ");
					std::cin >> choose;

					if (choose > 0 && choose < 3)
					{
						switch (choose)
						{
						case 1:
							condition = Result::Success;
							m_parent->SetState(State::Login);
							break;
						case 2:
							condition = Result::Success;
							m_parent->SetState(State::Register);
							break;
						default:
							condition = Result::Failed;
						}
					}
				});
		}
	}

	void TerminalInterace::DrawErrorMessage(std::string&& header_, const std::string& message_)
	{
		using namespace std::chrono_literals;

		vz::clear();
		Title<Location::Center>([head = std::move(header_)]
			{
				return head;
			});
		PrintBeauty(message_);
		vz::println(Separator());
		vz::println("Press any key to continue...");
		_getch();
	}

	void TerminalInterace::DrawBook()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "BOOK" };
				});

			Body<Location::Left>([&]
				{
					if (condition == Result::Failed)	vz::println("    Wrong number!");

					int choose = -1;

					// Show Book Details
					auto book = m_parent->GetBook();
					PrintBeauty("Title: " + book->title);
					PrintBeauty("Author: " + book->author);
					PrintBeauty("Genre: " + book->genre);
					vz::println("[+] Description: " + book->description);
					PrintBeauty("Years: " + std::to_string(book->years));
					PrintBeauty("Publisher: " + book->publisher);
					vz::println(Separator());

					PrintBeauty("1. Back");
					if (m_parent->IsAdmin())
					{
						PrintBeauty("");
						PrintBeauty("4. Delete Book");
						PrintBeauty("5. Edit Book");
					}
					PrintBeauty("");
					PrintBeauty("99. Exit");
					vz::print_nice("Choose: ");
					std::cin >> choose;

					if ((choose > 0 && choose < 6) || choose == 99)
					{
						condition = Result::Success;
						switch (choose)
						{
						case 1:
							m_parent->SetState(State::Dashboard);
							break;
						case 4:
							if (m_parent->IsAdmin())	m_parent->SetState(State::Delete);
							break;
						case 5:
							if (m_parent->IsAdmin())	m_parent->SetState(State::Update);
							break;
						case 99:
							m_parent->SetState(State::Stop);
							break;
						default:
							condition = Result::Failed;
						}
					}
				});
		}
	}

	void TerminalInterace::DrawBookList()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "BOOK LIST" };
				});

			Body<Location::Left>([&]
				{
					if (condition == Result::Failed)	vz::println("    Wrong number!");

					// Book List
					auto& books = m_parent->GetBooks();

					auto current = books.head();
					while(current)
					{
						auto data = current->data;
						PrintBeauty(data->title + " " + std::to_string(data->years) + " by " + data->author);
						current = current->next;
					}


					// Choice
					int choose = -1;

					vz::println(Separator());
					PrintBeauty("1. Back");
					if (m_parent->IsAdmin())
					{
						PrintBeauty("");
						PrintBeauty("3. Add Book");
					}
					PrintBeauty("");
					PrintBeauty("99. Exit");
					vz::print_nice("Choose: ");
					std::cin >> choose;

					if ((choose > 0 && choose < 6) || choose == 99)
					{
						condition = Result::Success;
						switch (choose)
						{
						case 1:
							m_parent->SetState(State::Dashboard);
							break;
						case 3:
							if (m_parent->IsAdmin())	m_parent->SetState(State::Add);
							break;
						case 4:
							if (m_parent->IsAdmin())	m_parent->SetState(State::Delete);
							break;
						case 5:
							if (m_parent->IsAdmin())	m_parent->SetState(State::Update);
							break;
						case 99:
							m_parent->SetState(State::Stop);
							break;
						default:
							condition = Result::Failed;
						}
					}
				});
		}
	}

	void TerminalInterace::DrawSearchBook()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "SEARCH BOOK" };
				});

			Body<Location::Left>([&]
				{

					std::string title, author;
					int year = 0;

					vz::print_nice("Title: ");
					std::cin >> title;
					vz::print_nice("Author: ");
					std::cin >> author;
					vz::print_nice("Years: ");
					std::cin >> year;

					// Search Book
					auto& books = m_parent->GetBooks();

					auto current = books.head();
					while (current)
					{
						const auto data = current->data;
						if (data->title == title && data->author == author && data->years == year)
						{
							m_parent->SetBook(data);
							m_parent->SetState(State::Book);
							condition = Result::Success;
							break;
						}
						current = current->next;
						condition = Result::Failed;
					}

					if (condition == Result::Failed)
					{
						condition = Result::Success;
						m_parent->SetErrorState(State::Dashboard, "There is no such book");
					}
				});
		}
	}

	void TerminalInterace::DrawAddBook()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "ADD BOOK" };
				});

			Body<Location::Left>([&]
				{

					std::string title, author, description, publisher, genre;
					int year = 0;

					vz::print_nice("Title: ");
					std::cin >> title;

					vz::print_nice("Author: ");
					std::cin >> author;

					vz::print_nice("Genre: ");
					std::cin >> genre;

					vz::print_nice("Description: ");
					std::getchar();	// Take new line
					std::getline(std::cin, description);

					vz::print_nice("Years: ");
					std::cin >> year;

					vz::print_nice("Publisher: ");
					std::cin >> publisher;

					// Search Book
				if (m_parent->IsBookExist(title, author, year))
				{
					condition = Result::Success;
					m_parent->SetErrorState(State::Dashboard, "Book already exists");
				}
				else
				{
					condition = Result::Success;
					auto book = new Book{ title, description, author, year, publisher, genre };
					m_parent->AddBook(book);
					m_parent->SetState(State::Book);
					m_parent->SetBook(book);
				}
				
				});
		}
	}

	void TerminalInterace::DrawUpdateBook()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "EDIT BOOK" };
				});

			Body<Location::Left>([&]
				{

					std::string title, author, description, publisher, genre;
					int year = 0;

					vz::println_nice("Write \'x\' when no change is needed");

					auto book = m_parent->GetBook();
					PrintBeauty("");
					PrintBeauty("Title: " + book->title);
					PrintBeauty("Author: " + book->author);
					PrintBeauty("Genre: " + book->genre);
					PrintBeauty("Description: " + book->description);
					PrintBeauty("Years: " + std::to_string(book->years));
					PrintBeauty("Publisher: " + book->publisher);
					vz::println(Separator());
					PrintBeauty("");

					vz::print_nice("New Title: ");
					std::cin >> title;
					if (title != "x")
					{
						book->title = title;
					}

					vz::print_nice("New Author: ");
					std::cin >> author;
					if (title != "x")
					{
						book->author = author;
					}

					vz::print_nice("New Genre: ");
					std::cin >> genre;
					if (title != "x")
					{
						book->genre = genre;
					}

					vz::print_nice("New Description: ");
					std::getchar();	// Take new line
					std::getline(std::cin, description);
					if (title != "x")
					{
						book->description = description;
					}

					vz::print_nice("New Years: ");
					std::cin >> year;
					if (title != "x")
					{
						book->years = year;
					}

					vz::print_nice("New Publisher: ");
					std::cin >> publisher;
					if (title != "x")
					{
						book->publisher = publisher;
					}

					m_parent->SaveBooks();
					m_parent->SetState(State::Book);
				});
		}
	}

	void TerminalInterace::DrawDeleteBook()
	{
		Result condition = Result::None;
		while (condition != Result::Success)
		{
			vz::clear();

			Title<Location::Center>([]
				{
					return std::string_view{ "DELETE BOOK" };
				});

			Body<Location::Left>([&]
				{
					if (condition == Result::Failed)	vz::println("    Wrong number!");

					int choose = -1;

					auto book = m_parent->GetBook();
					PrintBeauty("Are you sure to delete book titled " + book->title + " by " + book->author);
					PrintBeauty("1. Yes");
					PrintBeauty("2. No");
					
					PrintBeauty("");
					PrintBeauty("99. Exit");
					vz::print_nice("Choose: ");
					std::cin >> choose;

					if ((choose > 0 && choose < 3) || choose == 99)
					{
						condition = Result::Success;
						switch (choose)
						{
						case 1:
							m_parent->DeleteBook(book);
							m_parent->SetSuccessState(State::Dashboard, "Delete Book Success!");
							break;
						case 2:
							m_parent->SetState(State::Dashboard);
							break;
						case 99:
							m_parent->SetState(State::Stop);
							break;
						default:
							condition = Result::Failed;
						}
					}
				});
		}
	}

	void TerminalInterace::SetApplication(Application* app_)
	{
		m_parent = app_;
	}

	void TerminalInterace::PrintBeauty(const std::string& str_)
	{
		auto size = Separator().size();
		vz::println("[+] ", str_, std::setw(size - str_.size() - 4), "[+]");
	}
}
