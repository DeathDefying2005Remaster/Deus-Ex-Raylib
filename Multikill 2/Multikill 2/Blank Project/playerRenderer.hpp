#pragma once
#include <raylib-cpp.hpp>
#include <array>
#include "global.hpp"
#include "player.hpp"
using namespace std;

class PlayerRenderer
{
	public:

	//---- Data ----//
	Player* player;
	float runTimer = 0;

	//---- Properties ----//
	float runInterval = 0.5;

	PlayerRenderer(Player* _player)
	{
		player = _player;
	}

	void Update()
	{
		float frameWidth = 96;
		float frameHeight = 64;

		array<int, 2> coords = GetCoords();
		raylib::Rectangle sourceRect = raylib::Rectangle((float)coords[0] * frameWidth, (float)coords[1] * frameHeight, frameWidth, frameHeight);
		raylib::Rectangle destRect = raylib::Rectangle(player->position.x - (frameWidth * 0.5f), player->position.y - (frameHeight * 0.5f), frameWidth, frameHeight);

		DrawTexturePro(sheet, sourceRect, destRect, {}, 0, WHITE);

		runTimer += GetFrameTime();
		if (runTimer >= runInterval) { runTimer = 0; }
	}

	array<int, 2> GetCoords()
	{
		int row = 0;
		int column = 0;
		vector<Vector2>  dirToIndex = { { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };

		if (!Vector2Equals(player->shootDir, Vector2()))
		{
			if (player->state == player->sliding && player->left) { row = 0; }
			else if (player->state == player->sliding && !player->left) { row = 1; }
			else if (player->state != player->sliding) { row = 2; }

			for (int i = 0; i < dirToIndex.size(); i++)
			{
				if (Vector2Equals(player->shootDir, dirToIndex[i]))
				{
					column = i;
					break;
				}
			}
		}
		else
		{
			if (player->state == player->sliding)
			{
				row = player->left ? 0 : 1;
				column = 0;
			}
			else if (player->state == player->running)
			{
				row = 4;
				if (player->left && runTimer < runInterval / 2) { column = 0; }
				else if (player->left && runTimer >= runInterval / 2) { column = 1; }
				else if (!player->left && runTimer < runInterval / 2) { column = 2; }
				else if (!player->left && runTimer >= runInterval / 2) { column = 3; }
			}
			else if (player->state != player->running)
			{
				row = 3;
				if (runTimer < runInterval / 2) { column = player->left ? 0 : 2; }
				else if (runTimer >= runInterval / 2) { column = player->left ? 1 : 3; }
				else if (!player->left) { column = 2; }
			}
		}

		return { column, row };
	}
};