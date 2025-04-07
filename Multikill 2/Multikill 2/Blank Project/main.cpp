#define _HAS_STD_BYTE 0
#include <raylib-cpp.hpp>
#include "global.hpp"
#include "global2.hpp"
#include "netcode.hpp"
#include "player.hpp"
#include "objectPool.hpp"
#include "bulletEffects.hpp"
#include "enemyPlayer.hpp"
#include "inputField.hpp"
using namespace std;

const int screenWidth = 1000;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;
MotherOfTheTrucker* networking = nullptr;
int networkType = 0;
Player player;
PlayerInfo playerInfo;
PlayerInfo enemyInfo;
InputField field = InputField({ screenWidth / 2, 200 }, { 140, 20 }, WHITE, LIGHTGRAY, BLACK);
//NetworkVariable<Vector2> playerPos;
//NetworkVariable<Vector2> otherPlayerPos;

void Start()
{
	camera.target = {};
	camera.offset = { screenWidth / 2, screenHeight / 2 };
	camera.rotation = 0;
	camera.zoom = 1;
	sheet = LoadTexture("resources/veeonetwo.png");
	//bradPitt = ObjectPool<BulletTrail>();
}

void SkadooshNetworkVariables()
{
	if (networkType == 1)
	{
		sheet = LoadTexture("resources/veeonetwo.png");
		playerInfo = PlayerInfo();
		playerInfo.sheet = sheet;
		playerInfo.position = NetworkVariable<Vector2>({}, 1, true, networking, &playerInfo.position);
		playerInfo.shootDir = NetworkVariable<Vector2>({}, 2, true, networking, &playerInfo.shootDir);
		playerInfo.state = NetworkVariable<enet_uint32>(0, 3, true, networking, &playerInfo.state);
		playerInfo.left = NetworkVariable<bool>(false, 4, true, networking, &playerInfo.left);

		enemyInfo = PlayerInfo();
		enemyInfo.sheet = LoadTexture("resources/veetwoone.png");
		enemyInfo.position = NetworkVariable<Vector2>({}, 5, false, networking, &enemyInfo.position);
		enemyInfo.shootDir = NetworkVariable<Vector2>({}, 6, false, networking, &enemyInfo.shootDir);
		enemyInfo.state = NetworkVariable<enet_uint32>(0, 7, false, networking, &enemyInfo.state);
		enemyInfo.left = NetworkVariable<bool>(false, 8, false, networking, &enemyInfo.left);

		//playerPos = NetworkVariable<Vector2>({ 200, 200 }, 1, true, networking, &playerPos);
		//otherPlayerPos = NetworkVariable<Vector2>({ 200, 200 }, 2, false, networking, &otherPlayerPos);
	}
	else if (networkType == 2)
	{
		sheet = LoadTexture("resources/veetwoone.png");
		playerInfo = PlayerInfo();
		playerInfo.sheet = sheet;
		playerInfo.position = NetworkVariable<Vector2>({}, 5, true, networking, &playerInfo.position);
		playerInfo.shootDir = NetworkVariable<Vector2>({}, 6, true, networking, &playerInfo.shootDir);
		playerInfo.state = NetworkVariable<enet_uint32>(0, 7, true, networking, &playerInfo.state);
		playerInfo.left = NetworkVariable<bool>(false, 8, true, networking, &playerInfo.left);

		enemyInfo = PlayerInfo();
		enemyInfo.sheet = LoadTexture("resources/veeonetwo.png");
		enemyInfo.position = NetworkVariable<Vector2>({}, 1, false, networking, &enemyInfo.position);
		enemyInfo.shootDir = NetworkVariable<Vector2>({}, 2, false, networking, &enemyInfo.shootDir);
		enemyInfo.state = NetworkVariable<enet_uint32>(0, 3, false, networking, &enemyInfo.state);
		enemyInfo.left = NetworkVariable<bool>(false, 4, false, networking, &enemyInfo.left);

		//playerPos = NetworkVariable<Vector2>({ 200, 200 }, 2, true, networking, &playerPos);
		//otherPlayerPos = NetworkVariable<Vector2>({ 200, 200 }, 1, false, networking, &otherPlayerPos);
	}
}

int main()
{
	//---- Start ----//
	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);
	Start();

	while (!WindowShouldClose())
	{
		//---- Networking Input ----//
		if (IsKeyPressed(KEY_H))
		{
			if (networking == nullptr)
			{
				networking = new Server();
				networking->Start("");
				networkType = 1;
				SkadooshNetworkVariables();
			}
			else
			{
				networking->Kill();
				delete networking;
				networking = nullptr;
				networkType = 0;
			}
		}
		else if (IsKeyPressed(KEY_C))
		{
			if (networking == nullptr)
			{
				networking = new Client();
				networking->Start(field.text);
				networkType = 2;
				SkadooshNetworkVariables();
			}
			else
			{
				networking->Kill();
				delete networking;
				networking = nullptr;
				networkType = 0;
			}
		}
		//---- Input ----//


		//---- Player ----//
		player.Update();

		//---- UI ----//
		if (IsMouseButtonPressed(0)) { field.Click(); }
		field.TypeInput();

		//---- Networking ----//
		playerInfo.position.value = player.position;
		playerInfo.shootDir.value = player.shootDir;
		playerInfo.state.value = player.state;
		playerInfo.left.value = player.left;

		if (networking != nullptr) { networking->Run(); }

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);

		field.Draw();

		BeginMode2D(camera);

		for (int i = 0; i < blocks.size(); i++)
		{
			DrawRectangleRec(blocks[i], BLACK);
		}
		for (int i = 0; i < bradPitt.pool.size(); i++)
		{
			bradPitt.pool[i].Update();
			if (bradPitt.pool[i].timer > bradPitt.pool[i].lifespan)
			{
				bradPitt.Remove(i);
			}
			else
			{
				bradPitt.pool[i].Draw();
			}
		}
		enemyInfo.Draw();
		player.Draw();

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}