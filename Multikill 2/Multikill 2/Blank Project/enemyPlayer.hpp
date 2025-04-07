#pragma once
#include <raylib-cpp.hpp>
#include "netcode.hpp"
#include "player.hpp"
#include <array>

class PlayerInfo
{
	public:

	Texture2D sheet;

	NetworkVariable<Vector2> position;
	NetworkVariable<Vector2> shootDir;
	NetworkVariable<enet_uint32> state;
	NetworkVariable<bool> left;
	float runTimer;

	float runInterval = 0.5;

	PlayerInfo()
	{
		
	}

	void Draw()
	{
		float frameWidth = 96;
		float frameHeight = 64;

		array<int, 2> coords = GetCoords();
		raylib::Rectangle sourceRect = raylib::Rectangle((float)coords[0] * frameWidth, (float)coords[1] * frameHeight, frameWidth, frameHeight);
		raylib::Rectangle destRect = raylib::Rectangle(position.value.x - (frameWidth * 0.5f), position.value.y - (frameHeight * 0.5f), frameWidth, frameHeight);

		DrawTexturePro(sheet, sourceRect, destRect, {}, 0, WHITE);

		runTimer += GetFrameTime();
		if (runTimer >= runInterval) { runTimer = 0; }
	}

	array<int, 2> GetCoords()
	{
		int row = 0;
		int column = 0;
		vector<Vector2>  dirToIndex = { { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };

		if (!Vector2Equals(shootDir.value, Vector2()))
		{
			if (state.value == Player::sliding && left.value) { row = 0; }
			else if (state.value == Player::sliding && !left.value) { row = 1; }
			else if (state.value != Player::sliding) { row = 2; }

			for (int i = 0; i < dirToIndex.size(); i++)
			{
				if (Vector2Equals(shootDir.value, dirToIndex[i]))
				{
					column = i;
					break;
				}
			}
		}
		else
		{
			if (state.value == Player::sliding)
			{
				row = left.value ? 0 : 1;
				column = 0;
			}
			else if (state.value == Player::running)
			{
				row = 4;
				if (left.value && runTimer < runInterval / 2) { column = 0; }
				else if (left.value && runTimer >= runInterval / 2) { column = 1; }
				else if (!left.value && runTimer < runInterval / 2) { column = 2; }
				else if (!left.value && runTimer >= runInterval / 2) { column = 3; }
			}
			else if (state.value != Player::running)
			{
				row = 3;
				if (runTimer < runInterval / 2) { column = left.value ? 0 : 2; }
				else if (runTimer >= runInterval / 2) { column = left.value ? 1 : 3; }
				else if (!left.value) { column = 2; }
			}
		}

		return { column, row };
	}
};