#include <raylib-cpp.hpp>
#include "variables.hpp"
#include "triangle.hpp"
#include "tools.hpp"
#include "classes.hpp"
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
using namespace std;

Player player = Player();
vector<ParticleEffect> particleEffects = {};
vector<Enemy> enemies = {};

double lastScreenShrink = 0;

void PlayerDash(Player &p)
{
	juice -= juiceCostDash;
	Clamp(juice, 0, 1);

	Vector2 newPos = p.PlayerPrediction();
	int count = end(p.ghosts) - begin(p.ghosts);
	for (int i = 0; i < (count); i++)
	{
		Vector2 pos = { Lerp(p.pos.x, newPos.x, (float)i / (float)count), Lerp(p.pos.y, newPos.y, (float)i / (float)count) };
		if (pos.x < 0 || pos.x > GetScreenWidth())
		{
			pos.x += (pos.x < 0) ? GetScreenWidth() : -GetScreenWidth();
		}
		if (pos.y < 0 || pos.y > GetScreenHeight())
		{
			pos.y += (pos.y < 0) ? GetScreenHeight() : -GetScreenHeight();
		}

		float h = -p.size * sin(p.widthAngle * (PI / 180));
		float v = p.size * cos(p.widthAngle * (PI / 180));
		float h2 = p.size * sin(p.widthAngle * (PI / 180));
		float v2 = p.size * cos(p.widthAngle * (PI / 180));

		p.ghosts[i].pointTop = { (0 * cos(p.rotation)) - (-p.size * sin(p.rotation)) + pos.x, (-p.size * cos(p.rotation)) + (0 * sin(p.rotation)) + pos.y };
		p.ghosts[i].pointLeft = { (h * cos(p.rotation)) - (v * sin(p.rotation)) + pos.x, (v * cos(p.rotation)) + (h * sin(p.rotation)) + pos.y };
		p.ghosts[i].pointRight = { (h2 * cos(p.rotation)) - (v2 * sin(p.rotation)) + pos.x, (v2 * cos(p.rotation)) + (h2 * sin(p.rotation)) + pos.y };
	}

	p.triangle.Move(Vector2Subtract(newPos, p.pos));
	p.pos = newPos;

	for (int j = 0; j < end(enemies) - begin(enemies); j++)
	{
		vector<int> hu;
		end(hu);
		if (CheckTriangleInRectangle(p.ghosts[end(p.ghosts) - begin(p.ghosts) - 1], enemies[j].GetCollider()) && enemies[j].alive)
		{
			p.KillPlayer();
		}
	}

	for (int i = 0; i < end(p.ghosts) - begin(p.ghosts); i++)
	{
		for (int j = 0; j < end(enemies) - begin(enemies); j++)
		{
			if (CheckTriangleInRectangle(p.ghosts[i], enemies[j].GetCollider()) && enemies[j].alive)
			{
				enemies[j].Destroy(particleEffects);
			}
		}
	}

	p.ghostFade = 1;
}

void ShrinkWindow()
{
	if (GetTime() - lastScreenShrink > screenShrinkInterval)
	{
		lastScreenShrink = GetTime();

		SetWindowSize(GetScreenWidth() - screenShrinkAmount, GetScreenWidth() - screenShrinkAmount);
		SetWindowPosition(GetWindowPosition().x + (screenShrinkAmount/2), GetWindowPosition().y + (screenShrinkAmount/2));

		player.pos.x -= (int)(screenShrinkAmount/2);
		player.pos.y -= (int)(screenShrinkAmount/2);

		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i].pos.x -= (screenShrinkAmount/2);
			enemies[i].pos.y -= (screenShrinkAmount/2);
		}
	}
}

void Start()
{
	float distance = screenWidth - GetScreenWidth();
	SetWindowSize(screenWidth, screenHeight);
	SetWindowPosition(GetWindowPosition().x - (distance/2), GetWindowPosition().y - (distance/2));

	//drawingFake = false;
	gameover = false;
	timeflow = true;
	globalTimeScale = 1;
	player = Player({400, 400}, startSize, maxWidthAngle, 0, minRotSpeed);
	//playerTardis = Tardis();
	holdingDash = false;

	camera.target = { 0, 0 };
	camera.offset = Vector2();
	camera.rotation = 0;
	camera.zoom = 1;

	enemies = {
	Enemy(40, { 300, 600 }),
	Enemy(40, { 600, 600 })
	};
	particleEffects = {};

	juice = 1;
}

int main()
{	
	//---- Start ----//
	Start();

	InitWindow(screenWidth, screenHeight, "WAKE UP");
	SetTargetFPS(targetFps);

	while (!WindowShouldClose())
	{
		if (!gameover)
		{
			//---- Shrink Screen ----//
			ShrinkWindow();

			//---- Input ----//
			if (IsKeyPressed(restart))
			{
				Start();
			}

			if ((IsKeyDown(left1) || IsKeyDown(left2)) && !(IsKeyDown(right1) || IsKeyDown(right2)))
			{
				player.rotation -= player.rotationSpeed * GetFrameTime() * globalTimeScale;
			}
			if ((IsKeyDown(right1) || IsKeyDown(right2)) && !(IsKeyDown(left1) || IsKeyDown(left2)))
			{
				player.rotation += player.rotationSpeed * GetFrameTime() * globalTimeScale;
			}
			if (IsKeyDown(forward1) || IsKeyDown(forward2))
			{
				player.velocity.x -= speed * sin(-player.rotation) * GetFrameTime() * globalTimeScale;
				player.velocity.y -= speed * cos(-player.rotation) * GetFrameTime() * globalTimeScale;
			}

			if (IsKeyPressed(dash) || IsMouseButtonPressed(dashMouse) && juice > juiceRequiredDash)
			{
				holdingDash = true;
				globalTimeScale = 0.2;
			}
			else if (holdingDash && juice < juiceRequiredDash)
			{
				holdingDash = false;
				globalTimeScale = 1;
			}
			else if ((IsKeyReleased(dash) || IsMouseButtonReleased(dashMouse)) && holdingDash)
			{
				holdingDash = false;
				globalTimeScale = 1;
				if (sqrt(pow(player.velocity.x, 2) + pow(player.velocity.y, 2)) >= minDashVelocity && juice >= juiceRequiredDash)
				{
					PlayerDash(player);
					screenShake = screenShakeTime;
				}
			}

			//---- Dashing ----//
			juice += juiceRecharge * GetFrameTime();
			if (juice > 1) { juice = 1; }

			if (holdingDash && sqrt(pow(player.velocity.x, 2) + pow(player.velocity.y, 2)) >= minDashVelocity)
			{
				Vector2 newPos = player.PlayerPrediction();
				player.DrawPlayer(newPos, SKYBLUE, false);
				juice -= juicePlanDecay * GetFrameTime();
			}

			//---- Movement ----//
			player.velocity = DownClampVector2(player.velocity, maxVelocity);

			player.pos.x += player.velocity.x * GetFrameTime() * globalTimeScale;
			player.pos.y += player.velocity.y * GetFrameTime() * globalTimeScale;
			if (player.pos.x < 0)
			{
				player.pos.x = GetScreenWidth() + player.pos.x;
			}
			else if (player.pos.x > GetScreenWidth())
			{
				player.pos.x = player.pos.x - GetScreenWidth();
			}
			if (player.pos.y < 0)
			{
				player.pos.y = GetScreenHeight() + player.pos.y;
			}
			else if (player.pos.y > GetScreenHeight())
			{
				player.pos.y = player.pos.y - GetScreenHeight();
			}

			player.velocity.x *= (1 - (drag * globalTimeScale));
			player.velocity.y *= (1 - (drag * globalTimeScale));

			player.rotationSpeed = Lerp(minRotSpeed, maxRotSpeed, sqrt(pow(player.velocity.x, 2) + pow(player.velocity.y, 2)) / maxVelocity);
			player.widthAngle = Lerp(maxWidthAngle, minWidthAngle, sqrt(pow(player.velocity.x, 2) + pow(player.velocity.y, 2)) / maxVelocity);

			//---- Enemy Behaviour ----//
			for (int i = 0; i < end(enemies) - begin(enemies); i++)
			{
				enemies[i].MoveEnemy(enemies, player.pos);
			}

			//---- Collision ----//
			for (int i = 0; i < end(enemies) - begin(enemies); i++)
			{
				if (enemies[i].alive)
				{
					Rectangle circleBox = enemies[i].GetCollider();

					if (CheckTriangleInRectangle(player.triangle, circleBox))
					{
						player.KillPlayer();
					}
				}
			}
		}
		else
		{
			if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_R))
			{
				Start();
			}
		}

		//--- Screen Shake----//
		if (screenShake > 0)
		{
			float x = GetRandomValue(-screenShakeMagnitude, screenShakeMagnitude);
			float y = GetRandomValue(-screenShakeMagnitude, screenShakeMagnitude);
			camera.offset = { x * screenShake, y * screenShake };
			screenShake -= GetFrameTime();
			if (screenShake < 0) { screenShake = 0; }
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(RAYWHITE);
		BeginMode2D(camera);

		player.DrawDash();
		if (!gameover) { player.DrawPlayer(player.pos, BLUE, true); }
		for (int i = 0; i < end(enemies) - begin(enemies); i++)
		{
			enemies[i].DrawEnemy();
			enemies[i].bladeRotation += enemies[i].bladeSpeed * GetFrameTime();
		}

		if (gameover) { player.DrawDeadPlayer(); DrawText("PRESS SPACEBAR TO RESTART", 100, 190, 25, DARKGRAY); }

		if (!gameover)
		{
			for (int i = 0; i < end(particleEffects) - begin(particleEffects); i++)
			{
				particleEffects[i].Update(globalTimeScale);
				particleEffects[i].DrawParticleEffect();
			}

			int turnstileW = 200;
			int turnstileH = 60;
			DrawRectangle((GetScreenWidth()/2 - turnstileW)/2, GetScreenHeight() - turnstileH, turnstileW, turnstileH, BLACK);

			string strr = to_string(player.ghostFade);
			DrawText(strr.c_str(), 100, 70, 20, DARKGRAY);
			string str = to_string(player.rotation);
			DrawText(str.c_str(), 100, 100, 20, DARKGRAY);
			string str2 = to_string(player.velocity.x);
			string str3 = to_string(player.velocity.y);
			DrawText(str2.c_str(), 100, 130, 20, DARKGRAY);
			DrawText(str3.c_str(), 100, 160, 20, DARKGRAY);
			if (sqrt(pow(player.velocity.x, 2) + pow(player.velocity.y, 2)) < minDashVelocity)
			{
				DrawText("DASH CORE: INSUFFICIENT SPEED", 100, 190, 25, DARKBLUE);
			}
			else if (juice < juiceRequiredDash)
			{
				DrawText("DASH CORE: INSUFFICIENT JUICE", 100, 190, 25, DARKBLUE);
			}
			else
			{
				DrawText("DASH CORE: ACTIVE", 100, 190, 25, DARKBLUE);
			}
			DrawText("JUICE:", 100, 220, 25, DARKBLUE);

			DrawRectangle(190, 220, 150, 25, DARKGRAY);
			DrawRectangle(190, 220, juice * 150, 25, DARKBLUE);
		}

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}