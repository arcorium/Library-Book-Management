#include "utility.h"
#include "interface/ftxui_interface.h"
#include "system/application.h"

#define TOML_EXCEPTIONS 0
#include "toml++/toml.h"

int main()
{
	auto res = toml::parse_file("asd");
	vz::Application app{};
	vz::UserInterface* userInterace = new vz::FtxuiInterface{};

	return app.Run(userInterace);
}
