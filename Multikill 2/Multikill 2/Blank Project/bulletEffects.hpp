#pragma once
#include <raylib-cpp.hpp>
#include "global.hpp"
using namespace std;

class BulletImpact
{
	public:

	//Texture2D sheet = LoadTexture("resources/revolver_impact.png");
	Vector2 position;
	Vector2 direction;
	float lifespan;
	float timer;

	BulletImpact(Vector2 _pos, Vector2 _dir, float _lifespan)
	{
		position = _pos;
		direction = _dir;
		lifespan = _lifespan;
		timer = 0;
	}

	void Update()
	{
		timer += GetFrameTime();
	}
	void Draw()
	{
		float frame = timer / lifespan;
		raylib::Rectangle destination = raylib::Rectangle(position.x - 16, position.y - 16, 32, 32);
		float rotation = Vector2Angle({ 0, 1 }, direction);

		if (frame <= (float)1 / 3)
		{
			raylib::Rectangle rect = raylib::Rectangle(0, 0, 32, 32);
			//DrawTexturePro(sheet, rect, destination, { 0, 0 }, rotation, WHITE);
		}
		else if (frame <= (float)2 / 3)
		{
			raylib::Rectangle rect = raylib::Rectangle(32, 0, 32, 32);
			//DrawTexturePro(sheet, rect, destination, { 0, 0 }, rotation, WHITE);
		}
		else
		{
			raylib::Rectangle rect = raylib::Rectangle(64, 0, 32, 32);
			//DrawTexturePro(sheet, rect, destination, { 0, 0 }, rotation, WHITE);
		}
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
	float width;
	BulletImpact impact = BulletImpact({}, {}, 0);

	BulletTrail(Vector2 _origin, Vector2 _direction, float _lifespan, float _width)
	{
		origin = _origin;
		direction = _direction;
		lifespan = _lifespan;
		timer = 0;
		length = 2000;
		width = _width;

		for (int i = 0; i < blocks.size(); i++)
		{
			Vector3 originbutnot = { origin.x, origin.y, 0 };
			Vector3 directionbutnot = { direction.x, direction.y, 0 };
			Vector3 pos = { blocks[i].GetPosition().x, blocks[i].GetPosition().y, 0 };
			Vector3 pos2 = { blocks[i].GetPosition().x + blocks[i].width, blocks[i].GetPosition().y + blocks[i].height, 0 };
			BoundingBox box = raylib::BoundingBox(pos, pos2);
			RayCollision col = GetRayCollisionBox(raylib::Ray(originbutnot, directionbutnot), box);
			if (col.hit && col.distance < length) { length = col.distance; }
		}

		Vector2 impactPos = Vector2Add(origin, Vector2Scale(direction, length));
		impact = BulletImpact(impactPos, direction, 1);
	}

	void Update()
	{
		timer += GetFrameTime();
	}
	void Draw()
	{
		float newWidth = (1 - timer / lifespan) * width;
		DrawLineEx(origin, { origin.x + (direction.x * length), origin.y + (direction.y * length) }, newWidth, YELLOW);
		impact.Draw();
	}
};