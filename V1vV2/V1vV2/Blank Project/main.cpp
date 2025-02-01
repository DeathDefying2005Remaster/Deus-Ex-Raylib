#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <raylib-cpp.hpp>
#include "machine.hpp"
#include "shapeDefiner.hpp"
#include "networking2.hpp"
#include "chassis.hpp"
#include <algorithm>
using namespace std;

const int screenWidth = 1920;
const int screenHeight = 1080;
const int targetFps = 60;

Camera2D camera;

Machine machine = Machine({0, 0});
float gravity = 25;

vector<raylib::Rectangle> blocks = Box({ 0, 0 }, { 800, 500 }, 25);
//vector<raylib::Rectangle> blocks = { raylib::Rectangle(-5000, 100, 10000, 50) };

Server server;
Client client;
int type = 0;

NetworkVariable<Vector2> player1Pos;
NetworkVariable<Vector2> player2Pos;

template <typename T> class ObjectPool
{
	public:

	vector<T> pool;

	void Add(T value)
	{
		pool.push_back(value);
	}
	void Remove(int index)
	{
		pool.erase(pool.begin() + index);
	}
	T& Get(int index)
	{
		return pool[index];
	}
};

class BulletTrail
{
	public:

	Vector2 origin;
	Vector2 direction;
	float lifespan;
	float timer;
	float length;

	BulletTrail(Vector2 _origin, Vector2 _direction, float _lifespan)
	{
		origin = _origin;
		direction = _direction;
		lifespan = _lifespan;
		timer = 0;

		for (int i = 0; i < blocks.size(); i++)
		{
			Vector3 originbutnot = { origin.x, origin.y, 0 };
			Vector3 directionbutnot = { direction.x, direction.y, 0 };
			Vector3 pos = { blocks[i].GetPosition().x, blocks[i].GetPosition().y, 0 };
			Vector3 pos2 = { blocks[i].GetPosition().x + blocks[i].width, blocks[i].GetPosition().y + blocks[i].height, 0 };
			BoundingBox box = raylib::BoundingBox(pos, pos2);
			RayCollision col = GetRayCollisionBox(raylib::Ray(originbutnot, directionbutnot), box);
			if (col.hit) { length = col.distance; }
			else { length = 2000; }
		}
	}

	void Update()
	{
		timer += GetFrameTime();
	}
	void Draw()
	{
		DrawLineEx(origin, { origin.x + (direction.x * length), origin.y + (direction.y * length) }, 5, YELLOW);
	}
};
ObjectPool<BulletTrail> bradPitt;

void Start()
{
	camera.target = machine.position;
	camera.offset = { screenWidth/2, screenHeight/2 };
	camera.rotation = 0;
	camera.zoom = 2;
}

void Shoot()
{
	machine.shootTimer = machine.shootCooldown;
	BulletTrail trail = BulletTrail(machine.position, Vector2Normalize({ machine.shootDir.x, -machine.shootDir.y }), 10);
	bradPitt.Add(trail);
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "someone's getting fired");
	SetTargetFPS(targetFps);
	ToggleFullscreen();

	while (!WindowShouldClose())
	{
		//---- Input ----//
		Vector2 moveInput = Vector2();
		if (IsKeyDown(KEY_A)) { moveInput.x -= 1; }
		if (IsKeyDown(KEY_D)) { moveInput.x += 1; }

		Vector2 shootDir = Vector2();
		if (IsKeyPressed(KEY_LEFT)) { shootDir.x -= 1; }
		if (IsKeyPressed(KEY_RIGHT)) { shootDir.x += 1; }
		if (IsKeyPressed(KEY_UP)) { shootDir.y = 1; }
		if (!Vector2Equals(shootDir, {}))
		{
			machine.shootDir = shootDir;
			Shoot();
		}
		else
		{
			if (machine.shootTimer <= 0)
			{
				machine.shootDir = shootDir;
			}
		}

		if (IsKeyPressed(KEY_H))
		{
			if (type == 0)
			{
				type = 1;
				server = Server();
				server.Start();
				player1Pos = NetworkVariable<Vector2>({200, 200}, 1, true, &server, &player1Pos);
				player2Pos = NetworkVariable<Vector2>({200, 200}, 2, false, &server, &player2Pos);
			}
			else
			{
				server.Kill();
			}
		}
		else if (IsKeyPressed(KEY_C))
		{
			if (type == 0)
			{
				type = 2;
				client = Client();
				client.Start("");
				player1Pos = NetworkVariable<Vector2>({200, 200}, 1, false, &client, &player1Pos);
				player2Pos = NetworkVariable<Vector2>({200, 200}, 2, true, &client, &player2Pos);
			}
			else
			{
				client.Kill();
			}
		}

		if (type == 1) 
		{
			player1Pos.value = machine.position;
			server.Run(); 
		}
		else if (type == 2) 
		{ 
			player2Pos.value = machine.position;
			client.Run(); 
		}

		//---- Handle States ----//
		if (moveInput.x < 0) { machine.left = true; }
		else if (moveInput.x > 0) { machine.left = false; }

		if (IsKeyPressed(KEY_LEFT_CONTROL)) { machine.state = machine.sliding; }
		if (IsKeyReleased(KEY_LEFT_CONTROL)) { machine.state = machine.idle; }
		
		else if (moveInput.x != 0 && machine.state != machine.sliding) { machine.state = machine.running; }
		else if (machine.state != machine.sliding) { machine.state = machine.idle; }
			
		//---- Movement ----//
		if (!machine.grounded)
		{
			machine.velocity.y += gravity;
		}
		machine.HorizontalMovement2(moveInput);

		//---- Grounded Stuff and Collisions ----//
		SetMachineRect(&machine);
		machine.Collision(blocks);
		machine.GroundCheck(moveInput, blocks);
		DrawText(to_string(machine.grounded).c_str(), 400, 20, 20, BLACK);

		//---- Input 2: Electric Boogaloo ----//
		if (IsKeyPressed(KEY_SPACE))
		{
			if (machine.grounded)
			{
				machine.Jump();
			}
			else if (machine.WallCheck(blocks) != 0)
			{
				machine.WallJump(machine.WallCheck(blocks));
			}
		}

		//---- Timers ----//
		machine.Timers();

		DrawText(to_string(machine.state).c_str(), 500, 20, 20, BLACK);

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(GRAY);
		//camera.target = machine.position;
		BeginMode2D(camera);
			
		for (int i = 0; i < blocks.size(); i++)
		{
			DrawRectangleRec(blocks[i], BLACK);
		}
		SetMachineRect(&machine);
		DrawRectangleRec(machine.rect, RED);
		DrawMachine(&machine);
		for (int i = 0; i < bradPitt.pool.size(); i++)
		{
			bradPitt.Get(i).Update();
			bradPitt.Get(i).Draw();
		}

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}