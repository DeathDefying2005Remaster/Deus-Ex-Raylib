#include <raylib-cpp.hpp>
#include "redblacktree.hpp"
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;

RedBlackTree<int> tree;

void Start()
{
	camera.target = { 0, 0 };
	camera.offset = Vector2();
	camera.rotation = 0;
	camera.zoom = 1;

	tree = RedBlackTree<int>();
	tree.Insert(5);
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);
		BeginMode2D(camera);

		tree.Draw();

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}