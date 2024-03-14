#include <Lillis.h>

int main(int argc, char* argv[])
{
	Engine* e = Engine::CreateGameInstance();
	
	e->LoadLevel("Level.dat");
	e->Run();

	Engine::DestroyGameInstance();

	return 0;
}
