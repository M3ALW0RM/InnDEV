#include "App.hpp"
int main(int argc, char* args[])
{
	App app;
	if (app.Init()) {
		app.Run();
		app.Clean();
	}
	return 0;
}