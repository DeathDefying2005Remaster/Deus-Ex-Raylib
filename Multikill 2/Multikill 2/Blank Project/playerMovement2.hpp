//#pragma once
//#include <raylib-cpp.hpp>
//#include "keybinds.hpp"
//#include "player.hpp"
//
//class PlayerMovement
//{
//	public:
//
//	enum PlayerState
//	{
//		idle,
//		running,
//		sliding,
//	};
//
//	Player* player;
//	Vector2* position;
//	Vector2* velocity;
//	raylib::Rectangle* rect;
//	Vector2* shootDir;
//
//	PlayerMovement(Vector2* _position, Vector2* _velocity, raylib::Rectangle* _rect, Vector2* _shootDir)
//	{
//		player = nullptr;
//		position = _position;
//		velocity = _velocity;
//		rect = _rect;
//		shootDir = _shootDir;
//	}
//
//	Vector2 velocityBuffer;
//	float appliedVelocity;
//	bool grounded;
//	bool left;
//	float wallJumpTimer;
//	float runTimer;
//	float runInterval;
//	PlayerState state;
//
//	//---- Movement Properties ----//
//	float gravity = 25;
//	float horizontalForce = 9000 * 0.7;
//	float maxHorizontalVelocity = 600 * 0.7;
//	float normalDrag = 9000 * 0.7;
//	float slideMaxHorizontalVelocity = 1200 * 0.7;
//	float slideDrag = 7000 * 0.7;
//	float airHorizontalForce = 1000;
//	float airMaxHorizontalVelocity = 200;
//	float airDrag = 100;
//	float jumpForce = 500 - 100;
//	float mainDrag = 5000;
//	float wallJumpBuffer = 1;
//	float wallJumpMargin = 30;
//	float groundMargin = 1;
//
//	void Timers()
//	{
//		wallJumpTimer -= GetFrameTime();
//		if (wallJumpTimer < 0) { wallJumpTimer = 0; }
//		runTimer += GetFrameTime();
//		if (runTimer >= runInterval) { runTimer = 0; }
//	}
//
//	Vector2 MoveInput()
//	{
//		Vector2 moveInput = {};
//		if (IsKeyDown(MLEFT)) { moveInput.x -= 1; }
//		if (IsKeyDown(MRIGHT)) { moveInput.x += 1; }
//		return moveInput;
//	}
//
//	void Movement()
//	{
//		//---- Input ----//
//
//		if (MoveInput().x < 0) { left = true; }
//		else if (MoveInput().x > 0) { left = false; }
//
//		if (IsKeyPressed(SLIDE)) { state = sliding; }
//		if (IsKeyReleased(SLIDE)) { state = idle; }
//
//		else if (MoveInput().x != 0 && state != sliding) { state = running; }
//		else if (state != sliding) { state = idle; }
//
//		if (IsKeyPressed(KEY_SPACE))
//		{
//			if (grounded)
//			{
//				Jump();
//			}
//			else if (WallCheck(blocks) != 0)
//			{
//				WallJump(WallCheck(blocks));
//			}
//		}
//
//		//---- Literally Everything Else ----//
//
//		if (!grounded)
//		{
//			velocity->y += gravity;
//		}
//
//		float force;
//		float max;
//		float drag;
//
//		if (grounded)
//		{
//			if (state == sliding)
//			{
//				force = horizontalForce;
//				max = slideMaxHorizontalVelocity;
//				drag = slideDrag;
//			}
//			else
//			{
//				force = horizontalForce;
//				max = maxHorizontalVelocity;
//				drag = normalDrag;
//			}
//		}
//		else
//		{
//			force = airHorizontalForce;
//			max = airMaxHorizontalVelocity;
//			drag = airDrag;
//		}
//
//		if (state == sliding && grounded)
//		{
//			appliedVelocity = slideMaxHorizontalVelocity;
//			velocity->x = left ? -abs(velocity->x) : abs(velocity->x);
//		}
//		else
//		{
//			if (MoveInput().x != 0)
//			{
//				appliedVelocity += force * GetFrameTime();
//				appliedVelocity = Clamp(appliedVelocity, 0, max);
//			}
//			else if (appliedVelocity != 0 && grounded)
//			{
//				appliedVelocity *= Clamp(1 - (GetFrameTime() * drag / abs(appliedVelocity)), 0, 1);
//			}
//		}
//
//		if (velocity->x != 0 && grounded)
//		{
//			if (MoveInput().x == 0 || state == sliding)
//			{
//				velocity->x *= Clamp(1 - (GetFrameTime() * mainDrag / abs(velocity->x)), 0, 1);
//			}
//			else
//			{
//				velocity->x = 0;
//			}
//		}
//
//		float scaledAppliedVelocity = appliedVelocity;
//		if (velocity->x < 0 && left || velocity->x > 0 && !left)
//		{
//			scaledAppliedVelocity = Clamp(appliedVelocity, 0, Clamp(max - abs(velocity->x), 0, max));
//		}
//		Vector2 scaledForce = Vector2Scale({ velocity->x + ((left ? -1 : 1) * scaledAppliedVelocity), velocity->y }, GetFrameTime());
//		Vector2 pos = *position;
//		*position = Vector2Add(pos, scaledForce);
//
//		DrawText(to_string(appliedVelocity).c_str(), 20, 20, 20, BLACK);
//		DrawText(to_string(velocity->x).c_str(), 200, 20, 20, BLACK);
//		DrawText(to_string(scaledForce.x / GetFrameTime()).c_str(), 600, 20, 20, BLACK);
//	}
//
//	void Jump()
//	{
//		velocity->y = -jumpForce;
//	}
//
//	void WallJump(int side)
//	{
//		*velocity = { side == 1 ? -maxHorizontalVelocity : maxHorizontalVelocity, -maxHorizontalVelocity };
//		appliedVelocity = 0;
//		left = side == 1 ? true : false;
//	}
//
//	int WallCheck(vector<raylib::Rectangle> blocks)
//	{
//		for (int i = 0; i < blocks.size(); i++)
//		{
//			if ((rect->y + rect->height / 2) > blocks[i].y && (rect->y + rect->height / 2) < (blocks[i].y + blocks[i].height))
//			{
//				if (abs(rect->x - (blocks[i].x + blocks[i].width)) <= wallJumpMargin)
//				{
//					//wall on the left
//					return -1;
//				}
//				else if (abs((rect->x + rect->width) - blocks[i].x) <= wallJumpMargin)
//				{
//					//wall on the right
//					return 1;
//				}
//			}
//		}
//		return 0;
//	}
//
//	void Collision(vector<raylib::Rectangle> blocks)
//	{
//		for (int i = 0; i < blocks.size(); i++)
//		{
//			if (rect->x > blocks[i].x - rect->width && rect->x < blocks[i].x + blocks[i].width)
//			{
//				if (rect->y > blocks[i].y - rect->height && rect->y < blocks[i].y + blocks[i].height)
//				{
//					//check each crossover and pick smallest one
//					float left = blocks[i].x - (rect->x + rect->width);
//					float right = (blocks[i].x + blocks[i].width) - rect->x;
//					float up = blocks[i].y - (rect->y + rect->height);
//					float down = (blocks[i].y + blocks[i].height) - rect->y;
//					vector<float> arrgh = { abs(left), abs(right), abs(up), abs(down) };
//					float min = *min_element(arrgh.begin(), arrgh.end());
//					if (min == abs(left) || min == abs(right))
//					{
//						position->x += min == abs(left) ? -min : min;
//						if (wallJumpTimer == 0) { velocityBuffer = *velocity; }
//						velocity->x = 0;
//						wallJumpTimer = wallJumpBuffer;
//					}
//					else
//					{
//						position->y += min == abs(up) ? -min : min;
//						velocityBuffer = *velocity;
//						velocity->y = 0;
//					}
//				}
//			}
//		}
//	}
//
//	void EnterGround(Vector2 moveInput)
//	{
//		velocity->y = 0;
//		if (grounded == false)
//		{
//			//I get knocked down, but I get up again, you will never keep me down
//		}
//		grounded = true;
//	}
//	void LeaveGround()
//	{
//		if (grounded == true)
//		{
//			//"Liftoff!" said the meat worm.
//			if (velocity->x == 0)
//			{
//				velocity->x += (left ? -appliedVelocity : appliedVelocity);
//			}
//			else
//			{
//				float leFaktor = 600 / Clamp(abs(velocity->x), 600, 1200);
//				velocity->x += (left ? -appliedVelocity : appliedVelocity) * leFaktor;
//			}
//			appliedVelocity = 0;
//			state = idle;
//		}
//		grounded = false;
//	}
//
//	void GroundCheck(Vector2 moveInput, vector<raylib::Rectangle> blocks)
//	{
//		for (int i = 0; i < blocks.size(); i++)
//		{
//			if (rect->y + rect->height >= blocks[i].y - groundMargin && rect->y < blocks[i].y)
//			{
//				if (rect->x + rect->width - 15 > blocks[i].x && rect->x + 15 < blocks[i].x + blocks[i].width)
//				{
//					EnterGround(moveInput);
//					return;
//				}
//			}
//		}
//
//		LeaveGround();
//	}
//
//	vector<int> GetCoords()
//	{
//		int row = 0;
//		int column = 0;
//		vector<Vector2>  dirToIndex = { { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };
//
//		if (!Vector2Equals(*shootDir, {}))
//		{
//			if (state == sliding && left) { row = 0; }
//			else if (state == sliding && !left) { row = 1; }
//			else if (state != sliding) { row = 2; }
//
//			for (int i = 0; i < dirToIndex.size(); i++)
//			{
//				if (Vector2Equals(*shootDir, dirToIndex[i]))
//				{
//					column = i;
//					break;
//				}
//			}
//		}
//		else
//		{
//			if (state == sliding)
//			{
//				row = left ? 0 : 1;
//				column = 0;
//			}
//			else if (state == running)
//			{
//				row = 4;
//				if (left && runTimer < runInterval / 2) { column = 0; }
//				else if (left && runTimer >= runInterval / 2) { column = 1; }
//				else if (!left && runTimer < runInterval / 2) { column = 2; }
//				else if (!left && runTimer >= runInterval / 2) { column = 3; }
//			}
//			else if (state != running)
//			{
//				row = 3;
//				if (runTimer < runInterval / 2) { column = left ? 0 : 2; }
//				else if (runTimer >= runInterval / 2) { column = left ? 1 : 3; }
//				else if (!left) { column = 2; }
//			}
//		}
//
//		return { column, row };
//	}
//};