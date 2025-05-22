#include "../core/Lillis.h"
#include "Rotator.h"
#include "PlayerController.h"
#include "EngineStuffs/Graphics/ProcGen.h"
#include "EngineStuffs/UI/UISystem.h"

Engine* e;
WorldManager* wm;

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
			wm->ReloadCurrentWorld();
		}

		if (id1 == 1 && id2 == 2 || id1 == 0 && id2 == 3)
		{
			std::cout << "Player2 Wins!" << '\n';
			wm->ReloadCurrentWorld();
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

	RenderSettings renderSettings = RenderSettings(640, 480, 1280, 960, 40);

	GraphicsSystem* graphics = GraphicsSystem::createInstance(renderSettings, "Game");
	graphics->Init();
	InputSystem::addKeyCodes(importantKeys);
	UISystem::createInstance(renderSettings);
	wm = WorldManager::getInstance();

	EventSystem* events = EventSystem::getInstance();
	events->addCallback((EventType)COLLISION_EVENT ,handleEvent);

	ResourceLoader::LoadProjectInfo("assets/StaticData.lil");
	ResourceLoader::LoadDataRecursive("assets");
	ResourceLoader::LoadTextureRecursive("assets");

	AudioSystem::getInstance()->Init();
	PhysicsSettings physics_settings = PhysicsSettings();
	physics_settings.gravity = {};
	PhysicsSystem* phys = PhysicsSystem::createInstance(physics_settings);
	ProcGen* procGen = ProcGen::createInstance(30);
	procGen->SetPointSize(3.0f);
	phys->DoRenderPhysics(true);
	BehaviorSystem::RegisterBehavior("PlayerController", sizeof(PlayerController), PlayerController::CreatePlayerController);
	BehaviorSystem::RegisterBehavior("Rotator", sizeof(Rotator), Rotator::CreateRotator);
	wm->MakeWorld("PhysLevel");
	e->Run();

	Engine::DestroyGameInstance();

	return 0;
}
