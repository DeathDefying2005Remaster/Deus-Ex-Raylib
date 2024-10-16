#include <raylib-cpp.hpp>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera3D camera;

Vector2 mousePos;
float sensitivityX = 0.005;
float sensitivityY = 0.005;
float speed = 0.1;

void Start()
{
	mousePos = {};

	camera.position = { 0, 2, 0};
	camera.target = { 0, 2, 1 };
	camera.up = { 0, 1, 0};
	camera.fovy = 60;
	camera.projection = CAMERA_PERSPECTIVE;
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);

	DisableCursor();

	while (!WindowShouldClose())
	{
		//---- Input ----//

		Vector2 input = { 0, 0 };
		if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
		{
			input.x = -1;
		}
		else if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
		{
			input.x = 1;
		}
		if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
		{
			input.y = 1;
		}
		else if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W))
		{
			input.y = -1;
		}
		Vector3 rawCamDir = Vector3Subtract(camera.target, camera.position);
		Vector3 camFlatForward = Vector3Scale({ rawCamDir.x, 0, rawCamDir.z }, input.y);
		Vector3 camFlatRight = Vector3Scale(Vector3RotateByAxisAngle({ rawCamDir.x, 0, rawCamDir.z }, { 0, 1, 0 }, DEG2RAD * -90), input.x);
		Vector3 moveDir = Vector3Normalize(Vector3Add(camFlatForward, camFlatRight));
		camera.position = Vector3Add(camera.position, Vector3Scale(moveDir, speed));

		mousePos.x -= GetMouseDelta().x * sensitivityX;
		mousePos.y -= GetMouseDelta().y * sensitivityY;
		Clamp(mousePos.y, -90, 90);
		Vector3 target = Vector3Normalize({ sin(mousePos.x), sin(mousePos.y), cos(mousePos.x) });
		camera.target = Vector3Add(camera.position, target);

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(WHITE);
		BeginMode3D(camera);

		DrawCubeV({ 0, 0.5, 2}, { 1, 1, 1 }, DARKBLUE);

		EndMode3D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}