#include "../core/Lillis.h"
#include "Rotator.h"
#include "PlayerController.h"

Engine* e;

void handleEvent(const Event& theEvent)
{
	if (theEvent.getType() == COLLISION_EVENT)
	{
		const CollisionEvent& colEvent = static_cast<const CollisionEvent&>(theEvent);
		int id1 = colEvent.getThisCollider();
		int id2 = colEvent.getThatCollider();

		if (id1 == 0 && id2 == 2 || id1 == 1 && id2 == 3)
		{
			std::cout << "Player1 Wins!" << '\n';
			e->LoadLevel(e->CurrentLevel);
		}

		if (id1 == 1 && id2 == 2 || id1 == 0 && id2 == 3)
		{
			std::cout << "Player2 Wins!" << '\n';
			e->LoadLevel(e->CurrentLevel);
		}
	}
}

int main(int argc, char* argv[])
{

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

	std::vector<LILLIS::KeyCode> importantKeys
	{
		LILLIS::W, LILLIS::S, LILLIS::A, LILLIS::D, LILLIS::ESC
	};

	RenderSettings renderSettings = RenderSettings(640, 480, 1280, 960);

	e->Init(renderSettings, "Game", importantKeys);

	EventSystem* events = EventSystem::getInstance();
	events->addCallback((EventType)COLLISION_EVENT ,handleEvent);

	e->LoadImportData("assets/StaticData.lil");
	e->InjectAssets("assets", SPRITE);
	e->InjectAssets("assets", DATA);
	e->InitAudio();
	BehaviorSystem::RegisterBehavior("PlayerController", sizeof(PlayerController), PlayerController::CreatePlayerController);
	BehaviorSystem::RegisterBehavior("Rotator", sizeof(Rotator), Rotator::CreateRotator);
	e->LoadLevel("Level");
	e->Run();

	Engine::DestroyGameInstance();

	return 0;
}
