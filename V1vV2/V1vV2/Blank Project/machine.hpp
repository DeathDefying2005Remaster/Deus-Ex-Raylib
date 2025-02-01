#pragma once
#include <raylib-cpp.hpp>
using namespace std;

class Machine
{
	public:

	Vector2 position;
	Vector2 velocity;
	float appliedVelocity = 0;
	Vector2 velocityBuffer;

	enum MachineState
	{
		idle,
		running,
		sliding,
	};
	MachineState state;
	bool grounded;
	bool left;
	Vector2 shootDir;
	float runTimer;
	float wallJumpBuffer = 1;
	float wallJumpTimer;
	float wallJumpMargin = 30;
	float groundMargin = 1;
	float shootCooldown = 0.5;
	float shootTimer = 0;
	float runInterval = 0.5;
	float horizontalForce = 9000;
	float maxHorizontalVelocity = 600;
	float drag = 9000;
	float slideHorizontalForce = 0;
	float slideMaxHorizontalVelocity = 1200;
	float slideDrag = 7000;
	float airHorizontalForce = 1000;
	float airMaxHorizontalVelocity = 200;
	float airDrag = 1000;
	float jumpForce = 500;
	float mainDrag = 5000;

	raylib::Rectangle rect = raylib::Rectangle();

	Machine(Vector2 _position)
	{
		position = _position;
		velocity = Vector2();
		shootDir = Vector2();
		velocityBuffer = velocity;
		appliedVelocity = 0;
		state = idle;
		grounded = true;
		left = true;
		runTimer = 0;
		wallJumpTimer = 0;
	}

	int WallCheck(vector<raylib::Rectangle> blocks)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if ((rect.y + rect.height / 2) > blocks[i].y && (rect.y + rect.height / 2) < (blocks[i].y + blocks[i].height))
			{
				if (abs(rect.x - (blocks[i].x + blocks[i].width)) <= wallJumpMargin)
				{
					//wall on the left
					return -1;
				}
				else if (abs((rect.x + rect.width) - blocks[i].x) <= wallJumpMargin)
				{
					//wall on the right
					return 1;
				}
			}
		}
		return 0;
	}

	void HorizontalMovement2(Vector2 moveInput)
	{
		float force;
		float max;
		float drag;

		if (grounded)
		{
			if (state == sliding)
			{
				force = slideHorizontalForce;
				max = slideMaxHorizontalVelocity;
				drag = slideDrag;
			}
			else
			{
				force = horizontalForce;
				max = maxHorizontalVelocity;
				drag = this->drag;
			}
		}
		else
		{
			force = airHorizontalForce;
			max = airMaxHorizontalVelocity;
			drag = airDrag;
		}

		if (state == sliding && grounded)
		{
			appliedVelocity = slideMaxHorizontalVelocity;
			velocity.x = left ? -abs(velocity.x) : abs(velocity.x);
		}
		else
		{
			if (moveInput.x != 0)
			{
				appliedVelocity += force * GetFrameTime();
				appliedVelocity = Clamp(appliedVelocity, 0, max);
			}
			else if (appliedVelocity != 0 && grounded)
			{
				appliedVelocity *= Clamp(1 - (GetFrameTime() * drag / abs(appliedVelocity)), 0, 1);
			}
		}

		if (velocity.x != 0 && grounded)
		{
			if (moveInput.x == 0 || state == sliding)
			{
				velocity.x *= Clamp(1 - (GetFrameTime() * mainDrag / abs(velocity.x)), 0, 1);
			}
			else
			{
				velocity.x = 0;
			}
		}

		float scaledAppliedVelocity = appliedVelocity;
		if (velocity.x < 0 && left || velocity.x > 0 && !left)
		{
			scaledAppliedVelocity = Clamp(appliedVelocity, 0, Clamp(max - abs(velocity.x), 0, max));
		}
		Vector2 scaledForce = Vector2Scale({ velocity.x + ((left ? -1 : 1) * scaledAppliedVelocity), velocity.y }, GetFrameTime());
		position = Vector2Add(position, scaledForce);

		DrawText(to_string(appliedVelocity).c_str(), 20, 20, 20, BLACK);
		DrawText(to_string(velocity.x).c_str(), 200, 20, 20, BLACK);
		DrawText(to_string(scaledForce.x / GetFrameTime()).c_str(), 600, 20, 20, BLACK);
	}

	void Draw()
	{
		Image sheetImage = LoadImage("resources/veeonetwo.png");

		int frameWidth = 96;
		int frameHeight = 64;
		float scaleFactor = 3;

		Rectangle sourceRec = Rectangle();
		Rectangle destRec = { position.x - frameWidth*scaleFactor*0.5, position.y - frameHeight*scaleFactor*0.5, scaleFactor * frameWidth, scaleFactor * frameHeight };
		Vector2 origin = { 0, 0 };
		int rotation = 0;

		Texture2D sheet = LoadTextureFromImage(sheetImage);
		DrawTexturePro(sheet, sourceRec, destRec, origin, (float)rotation, WHITE);
	}

	void Collision(vector<raylib::Rectangle> blocks)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if (rect.x > blocks[i].x - rect.width && rect.x < blocks[i].x + blocks[i].width)
			{
				if (rect.y > blocks[i].y - rect.height && rect.y < blocks[i].y + blocks[i].height)
				{
					//check each crossover and pick smallest one
					float left = blocks[i].x - (rect.x + rect.width);
					float right = (blocks[i].x + blocks[i].width) - rect.x;
					float up = blocks[i].y - (rect.y + rect.height);
					float down = (blocks[i].y + blocks[i].height) - rect.y;
					vector<float> arrgh = { abs(left), abs(right), abs(up), abs(down) };
					float min = *min_element(arrgh.begin(), arrgh.end());
					if (min == abs(left) || min == abs(right))
					{
						position.x += min == abs(left) ? -min : min;
						if (wallJumpTimer == 0) { velocityBuffer = velocity; }
						velocity.x = 0;
						wallJumpTimer = wallJumpBuffer;
					}
					else
					{
						position.y += min == abs(up) ? -min : min;
						velocityBuffer = velocity;
						velocity.y = 0;
					}
				}
			}
		}
	}

	void Jump()
	{
		velocity.y = -jumpForce;
	}
	void WallJump(int side)
	{
		velocity = { side == 1 ? -maxHorizontalVelocity : maxHorizontalVelocity, -maxHorizontalVelocity  };
		appliedVelocity = 0;
		left = side == 1 ? true : false;
	}

	void EnterGround(Vector2 moveInput)
	{
		velocity.y = 0;
		if (grounded == false)
		{
			//I get knocked down, but I get up again, you will never keep me down
		}
		grounded = true;
	}
	void LeaveGround()
	{
		if (grounded == true)
		{
			//"Liftoff!" said the meat worm.
			if (velocity.x == 0)
			{
				velocity.x += (left ? -appliedVelocity : appliedVelocity);
			}
			else
			{
				float leFaktor = 600 / Clamp(abs(velocity.x), 600, 1200);
				velocity.x += (left ? -appliedVelocity : appliedVelocity) * leFaktor;
			}
			appliedVelocity = 0;
			state = idle;
		}
		grounded = false;
	}

	void GroundCheck(Vector2 moveInput, vector<raylib::Rectangle> blocks)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if (rect.y + rect.height >= blocks[i].y - groundMargin && rect.y < blocks[i].y)
			{
				if (rect.x + rect.width - 15 > blocks[i].x && rect.x + 15 < blocks[i].x + blocks[i].width)
				{
					EnterGround(moveInput);
					return;
				}
			}
		}

		LeaveGround();
	}

	void Timers()
	{
		wallJumpTimer -= GetFrameTime();
		if (wallJumpTimer < 0) { wallJumpTimer = 0; }
		shootTimer -= GetFrameTime();
		if (shootTimer < 0) { shootTimer = 0; }
		runTimer += GetFrameTime();
		if (runTimer >= runInterval) { runTimer = 0; }
	}
};