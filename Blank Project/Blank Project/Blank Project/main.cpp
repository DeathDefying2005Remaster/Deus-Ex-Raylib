#include <raylib-cpp.hpp>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;

void Start()
{
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
		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);
		BeginMode2D(camera);

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}