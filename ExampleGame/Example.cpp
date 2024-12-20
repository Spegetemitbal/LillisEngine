#include "../core/Lillis.h"
#include "Rotator.h"
#include "PlayerController.h"

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
	BehaviorSystem::RegisterBehavior("PlayerController", sizeof(PlayerController), PlayerController::CreatePlayerController);
	BehaviorSystem::RegisterBehavior("Rotator", sizeof(Rotator), Rotator::CreateRotator);
	e->LoadLevel("Level");
	e->Run();

	Engine::DestroyGameInstance();

	return 0;
}
