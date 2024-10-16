#include <raylib-cpp.hpp>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "event.h"
#include "tardis.h"
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;
Tardis tardis = Tardis();

int playerSize = 70;
float moveSpeed = 300;

int frameCounta = 0;

class Player
{
	public:
	Vector2 pos;
	bool blue;

	PlayerTardis tardis = PlayerTardis({ pos });

	Player(bool _blue)
	{
		pos = { 400, 400 };
		blue = _blue;
	}

	void Draw()
	{
		DrawRectangle(pos.x - (playerSize / 2), pos.y - (playerSize / 2), playerSize, playerSize, blue ? BLUE : RED);
	}
};
Player player = Player(false);
vector<Player> fakePlayers = {};

void Start()
{
	player = Player(false);

	camera.target = { 0, 0 };
	camera.offset = Vector2();
	camera.rotation = 0;
	camera.zoom = 1;
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		//---- Input ----//
		Vector2 moveInput = Vector2();

		if (IsKeyDown(KEY_LEFT))
		{
			moveInput.x -= 1;
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			moveInput.x += 1;
		}
		if (IsKeyDown(KEY_UP))
		{
			moveInput.y -= 1;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			moveInput.y += 1;
		}

		if (IsKeyPressed(KEY_SPACE))
		{
			fakePlayers.push_back(Player(player.blue));
			fakePlayers[fakePlayers.size() - 1].tardis.list.value = player.tardis.list.value;
			player.blue = !player.blue;
			player.tardis.list.value.clear();
		}

		//---- Movement ----//
		player.pos = Vector2Add(player.pos, Vector2Scale(moveInput, moveSpeed * GetFrameTime()));

		//---- Tracking ----//
		if (fakePlayers.size() == 0)
		{
			if (frameCounta == 3)
			{
				tardis.timeHistory.push_back(GetTime());
				player.tardis.list.value.push_back(player.pos);
				frameCounta = 1;
			}
			else
			{
				frameCounta++;
			}
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(RAYWHITE);
		BeginMode2D(camera);

		player.Draw();

		if (fakePlayers.size() > 0)
		{
			for (int i = 0; i < fakePlayers.size(); i++)
			{
				double timmy = tardis.timeHistory[tardis.timeHistory.size() - 1] - (GetTime() - fakePlayers[i].tardis.list.startTime);
				fakePlayers[i].pos = fakePlayers[i].tardis.GetPlayerPosition(tardis, timmy);
				fakePlayers[i].Draw();

				string str = to_string(fakePlayers[i].tardis.list.currentIndex) + " : ";
				printf(str.c_str());
			}
		}

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}