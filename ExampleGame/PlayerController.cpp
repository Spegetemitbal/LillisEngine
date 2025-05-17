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
	//std::cout << "Internal: " << thisObject->getSprite() << '\n';

	//Make this more cache friendly.
	LilObj<Transform> t = thisObject->transform;

	if (w)
	{
		t->Translate({0, moveSpeed * deltaTime});
	}
	else if (s)
	{
		t->Translate({0, -moveSpeed * deltaTime});
	}
	if (a)
	{
		t->Translate({-moveSpeed * deltaTime, 0});
	}
	else if (d)
	{
		t->Translate({moveSpeed * deltaTime, 0});
	}
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