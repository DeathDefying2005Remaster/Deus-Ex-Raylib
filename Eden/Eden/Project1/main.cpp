#include <raylib-cpp.hpp>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;

int iterations = 50;

class Joint
{
	public:

	Vector2 pos;
	float radius;
	float boneLength;
	Vector2 boneDir;

	Joint()
	{
		pos = { (int)screenWidth * 0.5, (int)screenHeight * 0.5 };
		radius = 20;
		boneLength = 100;
	}
	Joint(Vector2 _pos, float _rad)
	{
		pos = _pos;
		radius = _rad;
		boneLength = 100;
	}

	void Draw()
	{
		DrawCircle(pos.x, pos.y, radius, BLACK);
		DrawCircle(pos.x, pos.y, radius * 0.6, WHITE);
	}
};

class Target
{
	public:

	Vector2 pos;
	float radius;
	bool held;
	Vector2 holdOffset;

	Target()
	{
		pos = { (int)screenWidth * 0.5, (int)screenHeight * 0.5 };
		radius = 20;
		held = false;
	}
	Target(Vector2 _pos, float _rad)
	{
		pos = _pos;
		radius = _rad;
		held = false;
	}

	void Draw()
	{
		DrawCircle(pos.x, pos.y, radius, BLACK);
		DrawCircle(pos.x, pos.y, radius * 0.6, SKYBLUE);
	}
	bool MouseCheck()
	{
		return Vector2Distance(GetMousePosition(), pos) <= radius;
	}
	void StartHold()
	{
		held = true;
		holdOffset = Vector2Subtract(pos, GetMousePosition());
	}
	void EndHold()
	{
		held = false;
	}
	void ConstrainPos()
	{
		if (pos.x < 0)
		{
			pos.x = 0;
		}
		else if (pos.x > screenWidth)
		{
			pos.x = screenWidth;
		}

		if (pos.y < 0)
		{
			pos.y = 0;
		}
		else if (pos.y > screenHeight)
		{
			pos.y = screenHeight;
		}
	}
};
Vector2 root = { 400, 400 };
vector<Joint> chain = { Joint(), Joint(), Joint({500, 300}, 20), Joint({550, 250}, 20)};
Target target = Target({ 600, 200 }, 15);

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
		//---- Input ----//
		if (IsMouseButtonPressed(0))
		{
			if (target.MouseCheck())
			{
				target.StartHold();
			}
		}
		if (IsMouseButtonReleased(0))
		{
			if (target.held)
			{
				target.EndHold();
			}
		}

		//---- Mouse Movement ----//
		if (target.held)
		{
			target.pos = Vector2Add(GetMousePosition(), target.holdOffset);
			target.ConstrainPos();
		}

		//---- FABRIK ----//
		for (int i = 0; i < 5; i++)
		{
			chain[chain.size() - 1].pos = target.pos;
			chain[chain.size() - 1].boneDir = Vector2Normalize(Vector2Subtract(chain[chain.size() - 2].pos, chain[chain.size() - 1].pos));
			for (int j = chain.size() - 2; j > 0; j--)
			{
				chain[j].pos = Vector2Add(chain[j + 1].pos, Vector2Scale(chain[j + 1].boneDir, chain[j + 1].boneLength));
				chain[j].boneDir = Vector2Normalize(Vector2Subtract(j == 0 ? root : chain[j - 1].pos, chain[j].pos));
				
				float angle = Vector2Angle(chain[j].boneDir, chain[j + 1].boneDir);
				if (angle > DEG2RAD * 160)
				{
					float difference = (DEG2RAD * 160) - angle;
					chain[j].boneDir = Vector2Rotate(chain[j].boneDir, -difference);
				}
				if (angle < DEG2RAD * -160)
				{
					float difference = (DEG2RAD * -160) - angle;
					chain[j].boneDir = Vector2Rotate(chain[j].boneDir, -difference);
				}
			}
			chain[0].pos = Vector2Add(chain[1].pos, Vector2Scale(chain[1].boneDir, chain[1].boneLength));
			
			float angle = 0;
			for (int i = 2; i < chain.size(); i++)
			{
				angle += (180 * DEG2RAD) - Vector2Angle(chain[i].boneDir, chain[i - 1].boneDir);
			}
			angle += (180 * DEG2RAD) - Vector2Angle(chain[chain.size() - 1].boneDir, chain[0].boneDir);
			printf(to_string(angle * RAD2DEG).c_str());
			printf(" : ");

			if (angle > DEG2RAD * 180)
			{
				float difference = (DEG2RAD * 180) - angle;
				Vector2Rotate(chain[0].boneDir, -difference);
			}
			
			chain[0].pos = root;
			chain[0].boneDir = Vector2Normalize(Vector2Subtract(chain[1].pos, chain[0].pos));
			for (int j = 1; j < chain.size() - 1; j++)
			{
				chain[j].pos = Vector2Add(chain[j - 1].pos, Vector2Scale(chain[j - 1].boneDir, chain[j - 1].boneLength));
				chain[j].boneDir = Vector2Normalize(Vector2Subtract(chain[j + 1].pos, chain[j].pos));

				float angle = Vector2Angle(chain[j].boneDir, chain[j - 1].boneDir);
				if (angle > DEG2RAD * 160)
				{
					float difference = (DEG2RAD * 160) - angle;
					chain[j].boneDir = Vector2Rotate(chain[j].boneDir, -difference);
				}
				if (angle < DEG2RAD * -160)
				{
					float difference = (DEG2RAD * -160) - angle;
					chain[j].boneDir = Vector2Rotate(chain[j].boneDir, -difference);
				}
			}
			chain[chain.size() - 1].pos = Vector2Add(chain[chain.size() - 2].pos, Vector2Scale(chain[chain.size() - 2].boneDir, chain[chain.size() - 2].boneLength));

			angle = 0;
			for (int i = 2; i < chain.size(); i++)
			{
				angle += (180 * DEG2RAD) - Vector2Angle(chain[i].boneDir, chain[i - 1].boneDir);
			}
			angle += (180 * DEG2RAD) - Vector2Angle(chain[chain.size() - 1].boneDir, chain[0].boneDir);

			if (angle > DEG2RAD * 180)
			{
				float difference = (DEG2RAD * 180) - angle;
				Vector2Rotate(chain[chain.size() - 1].boneDir, -difference);
			}
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);
		BeginMode2D(camera);

		for (int i = 0; i < chain.size(); i++)
		{
			if (i != 0)
			{
				DrawLineEx(chain[i - 1].pos, chain[i].pos, 14, BLACK);
			}
		}
		for (int i = 0; i < chain.size(); i++)
		{
			chain[i].Draw();
		}

		target.Draw();

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}