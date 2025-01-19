#include <raylib-cpp.hpp>
#include "networking.hpp"
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

float moveSpeed = 5;

Camera2D camera;

NetworkVariable<Vector2> playerPosition;
NetworkVariable<Vector2> enemyPosition;

Server server;
Client client;
int type = 0;

void Start()
{
	camera.target = { 0, 0 };
	camera.offset = Vector2();
	camera.rotation = 0;
	camera.zoom = 1;

	playerPosition.value = { screenWidth / 2, screenHeight / 2 };
	enemyPosition.value = { -100, -100 };
}

void InitializeNetworkVariables()
{
	if (type == 1)
	{
		playerPosition = NetworkVariable<Vector2>(playerPosition.value, 1, true, &server, &playerPosition);
		enemyPosition = NetworkVariable<Vector2>(enemyPosition.value, 2, false, &server, &enemyPosition);
	}
	else if (type == 2)
	{
		playerPosition = NetworkVariable<Vector2>(playerPosition.value, 2, true, &client, &playerPosition);
		enemyPosition = NetworkVariable<Vector2>(enemyPosition.value, 1, false, &client, &enemyPosition);
	}
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "Cheryl took the kids and the Corvette.");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		//---- Input ----//
		if (IsKeyPressed(KEY_H) && type == 0)
		{
			type = 1;
			server.Start();
			InitializeNetworkVariables();
		}
		if (IsKeyPressed(KEY_C) && type == 0)
		{
			type = 2;
			client.Start();
			InitializeNetworkVariables();
		}
		if (IsKeyPressed(KEY_D) && type != 0)
		{
			type == 1 ? server.Kill() : client.Kill();
		}

		Vector2 dir = {};
		if (IsKeyDown(KEY_LEFT)) { dir.x -= 1; }
		if (IsKeyDown(KEY_RIGHT)) { dir.x += 1; }
		if (IsKeyDown(KEY_UP)) { dir.y -= 1; }
		if (IsKeyDown(KEY_DOWN)) { dir.y += 1; }
		dir = Vector2Normalize(dir);
		playerPosition.value = Vector2Add(playerPosition.value, Vector2Scale(dir, moveSpeed));

		//---- Networking ----//
		if (type != 0)
		{
			type == 1 ? server.Run() : client.Run();
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);
		BeginMode2D(camera);

		Color color = BLACK;
		Color enemyColor = BLACK;
		if (type == 1) { color = BLUE; enemyColor = RED; }
		if (type == 2) { color = RED; enemyColor = BLUE; }
		
		DrawRectangle(enemyPosition.value.x - 35, enemyPosition.value.y - 35, 70, 70, enemyColor);
		DrawRectangle(playerPosition.value.x - 35, playerPosition.value.y - 35, 70, 70, color);

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}