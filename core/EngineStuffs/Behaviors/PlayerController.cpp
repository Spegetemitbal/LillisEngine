#include "../GameObject.h"
#include "PlayerController.h"
#include "../../Utils/Events/InputEvent.h"

Component* PlayerController::CreatePlayerController(GameObject* G, int* params)
{
	return G->CreatePlayerController();
}

void PlayerController::Update()
{
	//std::cout << "Internal: " << thisObject->getSprite() << '\n';

	int speed = 5;

	//Make this more cache friendly.
	Transform& t = thisObject->transform;

	if (w)
	{
		t.y -= speed;
	}
	else if (s)
	{
		t.y += speed;
	}
	if (a)
	{
		t.x -= speed;
	}
	else if (d)
	{
		t.x += speed;
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