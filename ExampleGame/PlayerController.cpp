#include "PlayerController.h"

void PlayerController::LoadListeners()
{
	addListener(INPUT_EVENT);
}


Behavior* PlayerController::CreatePlayerController(char* loc)
{
	return new (loc)PlayerController();//G->CreatePlayerController();
}

void PlayerController::Update(float deltaTime)
{
	if (!transform.Exists())
	{
		return;
	}

	if (w)
	{
		transform->Translate({0, moveSpeed * deltaTime});
	}
	else if (s)
	{
		transform->Translate({0, -moveSpeed * deltaTime});
	}
	if (a)
	{
		transform->Translate({-moveSpeed * deltaTime, 0});
	}
	else if (d)
	{
		transform->Translate({moveSpeed * deltaTime, 0});
	}
	graphicsSystem->SetCameraPosition(transform->GlobalPosition());
	//std::cout << "PostMove: " << t.x << " " << t.y << '\n';
}

void PlayerController::handleEvent(const Event& theEvent)
{
	if (theEvent.getType() == INPUT_EVENT)
	{
		const InputEvent& inputEvent = static_cast<const InputEvent&>(theEvent);
		int key = inputEvent.getInput();
		bool down = inputEvent.getPressed();

		//std::cout << "Key Pressed: " << key << std::endl;

		switch (key)
		{	
		case W:
			w = down;
			break;
		case S:
			s = down;
			break;
		case A:
			a = down;
			break;
		case D:
			d = down;
			break;
		}
	}
}

void PlayerController::ConnectComponents()
{
	transform = thisObject->transform;
	graphicsSystem = GraphicsSystem::getInstance();
}
