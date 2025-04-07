#pragma once
#include <raylib-cpp.hpp>
#include "player.hpp"
#include "bulletEffects.hpp"
#include "global.hpp"
#include "global2.hpp"

class PlayerViolence
{
	public:

	Player* player;

	float revolverCooldown = 0.5;

	PlayerViolence(Player* _player)
	{
		player = _player;
	}

	void Shoot()
	{
		player->shootTimer = revolverCooldown;
		Vector2 origin;
		int index = 0;

		switch ((int)player->shootDir.x)
		{
			case -1:
				index = 0;
				break;
			case 0:
				index = 2;
				break;
			case 1:
				index = 4;
				break;
		}

		if (player->state == player->sliding)
		{
			if (player->left)
			{
				origin = barrelPos[0][index];
			}
			else
			{
				origin = barrelPos[1][index];
			}
		}
		else
		{
			origin = barrelPos[2][index];
		}

		Vector2 origin2 = { player->position.x - (spriteSize.x / 2) + origin.x, player->position.y - (spriteSize.y / 2) + origin.y };
		BulletTrail trail = BulletTrail(origin2, Vector2Normalize({ player->shootDir.x, -player->shootDir.y }), 0.1, 5);
		bradPitt.Add(trail);
	}
};