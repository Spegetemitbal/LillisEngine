#include "../core/Lillis.h"

int main(int argc, char* argv[])
{
	Engine* e;

	if (argc > 1)
	{
		e = Engine::CreateGameInstance();
		std::cout << "Launching as Server\n";
	}
	else
	{
		e = Engine::CreateGameInstance();
		std::cout << "Launching as Client\n";
	}
	
	//e->LoadLevel("Level.lvl");
	e->InjectAssets("assets", SINGLEIMAGES);
	e->InjectAssets("assets", DATA);
	e->LoadLevel("Level");
	e->Run();

	Engine::DestroyGameInstance();

	return 0;
}
