#pragma once
#include <raylib-cpp.hpp>
#include "objectPool.hpp"
#include "bulletEffects.hpp"

class PlayerInput;
class PlayerRenderer;
class PlayerMovement;
class PlayerViolence;

class Player
{
	public:

	enum PlayerState
	{
		idle,
		running,
		sliding
	};

	PlayerInput* playerInput;
	PlayerRenderer* playerRenderer;
	PlayerMovement* playerMovement;
	PlayerViolence* playerViolence;

	Vector2 position = {};
	Vector2 velocity = {};
	bool grounded = false;
	bool left = false;
	PlayerState state = idle;
	Vector2 shootDir = {};
	float moveInput = 0;

	float shootTimer = 0;

	Player();

	void Update();
	void Draw();
};