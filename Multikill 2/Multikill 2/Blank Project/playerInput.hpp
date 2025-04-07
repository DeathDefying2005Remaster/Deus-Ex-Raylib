#pragma once
#include <raylib-cpp.hpp>
#include "keybinds.hpp"
#include "player.hpp"
#include "playerMovement.hpp"
#include "playerViolence.hpp"

class PlayerInput
{
	public:

	Player* player;
	PlayerMovement* playerMovement;
	PlayerViolence* playerViolence;

	PlayerInput(Player* _player, PlayerMovement* _playerMovement, PlayerViolence* _playerViolence)
	{
		player = _player;
		playerMovement = _playerMovement;
		playerViolence = _playerViolence;
	}

	void Update()
	{
		player->moveInput = 0;
		if (IsKeyDown(MLEFT)) { player->moveInput -= 1; }
		if (IsKeyDown(MRIGHT)) { player->moveInput += 1; }

		if (player->moveInput < 0) { player->left = true; }
		else if (player->moveInput > 0) { player->left = false; }

		if (IsKeyPressed(SLIDE)) { player->state = player->sliding; }
		if (IsKeyReleased(SLIDE)) { player->state = player->idle; }

		else if (player->moveInput != 0 && player->state != player->sliding) { player->state = player->running; }
		else if (player->state != player->sliding) { player->state = player->idle; }

		Vector2 shootInput = {};
		if (IsKeyDown(SUP)) { shootInput = { 0, 1 }; }
		else
		{
			if (IsKeyDown(SLEFT)) { shootInput.x -= 1; }
			if (IsKeyDown(SRIGHT)) { shootInput.x += 1; }
		}

		if (!Vector2Equals(shootInput, {}) && player->shootTimer <= 0)
		{
			player->shootDir = shootInput;
			playerViolence->Shoot();
		}
		else if (player->shootTimer <= 0)
		{
			player->shootDir = {};
		}

		if (IsKeyPressed(JUMP))
		{
			playerMovement->Jump();
		}
	}
};