#include <raylib-cpp.hpp>
#include "player.hpp"
#include "objectPool.hpp"
#include "bulletEffects.hpp"
#include "playerInput.hpp"
#include "playerRenderer.hpp"
#include "playerMovement.hpp"
#include "playerViolence.hpp"

Player::Player()
{
	playerMovement = new PlayerMovement(this);
	playerViolence = new PlayerViolence(this);
	playerInput = new PlayerInput(this, playerMovement, playerViolence);
	playerRenderer = new PlayerRenderer(this);
}

void Player::Update()
{
	Player::playerInput->Update();
	Player::playerMovement->Update();

	shootTimer -= GetFrameTime();
	if (shootTimer < 0) { shootTimer = 0; }
}
void Player::Draw()
{
	Player::playerRenderer->Update();
}