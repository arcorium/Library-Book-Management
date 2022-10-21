#include "utility.h"
#include "system/application.h"
#include <filesystem>

#include "interface/terminal_interface.h"
#include "toml++/toml.h"

int main(int argc, char** argv)
{
	using namespace std::chrono_literals;

	std::string folderpath{};
	if (argc > 1)
	{
		folderpath = argv[1];

		if (!folderpath.ends_with('/') || !folderpath.ends_with('\\'))
			folderpath += "/";
	}
	else
	{
		folderpath = "./res/";
	}

	vz::println("Data Resource Path: ", folderpath);
	vz::Application app{ new vz::TerminalInterace{} };
	app.SetPath(folderpath);
	std::this_thread::sleep_for(1000ms);

	return app.Run();
}
