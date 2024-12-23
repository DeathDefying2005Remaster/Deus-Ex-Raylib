#include <raylib-cpp.hpp>

#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include "client.hpp"
#include "server.hpp"
#include "playerData.hpp"

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;

int type = 0; //client = 1 host = 2

Client client;
Server server;

Vector2 position;
vector<PlayerData> peerInfo;
float speed = 4;

void Start()
{
	camera.target = { 0, 0 };
	camera.offset = Vector2();
	camera.rotation = 0;
	camera.zoom = 1;
	position = { screenWidth*0.5, screenHeight*0.5 };
	peerInfo = {};
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_C) && type == 0)
		{
			type = 1;
			client.StartClient(&peerInfo);
		}
		else if (IsKeyPressed(KEY_H) && type == 0)
		{
			type = 2;
			server.StartServer();
			printf("server start\n");
		}

		if (type != 0)
		{
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

			position = Vector2Add(position, Vector2Scale(moveInput, speed));
		}

		if (type == 1) { client.RunClient(&peerInfo, position); }
		else if (type == 2) { server.RunServer(&peerInfo, position); }

		if (IsKeyPressed(KEY_D) && type == 1)
		{
			client.DisconnectClient();
		}
		else if (IsKeyPressed(KEY_D) && type == 2)
		{
			server.DestroyServer();
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);
		BeginMode2D(camera);

		if (type != 0)
		{
			for (int i = 0; i < peerInfo.size(); i++)
			{
				DrawRectangle(peerInfo[i].position.x - 30, peerInfo[i].position.y - 30, 60, 60, RED);
			}
			DrawRectangle(position.x - 30, position.y - 30, 60, 60, BLUE);
		}

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}