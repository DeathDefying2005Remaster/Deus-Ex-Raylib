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

int frameCounta = 1;

double globalTime = 0;

class Player
{
	public:
	Vector2 pos;
	bool blue;

	PlayerTardis tardis;

	Player(bool _blue)
	{
		pos = { 400, 400 };
		tardis = PlayerTardis(pos);
		blue = _blue;
	}

	void DrawPlayer()
	{
		DrawRectangle(pos.x - (playerSize / 2), pos.y - (playerSize / 2), playerSize, playerSize, BLACK);
		DrawRectangle(pos.x - (playerSize / 2) + 5, pos.y - (playerSize / 2) + 5, playerSize - 10, playerSize - 10, blue ? BLUE : RED);
	}
	void DrawFakePlayer()
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

	globalTime = 0;
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		//---- Update Time ----//
		globalTime += player.blue ? -(GetFrameTime()) : GetFrameTime();

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
			Player sigma = Player(player.blue);
			sigma.tardis.list = player.tardis.list;
			fakePlayers.push_back(sigma);
			player.blue = !player.blue;
			player.tardis.list.Clear();
			player.tardis.currentIndex = 0;
		}

		//---- Movement ----//
		player.pos = Vector2Add(player.pos, Vector2Scale(moveInput, moveSpeed * GetFrameTime()));

		//---- Tracking ----//
		if (frameCounta == 1)
		{
			//tardis.timeHistory.push_back(GetTime());
			if (tardis.timeHistory.size() == 0 || globalTime < tardis.timeHistory.front())
			{
				tardis.timeHistory.insert(tardis.timeHistory.begin(), globalTime);
			}
			else if (globalTime > tardis.timeHistory.back())
			{
				tardis.timeHistory.insert(tardis.timeHistory.end(), globalTime);
			}
			if (player.blue)
			{
				player.tardis.list.value.insert(player.tardis.list.value.begin(), player.pos);
				player.tardis.list.reference.insert(player.tardis.list.reference.begin(), globalTime);
			}
			else
			{
				player.tardis.list.value.push_back(player.pos);
				player.tardis.list.reference.push_back(globalTime);
			}
			frameCounta = 1;
		}
		else
		{
			frameCounta++;
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(RAYWHITE);
		BeginMode2D(camera);

		player.DrawPlayer();

		if (fakePlayers.size() > 0)
		{
			for (int i = 0; i < fakePlayers.size(); i++)
			{
				double timmy = 0;
				if (fakePlayers[i].blue != player.blue)
				{
					double lastReference = fakePlayers[i].tardis.list.reference[fakePlayers[i].tardis.list.reference.size() - 1];
					timmy = lastReference - (GetTime() - lastReference);
				}
				else
				{
					timmy = GetTime() - (fakePlayers[i].tardis.list.reference[fakePlayers[i].tardis.list.reference.size() - 1] - fakePlayers[i].tardis.list.reference[0]);
				}
				//fakePlayers[i].pos = fakePlayers[i].tardis.GetPlayerPosition(timmy);
				if (fakePlayers[i].tardis.GetWithinBounds(globalTime))
				{
					fakePlayers[i].pos = fakePlayers[i].tardis.GetPlayerPosition(globalTime);
					fakePlayers[i].DrawFakePlayer();
				}
			}
		}

		DrawRectangle(15, 15, (GetScreenWidth() - 30) * ((0 - tardis.timeHistory[0]) / (tardis.timeHistory.back() - tardis.timeHistory[0])), 15, BLUE);
		DrawRectangle(15 + (GetScreenWidth() - 30) * ((0 - tardis.timeHistory[0]) / (tardis.timeHistory.back() - tardis.timeHistory[0])), 15, (GetScreenWidth() - 30) * ((tardis.timeHistory.back() - 0) / (tardis.timeHistory.back() - tardis.timeHistory[0])), 15, RED);
		float posX = (globalTime - tardis.timeHistory[0]) / (tardis.timeHistory.back() - tardis.timeHistory[0]);
		posX = posX * (GetScreenWidth() - 30) + 15;
		DrawTriangle({ posX, 30 }, { posX - 10, 50 }, { posX + 10, 50 }, player.blue ? BLUE : RED);

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}