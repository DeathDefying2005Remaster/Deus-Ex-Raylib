#pragma once
#include <raylib-cpp.hpp>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "triangle.hpp"
#include "variables.hpp"
#include "tools.hpp"
using namespace std;

class ParticleEffect
{
	public:
	float life;
	float lifeTime;
	float speed;
	float size;
	float particleCount;
	Vector2 origin;
	Color color;

	vector<Vector2> particlePositions;
	vector<Vector2> particleDirections;

	ParticleEffect(float _lifeTime, float _speed, float _size, float _particleCount, Vector2 _origin, Color _color)
	{
		lifeTime = _lifeTime;
		life = lifeTime;
		speed = _speed;
		size = _size;
		particleCount = _particleCount;
		origin = _origin;
		color = _color;

		particlePositions = {};
		particleDirections = {};
		for (int i = 0; i < particleCount; i++)
		{
			float randX = (float)rand() * (GetRandomValue(0, 1) == 0 ? -1 : 1);
			float randY = (float)rand() * (GetRandomValue(0, 1) == 0 ? -1 : 1);

			particlePositions.push_back(origin);
			particleDirections.push_back(Vector2Normalize({ randX, randY }));
		}
	}

	void Update(float globalTimeScale)
	{
		if (life > 0)
		{
			life -= GetFrameTime() * globalTimeScale;

			for (int i = 0; i < end(particlePositions) - begin(particlePositions); i++)
			{
				Vector2 scaledDir = Vector2Scale(particleDirections[i], speed * GetFrameTime() * globalTimeScale);
				particlePositions[i] = Vector2Add(particlePositions[i], scaledDir);
			}
		}
	}

	void DrawParticleEffect()
	{
		for (int i = 0; i < end(particlePositions) - begin(particlePositions); i++)
		{
			Color newColor = ColorAlpha(color, life / lifeTime);
			DrawCircle(particlePositions[i].x, particlePositions[i].y, size * (life / lifeTime), newColor);
		}
	}
};

class Enemy
{
	public:
	bool alive;
	float size;
	Vector2 pos;
	Vector2 velocity;
	float maxVelocity;
	float speed;
	float antiMergeForce;
	float drag;
	int bladeCount;
	float bladeRotation;
	float bladeSpeed;

	Enemy(float s, Vector2 p)
	{
		alive = true;
		size = s;
		pos = p;
		velocity = Vector2();
		maxVelocity = 700;
		speed = 1200 * 0;
		antiMergeForce = 14;
		drag = 0.05;
		bladeCount = 6;
		bladeRotation = 0;
		bladeSpeed = 1;
	}

	Rectangle GetCollider()
	{
		Vector2 position = { pos.x - size, pos.y - size };
		Vector2 recSize = { size * 2, size * 2 };
		return raylib::Rectangle(position, recSize);
	}

	void Destroy(vector<ParticleEffect> &particleEffectList)
	{
		alive = false;
		ParticleEffect effect = ParticleEffect(0.6, 150, 15, 10, pos, RED);
		particleEffectList.push_back(effect);
	}

	void DrawEnemy()
	{
		if (alive)
		{
			DrawCircle(pos.x, pos.y, size, RED);

			float tempRotation = bladeRotation / (PI / 180);
			for (int i = 0; i < bladeCount; i++)
			{
				Vector2 top;
				top.x = (0 * cos(tempRotation)) - ((-size - 20) * sin(tempRotation)) + pos.x;
				top.y = ((-size - 20) * cos(tempRotation)) + (0 * sin(tempRotation)) + pos.y;
				Vector2 left;
				left.x = (-10 * cos(tempRotation)) - ((-size - 5) * sin(tempRotation)) + pos.x;
				left.y = ((-size - 5) * cos(tempRotation)) + (-10 * sin(tempRotation)) + pos.y;
				Vector2 right;
				right.x = (10 * cos(tempRotation)) - ((-size - 5) * sin(tempRotation)) + pos.x;
				right.y = ((-size - 5) * cos(tempRotation)) + (10 * sin(tempRotation)) + pos.y;

				DrawTriangle(top, left, right, RED);

				tempRotation += (2 * PI) / bladeCount;
			}
		}
	}

	void MoveEnemy(vector<Enemy> enemyList, Vector2 target)
	{
		Vector2 direction = Vector2Normalize({ target.x - pos.x, target.y - pos.y });
		float factor = speed * GetFrameTime() * globalTimeScale;
		Vector2 change = Vector2Scale(direction, factor);
		velocity = Vector2Add(velocity, change);

		for (int i = 0; i < end(enemyList) - begin(enemyList); i++)
		{
			if (enemyList[i].pos.x != pos.x || enemyList[i].pos.y != pos.y)
			{
				Vector2 antiMergeDir = Vector2Normalize(Vector2Subtract(pos, enemyList[i].pos));
				float force = (speed * antiMergeForce * GetFrameTime() * globalTimeScale) / Vector2Distance(pos, enemyList[i].pos);
				Vector2 antiMerge = Vector2Scale(Vector2Normalize(Vector2Subtract(pos, enemyList[i].pos)), force);
				velocity = Vector2Add(velocity, antiMerge);
			}
		}

		velocity = DownClampVector2(velocity, maxVelocity);

		pos.x += velocity.x * GetFrameTime() * globalTimeScale;
		pos.y += velocity.y * GetFrameTime() * globalTimeScale;
		velocity.x *= (1 - (drag * globalTimeScale));

		if (pos.x < size) { pos.x = size; }
		else if (pos.x > GetScreenWidth() - size) { pos.x = GetScreenWidth() - size; }
		if (pos.y < size) { pos.y = size; }
		else if (pos.y > GetScreenHeight() - size) { pos.y = GetScreenHeight() - size; }
	}
};

class Player
{
	public:
	Vector2 pos;
	Vector2 velocity;
	float size;
	float widthAngle;
	float rotation;
	float rotationSpeed;
	float ghostFade;
	Triangle triangle;
	vector<Triangle> pieces;
	vector<Triangle> ghosts;

	Player(Vector2 _pos, float _size, float _widthAngle, float _rotation, float _rotationSpeed)
	{
		pos = _pos;
		velocity = Vector2();
		size = _size;
		widthAngle = _widthAngle;
		rotation = _rotation;
		rotationSpeed = _rotationSpeed;
		ghostFade = 0;
		pieces = {};
		for (int i = 0; i < 10; i++) { ghosts.push_back(Triangle()); }
	}
	Player()
	{
		pos = Vector2();
		velocity = Vector2();
		size = 0;
		widthAngle = 0;
		rotation = 0;
		rotationSpeed = 0;
		ghostFade = 0;
		pieces = {};
		for (int i = 0; i < 10; i++) { ghosts.push_back(Triangle()); }
	}

	Vector2 PlayerPrediction()
	{
		return { pos.x + (velocity.x * dashTime), pos.y + (velocity.y * dashTime) };
	}

	void KillPlayer()
	{
		globalTimeScale = 0;
		gameover = true;

		pieces = BreakTriangle(triangle, 3);
	}

	void DrawDeadPlayer()
	{
		for (int i = 0; i < end(pieces) - begin(pieces); i++)
		{
			DrawTriangle(pieces[i].pointTop, pieces[i].pointLeft, pieces[i].pointRight, BLUE);

			Vector2 push = Vector2Scale(Vector2Normalize(Vector2Subtract(pieces[i].FindCentroid(), pos)), pieceMoveSpeed);
			pieces[i].Move(push);
			pieces[i].Rotate(pieceSpinSpeed);
		}
	}

	void DrawDash()
	{
		Color color = BLUE;
		int count = end(ghosts) - begin(ghosts);
		for (int i = 0; i < count; i++)
		{
			DrawTriangle(ghosts[i].pointTop, ghosts[i].pointLeft, ghosts[i].pointRight, ColorAlpha(color, ghostFade * Lerp(0.1, 1, (float)i / (float)count)));
		}

		ghostFade -= ghostFadeSpeed * GetFrameTime();
		if (ghostFade < 0) { ghostFade = 0; }
	}

	void DrawPlayer(Vector2 pos, Color color, bool player)
	{
		Vector2 top;
		top.x = (0 * cos(rotation)) - (-size * sin(rotation)) + pos.x;
		top.y = (-size * cos(rotation)) + (0 * sin(rotation)) + pos.y;

		Vector2 left;
		float h = -size * sin(widthAngle * (PI / 180));
		float v = size * cos(widthAngle * (PI / 180));
		left.x = (h * cos(rotation)) - (v * sin(rotation)) + pos.x;
		left.y = (v * cos(rotation)) + (h * sin(rotation)) + pos.y;

		Vector2 right;
		float h2 = size * sin(widthAngle * (PI / 180));
		float v2 = size * cos(widthAngle * (PI / 180));
		right.x = (h2 * cos(rotation)) - (v2 * sin(rotation)) + pos.x;
		right.y = (v2 * cos(rotation)) + (h2 * sin(rotation)) + pos.y;

		DrawTriangle(right, top, left, color);
		if (player) { triangle = Triangle(top, left, right); };

		Vector2 points[] = { top, left, right };
		for (Vector2 point : points)
		{
			int mirrors = 0;

			if (point.x < 0 || point.x > GetScreenWidth()) { mirrors++; }
			if (point.y < 0 || point.y > GetScreenHeight()) { mirrors++; }

			Vector2 newPoints[3] = { Vector2(), Vector2(), Vector2() };
			if (point.x < 0 || point.x > GetScreenWidth())
			{
				newPoints[0] = { points[0].x + ((point.x < 0) ? (float)GetScreenWidth() : -(float)GetScreenWidth()), points[0].y };
				newPoints[1] = { points[1].x + ((point.x < 0) ? (float)GetScreenWidth() : -(float)GetScreenWidth()), points[1].y };
				newPoints[2] = { points[2].x + ((point.x < 0) ? (float)GetScreenWidth() : -(float)GetScreenWidth()), points[2].y };
			}
			else if (point.y < 0 || point.y > GetScreenHeight())
			{
				newPoints[0] = { points[0].x, points[0].y + ((point.y < 0) ? (float)GetScreenHeight() : -(float)GetScreenHeight()) };
				newPoints[1] = { points[1].x, points[1].y + ((point.y < 0) ? (float)GetScreenHeight() : -(float)GetScreenHeight()) };
				newPoints[2] = { points[2].x, points[2].y + ((point.y < 0) ? (float)GetScreenHeight() : -(float)GetScreenHeight()) };
			}
			DrawTriangle(newPoints[0], newPoints[1], newPoints[2], color);

			if (mirrors > 1)
			{
				Vector2 newPoints2[3] = { Vector2(), Vector2(), Vector2() };

				newPoints2[0].x = points[0].x + ((point.x < 0) ? GetScreenWidth() : -GetScreenWidth());
				newPoints2[1].x = points[1].x + ((point.x < 0) ? GetScreenWidth() : -GetScreenWidth());
				newPoints2[2].x = points[2].x + ((point.x < 0) ? GetScreenWidth() : -GetScreenWidth());

				newPoints2[0].y = points[0].y + ((point.y < 0) ? GetScreenHeight() : -GetScreenHeight());
				newPoints2[1].y = points[1].y + ((point.y < 0) ? GetScreenHeight() : -GetScreenHeight());
				newPoints2[2].y = points[2].y + ((point.y < 0) ? GetScreenHeight() : -GetScreenHeight());

				DrawTriangle(newPoints2[0], newPoints2[1], newPoints2[2], color);
			}
		}
	}
};

class PlayerTardis
{
	public:

	Vector2 position;
	Vector2 velocity;
	float rotation;

	PlayerTardis(Vector2 _position, Vector2 _velocity, float _rotation)
	{
		position = _position;
		velocity = _velocity;
		rotation = _rotation;
	}
};

//class Tardis
//{
//	public:
//
//	double startTime;
//	vector<double> frameTimes;
//	vector<PlayerTardis> player;
//
//	Tardis(Player _player)
//	{
//		startTime = GetTime();
//		frameTimes = {};
//		player = {};
//		UpdateValues(_player);
//	}
//
//	void UpdateValues(Player _player)
//	{
//		frameTimes.push_back(GetTime());
//		player.push_back(PlayerTardis(_player.pos, _player.velocity, _player.rotation));
//	}
//
//	int ClosestEntry(double time)
//	{
//		int roundedTime = round(time);
//
//		for (int i = 0; i < frameTimes.size(); i++)
//		{
//			if ()
//		}
//	}
//};

//class Tardis
//{
//	public:
//	vector<double> timeHistory;
//	vector<Vector2> positionHistory;
//	vector<Vector2> velocityHistory;
//	vector<float> rotationHistory;
//	int currentIndex;
//
//	Tardis()
//	{
//		timeHistory = {};
//		positionHistory = {};
//		velocityHistory = {};
//		rotationHistory = {};
//		currentIndex = 0;
//	}
//
//	void UpdateValues(Vector2 pos, Vector2 vel, float rot)
//	{
//		timeHistory.push_back(GetTime());
//		positionHistory.push_back(pos);
//		velocityHistory.push_back(vel);
//		rotationHistory.push_back(rot);
//		UpdateIndex();
//	}
//
//	void UpdateIndex()
//	{
//		if (timeHistory[currentIndex + 1] < GetTime())
//		{
//			while (timeHistory[currentIndex + 1] < GetTime())
//			{
//				currentIndex++;
//			}
//		}
//		else if (timeHistory[currentIndex] > GetTime())
//		{
//			while (timeHistory[currentIndex] > GetTime())
//			{
//				currentIndex--;
//			}
//		}
//	}
//
//	Vector2 GetPosition(double time)
//	{
//		UpdateIndex();
//		float t = (time - timeHistory[currentIndex]) / (timeHistory[currentIndex + 1] - timeHistory[currentIndex]);
//		return Vector2Lerp(positionHistory[currentIndex], positionHistory[currentIndex + 1], t);
//	}
//	Vector2 GetVelocity(double time)
//	{
//		UpdateIndex();
//		float t = (time - timeHistory[currentIndex]) / (timeHistory[currentIndex + 1] - timeHistory[currentIndex]);
//		return Vector2Lerp(velocityHistory[currentIndex], velocityHistory[currentIndex + 1], t);
//	}
//	float GetRotation(double time)
//	{
//		UpdateIndex();
//		float t = (time - timeHistory[currentIndex]) / (timeHistory[currentIndex + 1] - timeHistory[currentIndex]);
//		return Lerp(rotationHistory[currentIndex], rotationHistory[currentIndex + 1], t);
//	}
//};
//
//class FakePlayer
//{
//	public:
//	Player player;
//	double birthTime;
//
//	FakePlayer(Player &_player)
//	{
//		player = _player;
//		birthTime = GetTime();
//	}
//
//	void Draw(Tardis tardis)
//	{
//		double time = birthTime - (GetTime() - birthTime);
//		if (time <= 0) 
//		{ 
//			return; 
//		}
//
//		Vector2 pos = tardis.GetPosition(time);
//		float rotation = tardis.GetRotation(time);
//		float size = player.size;
//		float widthAngle = player.widthAngle;
//
//		Vector2 top;
//		top.x = (0 * cos(rotation)) - (-size * sin(rotation)) + pos.x;
//		top.y = (-size * cos(rotation)) + (0 * sin(rotation)) + pos.y;
//
//		Vector2 left;
//		float h = -size * sin(widthAngle * (PI / 180));
//		float v = size * cos(widthAngle * (PI / 180));
//		left.x = (h * cos(rotation)) - (v * sin(rotation)) + pos.x;
//		left.y = (v * cos(rotation)) + (h * sin(rotation)) + pos.y;
//
//		Vector2 right;
//		float h2 = size * sin(widthAngle * (PI / 180));
//		float v2 = size * cos(widthAngle * (PI / 180));
//		right.x = (h2 * cos(rotation)) - (v2 * sin(rotation)) + pos.x;
//		right.y = (v2 * cos(rotation)) + (h2 * sin(rotation)) + pos.y;
//
//		DrawTriangle(right, top, left, PURPLE);
//	}
//};