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

	int speed = 5;

	//Make this more cache friendly.
	Transform& t = thisObject->transform;

	if (w)
	{
		t.localPosition.y -= speed;
	}
	else if (s)
	{
		t.localPosition.y += speed;
	}
	if (a)
	{
		t.localPosition.x -= speed;
	}
	else if (d)
	{
		t.localPosition.x += speed;
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