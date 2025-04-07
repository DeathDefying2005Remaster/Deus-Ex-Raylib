#pragma once
#include <raylib-cpp.hpp>
#include "player.hpp"
#include "global.hpp"

class PlayerMovement
{
	public:

	Player* player;
	raylib::Rectangle rect = raylib::Rectangle(-18, -15, 36, 47);
	float wallJumpTimer = 0;
	Vector2 velocityBuffer = {};
	float appliedVelocity = 0;

	//---- Movement Properties ----//
	float horizontalForce = 9000 * 0.7;
	float maxHorizontalVelocity = 600 * 0.7;
	float normalDrag = 9000 * 0.7;

 	float slideMaxHorizontalVelocity = 1200 * 0.7;
 	float slideDrag = 7000 * 0.7;

 	float airHorizontalForce = 1000;
 	float airMaxHorizontalVelocity = 200;
 	float airDrag = 100;

 	float mainDrag = 5000;
	float gravity = 25;
	float jumpForce = 400;
	float wallJumpBuffer = 1;
	float wallJumpMargin = 30;
	float groundMargin = 1;

	PlayerMovement(Player* _player)
	{
		player = _player;
	}

	raylib::Rectangle PlayerRect()
	{
		return raylib::Rectangle(rect.x + player->position.x, rect.y + player->position.y, rect.width, rect.height);
	}

	void Update()
	{
		GroundCheck();
		LiterallyEverythingElse();
		Collision();
		
		wallJumpTimer -= GetFrameTime();
		if (wallJumpTimer < 0) { wallJumpTimer = 0; }
	}
	
	void Collision()
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if (PlayerRect().x > blocks[i].x - PlayerRect().width && PlayerRect().x < blocks[i].x + blocks[i].width)
			{
				if (PlayerRect().y > blocks[i].y - PlayerRect().height && PlayerRect().y < blocks[i].y + blocks[i].height)
				{
					//check each crossover and pick smallest one
					float left = blocks[i].x - (PlayerRect().x + PlayerRect().width);
					float right = (blocks[i].x + blocks[i].width) - PlayerRect().x;
					float up = blocks[i].y - (PlayerRect().y + PlayerRect().height);
					float down = (blocks[i].y + blocks[i].height) - PlayerRect().y;
					vector<float> arrgh = { abs(left), abs(right), abs(up), abs(down) };
					float min = *min_element(arrgh.begin(), arrgh.end());
					if (min == abs(left) || min == abs(right))
					{
						player->position.x += min == abs(left) ? -min : min;
						if (wallJumpTimer == 0) { velocityBuffer = player->velocity; }
						player->velocity.x = 0;
						wallJumpTimer = wallJumpBuffer;
					}
					else
					{
						velocityBuffer = player->velocity;
						player->velocity.y = 0;
						if (min == abs(up))
						{
							player->position.y -= min;
							EnterGround();
						}
						else
						{
							player->position.y += min;
						}
					}
				}
			}
		}
	}

	void GroundCheck()
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if (PlayerRect().y + PlayerRect().height >= blocks[i].y - groundMargin && PlayerRect().y < blocks[i].y)
			{
				if (PlayerRect().x + PlayerRect().width - 15 > blocks[i].x && PlayerRect().x + 15 < blocks[i].x + blocks[i].width)
				{
					return;
				}
			}
		}

		LeaveGround();
	}

	void EnterGround()
	{
		player->grounded = true;
	}

	void LeaveGround()
	{
		if (player->grounded)
		{
			//"Liftoff!" said the meat worm.
			if (player->velocity.x == 0)
			{
				player->velocity.x += (player->left ? -appliedVelocity : appliedVelocity);
			}
			else
			{
				float leFaktor = 600 / Clamp(abs(player->velocity.x), 600, 1200);
				player->velocity.x += (player->left ? -appliedVelocity : appliedVelocity) * leFaktor;
			}
			appliedVelocity = 0;
			player->state = player->idle;
		}
		player->grounded = false;
	}

	void Jump()
	{
		if (player->grounded)
		{
			player->velocity.y = -jumpForce;
		}
		else
		{
			WallJump();
		}
	}

	int WallCheck()
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if ((PlayerRect().y + PlayerRect().height / 2) > blocks[i].y && (PlayerRect().y + PlayerRect().height / 2) < (blocks[i].y + blocks[i].height))
			{
				if (abs(PlayerRect().x - (blocks[i].x + blocks[i].width)) <= wallJumpMargin)
				{
					//wall on the left
					return -1;
				}
				else if (abs((PlayerRect().x + PlayerRect().width) - blocks[i].x) <= wallJumpMargin)
				{
					//wall on the right
					return 1;
				}
			}
		}
		return 0;
	}

	void WallJump()
	{
		int side = WallCheck();
		if (side != 0)
		{
			player->velocity = { side == 1 ? -maxHorizontalVelocity : maxHorizontalVelocity, -maxHorizontalVelocity };
			appliedVelocity = 0;
			player->left = side == 1 ? true : false;
		}
	}

	void LiterallyEverythingElse()
	{
		if (!player->grounded)
		{
			player->velocity.y += gravity;
		}

		float force;
		float max;
		float drag;

		if (player->grounded)
		{
			if (player->state == player->sliding)
			{
				force = horizontalForce;
				max = slideMaxHorizontalVelocity;
				drag = slideDrag;
			}
			else
			{
				force = horizontalForce;
				max = maxHorizontalVelocity;
				drag = normalDrag;
			}
		}
		else
		{
			force = airHorizontalForce;
			max = airMaxHorizontalVelocity;
			drag = airDrag;
		}

		if (player->state == player->sliding && player->grounded)
		{
			appliedVelocity = slideMaxHorizontalVelocity;
			player->velocity.x = player->left ? -abs(player->velocity.x) : abs(player->velocity.x);
		}
		else
		{
			if (player->moveInput != 0)
			{
				appliedVelocity += force * GetFrameTime();
				appliedVelocity = Clamp(appliedVelocity, 0, max);
			}
			else if (appliedVelocity != 0 && player->grounded)
			{
				appliedVelocity *= Clamp(1 - (GetFrameTime() * drag / abs(appliedVelocity)), 0, 1);
			}
		}

		if (player->velocity.x != 0 && player->grounded)
		{
			if (player->moveInput == 0 || player->state == player->sliding)
			{
				player->velocity.x *= Clamp(1 - (GetFrameTime() * mainDrag / abs(player->velocity.x)), 0, 1);
			}
			else
			{
				player->velocity.x = 0;
			}
		}

		float scaledAppliedVelocity = appliedVelocity;
		if (player->velocity.x < 0 && player->left || player->velocity.x > 0 && !player->left)
		{
			scaledAppliedVelocity = Clamp(appliedVelocity, 0, Clamp(max - abs(player->velocity.x), 0, max));
		}
		Vector2 scaledForce = Vector2Scale({ player->velocity.x + ((player->left ? -1 : 1) * scaledAppliedVelocity), player->velocity.y }, GetFrameTime());
		Vector2 pos = player->position;
		player->position = Vector2Add(pos, scaledForce);

		DrawText(to_string(appliedVelocity).c_str(), 20, 20, 20, BLACK);
		DrawText(to_string(player->velocity.x).c_str(), 200, 20, 20, BLACK);
		DrawText(to_string(scaledForce.x / GetFrameTime()).c_str(), 600, 20, 20, BLACK);
		DrawText(player->grounded ? "GROUNDED" : "FREE", 400, 20, 20, BLACK);
	}
};