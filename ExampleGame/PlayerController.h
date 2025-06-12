#pragma once
#include "Lillis.h"

class GameObject;

class PlayerController : public Behavior
{
public:
	PlayerController() : Behavior("PlayerController")
	{

	}

	~PlayerController() {};

	static Behavior* CreatePlayerController(char* loc);
	void Update(float deltaTime) override;
	void LoadListeners() override;
	void ConnectComponents() override;
	void handleEvent(const Event& theEvent);

	bool w = false, s = false, a = false, d = false;
private:
	const float moveSpeed = 5.0f;
	LilObj<Transform> transform;
	GraphicsSystem* graphicsSystem = nullptr;
};