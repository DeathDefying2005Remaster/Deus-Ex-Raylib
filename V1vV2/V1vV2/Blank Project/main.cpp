#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <raylib-cpp.hpp>
#include "machine.hpp"
#include "shapeDefiner.hpp"
#include "networking2.hpp"
#include <algorithm>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;

Machine machine = Machine({0, 0});
float gravity = 25;

vector<raylib::Rectangle> blocks = Box({ 0, 0 }, { 2000, 1500 }, 50);
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
	float length;
	float speed;

	BulletTrail(Vector2 _origin, Vector2 _direction, float _length, float _speed)
	{
		origin = _origin;
		direction = _direction;
		length = _length;
		speed = _speed;
	}

	void Update()
	{
		length += speed * GetFrameTime();
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
	camera.zoom = 0.3;
}

void EnterGround(Vector2 moveInput)
{
	machine.velocity.y = 0;
	if (machine.grounded == false)
	{
		//I get knocked down, but I get up again, you will never keep me down
	}
	machine.grounded = true;
}
void LeaveGround()
{
	if (machine.grounded == true)
	{
		//"Liftoff!" said the meat worm.
		if (machine.velocity.x == 0)
		{
			machine.velocity.x += (machine.left ? -machine.appliedVelocity : machine.appliedVelocity);
		}
		else
		{
			float leFaktor = 600 / Clamp(abs(machine.velocity.x), 600, 1200);
			machine.velocity.x += (machine.left ? -machine.appliedVelocity : machine.appliedVelocity) * leFaktor;
		}
		machine.appliedVelocity = 0;
		machine.state = machine.idle;
	}
	machine.grounded = false;
}

void GroundCheck(Vector2 moveInput)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (machine.rect.y + machine.rect.height >= blocks[i].y - machine.groundMargin && machine.rect.y < blocks[i].y)
		{
			if (machine.rect.x + machine.rect.width - 15 > blocks[i].x && machine.rect.x + 15 < blocks[i].x + blocks[i].width)
			{
				EnterGround(moveInput);
				return;
			}
		}
	}

	LeaveGround();
}

void Shoot(Vector2 input)
{
	machine.shootTimer = machine.shootCooldown;
	machine.shootDir = input;
	BulletTrail trail = BulletTrail(machine.position, Vector2Normalize({ input.x, -input.y }), 0, 400);
	bradPitt.Add(trail);
}

int main()
{
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "someone's getting fired");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		//---- Input ----//
		Vector2 moveInput = Vector2();
		if (IsKeyDown(KEY_A)) { moveInput.x -= 1; }
		if (IsKeyDown(KEY_D)) { moveInput.x += 1; }

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP))
		{
			Vector2 attackInput = Vector2();
			if (IsKeyPressed(KEY_LEFT)) { attackInput.x -= 1; }
			if (IsKeyPressed(KEY_RIGHT)) { attackInput.x += 1; }
			if (IsKeyPressed(KEY_UP)) { attackInput.y = 1; }
			Shoot(attackInput);
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
				client.Start();
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
		machine.rect = raylib::Rectangle(machine.position.x - 50, machine.position.y - 50, 100, 50 + 64 * 3 * 0.5);
		for (int i = 0; i < blocks.size(); i++)
		{
			if (machine.rect.x > blocks[i].x - machine.rect.width && machine.rect.x < blocks[i].x + blocks[i].width)
			{
				if (machine.rect.y > blocks[i].y - machine.rect.height && machine.rect.y < blocks[i].y + blocks[i].height)
				{
					//check each crossover and pick smallest one
					float left = blocks[i].x - (machine.rect.x + machine.rect.width);
					float right = (blocks[i].x + blocks[i].width) - machine.rect.x;
					float up = blocks[i].y - (machine.rect.y + machine.rect.height);
					float down = (blocks[i].y + blocks[i].height) - machine.rect.y;
					vector<float> arrgh = { abs(left), abs(right), abs(up), abs(down) };
					float min = *min_element(arrgh.begin(), arrgh.end());
					if (min == abs(left) || min == abs(right))
					{
						machine.position.x += min == abs(left) ? -min : min;
						if (machine.wallJumpTimer == 0) { machine.velocityBuffer = machine.velocity; }
						machine.velocity.x = 0;
						machine.wallJumpTimer = machine.wallJumpBuffer;
					}
					else
					{
						machine.position.y += min == abs(up) ? -min : min;
						machine.velocityBuffer = machine.velocity;
						machine.velocity.y = 0;
					}
				}
			}
		}
		GroundCheck(moveInput);
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

		machine.Draw();
		for (int i = 0; i < blocks.size(); i++)
		{
			DrawRectangleRec(blocks[i], BLACK);
		}
		//DrawRectangleRec(machine.rect, RED);
		for (int i = 0; i < bradPitt.pool.size(); i++)
		{
			bradPitt.Get(i).Update();
			bradPitt.Get(i).Draw();
		}

		DrawCircleV(player1Pos.value, 20, BLUE);
		DrawCircleV(player2Pos.value, 20, RED);

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}