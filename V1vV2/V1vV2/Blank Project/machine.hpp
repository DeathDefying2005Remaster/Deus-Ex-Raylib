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
		if (left)
		{
			if (shootTimer == 0)
			{
				switch (state)
				{
				case idle:
					sourceRec = { 0, 0, (float)frameWidth, (float)frameHeight };
					break;
				case running:
					runTimer += GetFrameTime();
					if (runTimer > runInterval) { runTimer = 0; }
					if (runTimer < runInterval / 2)
					{
						sourceRec = { (float)frameWidth * 8, 0, (float)frameWidth, (float)frameHeight };
					}
					else
					{
						sourceRec = { (float)frameWidth * 10, 0, (float)frameWidth, (float)frameHeight };
					}
					break;
				case sliding:
					sourceRec = { (float)frameWidth * 12, 0, (float)frameWidth, (float)frameHeight };
					break;
				}
			}
			else
			{
				if (state == idle || state == running)
				{
					if (Vector2Equals(shootDir, { 0, 1 }))
					{
						sourceRec = { (float)frameWidth * 16, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 1 }))
					{
						sourceRec = { (float)frameWidth * 14, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 1 }))
					{
						sourceRec = { (float)frameWidth * 15, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 0 }))
					{
						sourceRec = { (float)frameWidth * 2, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 0 }))
					{
						sourceRec = { (float)frameWidth * 27, 0, (float)frameWidth, (float)frameHeight };
					}
				}
				else if (state == sliding && left)
				{
					if (Vector2Equals(shootDir, { 0, 1 }))
					{
						sourceRec = { (float)frameWidth * 17, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 1 }))
					{
						sourceRec = { (float)frameWidth * 18, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 1 }))
					{
						sourceRec = { (float)frameWidth * 19, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 0 }))
					{
						sourceRec = { (float)frameWidth * 21, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 0 }))
					{
						sourceRec = { (float)frameWidth * 20, 0, (float)frameWidth, (float)frameHeight };
					}
				}
				else if (state == sliding && !left)
				{
					if (Vector2Equals(shootDir, { 0, 1 }))
					{
						sourceRec = { (float)frameWidth * 24, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 1 }))
					{
						sourceRec = { (float)frameWidth * 23, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 1 }))
					{
						sourceRec = { (float)frameWidth * 25, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 0 }))
					{
						sourceRec = { (float)frameWidth * 22, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 0 }))
					{
						sourceRec = { (float)frameWidth * 26, 0, (float)frameWidth, (float)frameHeight };
					}
				}
			}
		}
		else
		{
			if (shootTimer == 0)
			{
				switch (state)
				{
				case idle:
					sourceRec = { (float)frameWidth * 6, 0, (float)frameWidth, (float)frameHeight };
					break;
				case running:
					runTimer += GetFrameTime();
					if (runTimer > runInterval) { runTimer = 0; }
					if (runTimer < runInterval / 2)
					{
						sourceRec = { (float)frameWidth * 9, 0, (float)frameWidth, (float)frameHeight };
					}
					else
					{
						sourceRec = { (float)frameWidth * 11, 0, (float)frameWidth, (float)frameHeight };
					}
					break;
				case sliding:
					sourceRec = { (float)frameWidth * 13, 0, (float)frameWidth, (float)frameHeight };
					break;
				}
			}
			else
			{
				if (state == idle || state == running)
				{
					if (Vector2Equals(shootDir, { 0, 1 }))
					{
						sourceRec = { (float)frameWidth * 16, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 1 }))
					{
						sourceRec = { (float)frameWidth * 14, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 1 }))
					{
						sourceRec = { (float)frameWidth * 15, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 0 }))
					{
						sourceRec = { (float)frameWidth * 2, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 0 }))
					{
						sourceRec = { (float)frameWidth * 27, 0, (float)frameWidth, (float)frameHeight };
					}
				}
				else if (state == sliding && left)
				{
					if (Vector2Equals(shootDir, { 0, 1 }))
					{
						sourceRec = { (float)frameWidth * 17, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 1 }))
					{
						sourceRec = { (float)frameWidth * 18, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 1 }))
					{
						sourceRec = { (float)frameWidth * 19, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 0 }))
					{
						sourceRec = { (float)frameWidth * 21, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 0 }))
					{
						sourceRec = { (float)frameWidth * 20, 0, (float)frameWidth, (float)frameHeight };
					}
				}
				else if (state == sliding && !left)
				{
					if (Vector2Equals(shootDir, { 0, 1 }))
					{
						sourceRec = { (float)frameWidth * 24, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 1 }))
					{
						sourceRec = { (float)frameWidth * 23, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 1 }))
					{
						sourceRec = { (float)frameWidth * 25, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { -1, 0 }))
					{
						sourceRec = { (float)frameWidth * 22, 0, (float)frameWidth, (float)frameHeight };
					}
					else if (Vector2Equals(shootDir, { 1, 0 }))
					{
						sourceRec = { (float)frameWidth * 26, 0, (float)frameWidth, (float)frameHeight };
					}
				}
			}
		}

		Rectangle destRec = { position.x - frameWidth*scaleFactor*0.5, position.y - frameHeight*scaleFactor*0.5, scaleFactor * frameWidth, scaleFactor * frameHeight };
		Vector2 origin = { 0, 0 };
		int rotation = 0;

		Texture2D sheet = LoadTextureFromImage(sheetImage);
		DrawTexturePro(sheet, sourceRec, destRec, origin, (float)rotation, WHITE);
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

	void Timers()
	{
		wallJumpTimer -= GetFrameTime();
		if (wallJumpTimer < 0) { wallJumpTimer = 0; }
		shootTimer -= GetFrameTime();
		if (shootTimer < 0) { shootTimer = 0; }
	}
};