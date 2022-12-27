#include <Engine.h>

int main(int argc, char* argv[])
{
	Engine* e = new Engine();

	e->Run();

	delete e;

	return 0;
}
