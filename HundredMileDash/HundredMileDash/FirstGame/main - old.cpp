#include <raylib-cpp.hpp>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int targetFps = 60;

Camera2D camera;
bool gameover;
float globalTimeScale;
Vector2 pPos;
Vector2 pVelocity;
float pSize = 40;
float pWidthAngle;
float minWidthAngle = 35;
float maxWidthAngle = 45;
float pSpeed = 3000;
float pMaxVelocity = 800;
float pDrag = 0.05;
float rotation;
float rotationSpeed;
float minRotSpeed = 3;
float maxRotSpeed = 6;
bool holdingDash;
float dashTime = 0.4;
float minDashVelocity = pMaxVelocity * 0.6;
float ghostFade;
float ghostFadeSpeed = 4;
float pieceShrinkSpeed = 0.005;
float pieceSpinSpeed = 0.2;
float pieceMoveSpeed = 0.1;
float screenShake;
float screenShakeTime = 0.5;
float screenShakeMagnitude = 10;
float juice;
float juiceRecharge = 0.1;
float juicePlanDecay = 0.3;
float juiceRequiredDash = 0;
float juiceCostDash = (float)1/3;

Vector2 DownClampVector2(Vector2 vector, float magnitude)
{
	if (Vector2Length(vector) > magnitude)
	{
		float factor = magnitude / Vector2Length(vector);
		Vector2 vectorNew = { vector.x * factor, vector.y * factor };

		return vectorNew;
	}

	return vector;
}

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

	void Update()
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
vector<ParticleEffect> particleEffects = {};

//class Triangle
//{
//	public:
//	Vector2 centre;
//	Vector2 pointTop;
//	Vector2 pointLeft;
//	Vector2 pointRight;
//	float tRotation;
//
//	Triangle(Vector2 top, Vector2 left, Vector2 right)
//	{
//		pointTop = top;
//		pointLeft = left;
//		pointRight = right;
//		tRotation = 0;
//
//		Vector2 middle = { Lerp(pointLeft.x, pointRight.x, 0.5), Lerp(pointLeft.y, pointRight.y, 0.5) };
//		centre = { Lerp(middle.x, pointTop.x, 0.5), Lerp(middle.y, pointTop.y, 0.5) };
//	}
//	Triangle()
//	{
//		pointTop = Vector2();
//		pointLeft = Vector2();
//		pointRight = Vector2();
//		tRotation = 0;
//		centre = Vector2();
//	}
//
//	void Rotate(float rotDegrees)
//	{
//		tRotation += rotDegrees * PI/180;
//
//		Vector2 middle = { Lerp(pointLeft.x, pointRight.x, 0.5), Lerp(pointLeft.y, pointRight.y, 0.5) };
//		centre = { Lerp(middle.x, pointTop.x, 0.5), Lerp(middle.y, pointTop.y, 0.5) };
//
//		pointTop.x = ((pointTop.x - centre.x) * cos(tRotation)) - ((pointTop.y - centre.y) * sin(tRotation)) + centre.x;
//		pointTop.y = ((pointTop.y - centre.y) * cos(tRotation)) + ((pointTop.x - centre.x) * sin(tRotation)) + centre.y;
//		
//		pointLeft.x = ((pointLeft.x - centre.x) * cos(tRotation)) - ((pointLeft.y - centre.y) * sin(tRotation)) + centre.x;
//		pointLeft.y = ((pointLeft.y - centre.y) * cos(tRotation)) + ((pointLeft.x - centre.x) * sin(tRotation)) + centre.y;
//
//		pointRight.x = ((pointRight.x - centre.x) * cos(tRotation)) - ((pointRight.y - centre.y) * sin(tRotation)) + centre.x;
//		pointRight.y = ((pointRight.y - centre.y) * cos(tRotation)) + ((pointRight.x - centre.x) * sin(tRotation)) + centre.y;
//	}
//};
Triangle ghosts[10];
Triangle pTriangle = Triangle();
vector<Triangle> pieces = {};
vector<Vector2> piecePositions = {};

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
		speed = 1200;
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

	void Destroy()
	{
		alive = false;
		ParticleEffect effect = ParticleEffect(0.6, 150, 15, 10, pos, RED);
		particleEffects.push_back(effect);
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

	void MoveEnemy(vector<Enemy> enemyList)
	{
		Vector2 direction = Vector2Normalize({ pPos.x - pos.x, pPos.y - pos.y });
		Vector2 change = Vector2Scale(direction, speed * GetFrameTime() * globalTimeScale);
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
		else if (pos.x > screenWidth - size) { pos.x = screenWidth - size; }
		if (pos.y < size) { pos.y = size; }
		else if (pos.y > screenHeight - size) { pos.y = screenHeight - size; }
	}
};
vector<Enemy> enemies;

vector<Triangle> SplitTriangleRandom(Triangle triangle)
{
	vector<Triangle> triangles = { Triangle(), Triangle() };

	int rand = GetRandomValue(0, 2);
	if (rand == 0)
	{
		triangles[0].pointTop.x = Lerp(triangle.pointTop.x, triangle.pointLeft.x, 0.5);
		triangles[0].pointTop.y = Lerp(triangle.pointTop.y, triangle.pointLeft.y, 0.5);
		triangles[0].pointLeft = triangle.pointLeft;
		triangles[0].pointRight = triangle.pointRight;

		triangles[1].pointTop = triangle.pointTop;
		triangles[1].pointLeft.x = Lerp(triangle.pointTop.x, triangle.pointLeft.x, 0.5);
		triangles[1].pointLeft.y = Lerp(triangle.pointTop.y, triangle.pointLeft.y, 0.5);
		triangles[1].pointRight = triangle.pointRight;
	}
	else if (rand == 1)
	{
		triangles[0].pointTop.x = Lerp(triangle.pointTop.x, triangle.pointRight.x, 0.5);
		triangles[0].pointTop.y = Lerp(triangle.pointTop.y, triangle.pointRight.y, 0.5);
		triangles[0].pointLeft = triangle.pointLeft;
		triangles[0].pointRight = triangle.pointRight;

		triangles[1].pointTop = triangle.pointTop;
		triangles[1].pointLeft = triangle.pointLeft;
		triangles[1].pointRight.x = Lerp(triangle.pointTop.x, triangle.pointRight.x, 0.5);
		triangles[1].pointRight.y = Lerp(triangle.pointTop.y, triangle.pointRight.y, 0.5);
	}
	else if (rand == 2)
	{
		triangles[0].pointTop = triangle.pointTop;
		triangles[0].pointLeft = triangle.pointLeft;
		triangles[0].pointRight.x = Lerp(triangle.pointLeft.x, triangle.pointRight.x, 0.5);
		triangles[0].pointRight.y = Lerp(triangle.pointLeft.y, triangle.pointRight.y, 0.5);

		triangles[1].pointTop = triangle.pointTop;
		triangles[1].pointLeft.x = Lerp(triangle.pointLeft.x, triangle.pointRight.x, 0.5);
		triangles[1].pointLeft.y = Lerp(triangle.pointLeft.y, triangle.pointRight.y, 0.5);
		triangles[1].pointRight = triangle.pointRight;
	}

	return triangles;
}

vector<Triangle> BreakTriangle(Triangle triangle, int slices)
{
	vector<Triangle> triangles = { triangle };

	for (int i = 0; i < slices; i++)
	{
		vector<Triangle> newTriangles = {};
		for (Triangle tri : triangles)
		{
			vector<Triangle> newTris = SplitTriangleRandom(tri);
			newTriangles.push_back(newTris[0]);
			newTriangles.push_back(newTris[1]);
		}
		triangles = newTriangles;
	}

	return triangles;
}

bool CheckTriangleInRectangle(Triangle triangle, Rectangle rectangle)
{
	float upperX = rectangle.x + rectangle.width;
	float lowerX = rectangle.x;
	float upperY = rectangle.y + rectangle.height;
	float lowerY = rectangle.y;
	if (triangle.pointTop.x < upperX && triangle.pointTop.x > lowerX)
	{
		if (triangle.pointTop.y < upperY && triangle.pointTop.y > lowerY)
		{
			return true;
		}
	}
	if (triangle.pointLeft.x < upperX && triangle.pointLeft.x > lowerX)
	{
		if (triangle.pointLeft.y < upperY && triangle.pointLeft.y > lowerY)
		{
			return true;
		}
	}
	if (triangle.pointRight.x < upperX && triangle.pointRight.x > lowerX)
	{
		if (triangle.pointRight.y < upperY && triangle.pointRight.y > lowerY)
		{
			return true;
		}
	}

	return false;
}

Vector2 PlayerPrediction()
{
	/*Vector2 newPosition = pPos;
	Vector2 newVelocity = pVelocity;
	float newRotation = rotation;
	
	float time = dashTime;
	while (time > 0)
	{
		time -= (float)1/targetFps;

		if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && !(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)))
		{
			newRotation -= rotationSpeed * ((float)1/targetFps) * globalTimeScale;
		}
		if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && !(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)))
		{
			newRotation += rotationSpeed * ((float)1/targetFps) * globalTimeScale;
		}
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
		{
			newVelocity.x -= pSpeed * sin(-newRotation) * ((float)1 / targetFps) * globalTimeScale;
			newVelocity.y -= pSpeed * cos(-newRotation) * ((float)1 / targetFps) * globalTimeScale;
		}

		newPosition.x += newVelocity.x * ((float)1 / targetFps) * globalTimeScale;
		newPosition.y += newVelocity.y * ((float)1 / targetFps) * globalTimeScale;
	}

	return newPosition;*/

	return { pPos.x + (pVelocity.x * dashTime), pPos.y + (pVelocity.y * dashTime) };
}

void DrawPlayer(Vector2 pos, Color color, bool player)
{
	Vector2 top;
	top.x = (0 * cos(rotation)) - (-pSize * sin(rotation)) + pos.x;
	top.y = (-pSize * cos(rotation)) + (0 * sin(rotation)) + pos.y;

	Vector2 left;
	float h = -pSize * sin(pWidthAngle * (PI / 180));
	float v = pSize * cos(pWidthAngle * (PI / 180));
	left.x = (h * cos(rotation)) - (v * sin(rotation)) + pos.x;
	left.y = (v * cos(rotation)) + (h * sin(rotation)) + pos.y;

	Vector2 right;
	float h2 = pSize * sin(pWidthAngle * (PI / 180));
	float v2 = pSize * cos(pWidthAngle * (PI / 180));
	right.x = (h2 * cos(rotation)) - (v2 * sin(rotation)) + pos.x;
	right.y = (v2 * cos(rotation)) + (h2 * sin(rotation)) + pos.y;

	DrawTriangle(right, top, left, color);
	if (player) { pTriangle = Triangle(top, left, right); };

	Vector2 points[] = { top, left, right };
	for (Vector2 point : points)
	{
		int mirrors = 0;

		if (point.x < 0 || point.x > screenWidth) { mirrors++; }
		if (point.y < 0 || point.y > screenHeight) { mirrors++; }

		Vector2 newPoints[3] = { Vector2(), Vector2(), Vector2() };
		if (point.x < 0 || point.x > screenWidth)
		{
			newPoints[0] = { points[0].x + ((point.x < 0) ? (float)screenWidth : -(float)screenWidth), points[0].y };
			newPoints[1] = { points[1].x + ((point.x < 0) ? (float)screenWidth : -(float)screenWidth), points[1].y };
			newPoints[2] = { points[2].x + ((point.x < 0) ? (float)screenWidth : -(float)screenWidth), points[2].y };
		}
		else if (point.y < 0 || point.y > screenHeight)
		{
			newPoints[0] = { points[0].x, points[0].y + ((point.y < 0) ? (float)screenHeight : -(float)screenHeight) };
			newPoints[1] = { points[1].x, points[1].y + ((point.y < 0) ? (float)screenHeight : -(float)screenHeight) };
			newPoints[2] = { points[2].x, points[2].y + ((point.y < 0) ? (float)screenHeight : -(float)screenHeight) };
		}
		DrawTriangle(newPoints[0], newPoints[1], newPoints[2], color);

		if (mirrors > 1)
		{
			Vector2 newPoints2[3] = { Vector2(), Vector2(), Vector2() };

			newPoints2[0].x = points[0].x + ((point.x < 0) ? screenWidth : -screenWidth);
			newPoints2[1].x = points[1].x + ((point.x < 0) ? screenWidth : -screenWidth);
			newPoints2[2].x = points[2].x + ((point.x < 0) ? screenWidth : -screenWidth);

			newPoints2[0].y = points[0].y + ((point.y < 0) ? screenHeight : -screenHeight);
			newPoints2[1].y = points[1].y + ((point.y < 0) ? screenHeight : -screenHeight);
			newPoints2[2].y = points[2].y + ((point.y < 0) ? screenHeight : -screenHeight);

			DrawTriangle(newPoints2[0], newPoints2[1], newPoints2[2], color);
		}
	}
}

void DrawDeadPlayer()
{
	for (int i = 0; i < end(pieces) - begin(pieces); i++)
	{
		DrawTriangle(pieces[i].pointTop, pieces[i].pointLeft, pieces[i].pointRight, BLUE);
		
		Vector2 push = { (piecePositions[i].x - pPos.x) * pieceMoveSpeed, (piecePositions[i].y - pPos.y) * pieceMoveSpeed };

		Vector2 middle = { Lerp(pieces[i].pointLeft.x, pieces[i].pointRight.x, 0.5), Lerp(pieces[i].pointLeft.y, pieces[i].pointRight.y, 0.5) };
		Vector2 pos = { Lerp(middle.x, pieces[i].pointTop.x, 0.5), Lerp(middle.y, pieces[i].pointTop.y, 0.5) };

		pieces[i].pointTop.x += push.x + ((pos.x - pieces[i].pointTop.x) * pieceShrinkSpeed);
		pieces[i].pointTop.y += push.y + ((pos.y - pieces[i].pointTop.y) * pieceShrinkSpeed);
		pieces[i].pointLeft.x += push.x + ((pos.x - pieces[i].pointLeft.x) * pieceShrinkSpeed);
		pieces[i].pointLeft.y += push.y + ((pos.y - pieces[i].pointLeft.y) * pieceShrinkSpeed);
		pieces[i].pointRight.x += push.x + ((pos.x - pieces[i].pointRight.x) * pieceShrinkSpeed);
		pieces[i].pointRight.y += push.y + ((pos.y - pieces[i].pointRight.y) * pieceShrinkSpeed);

		pieces[i].Rotate(pieceSpinSpeed);
	}
}

void DrawDash()
{
	Color color = BLUE;
	int count = sizeof(ghosts) / sizeof(Triangle);
	for (int i = 0; i < count; i++)
	{
		DrawTriangle(ghosts[i].pointTop, ghosts[i].pointLeft, ghosts[i].pointRight, ColorAlpha(color, ghostFade * Lerp(0.1, 1, (float)i / (float)count)));
	}

	ghostFade -= ghostFadeSpeed * GetFrameTime();
	if (ghostFade < 0) { ghostFade = 0; }
}

void KillPlayer()
{
	globalTimeScale = 0;
	gameover = true;

	Vector2 top = { (0 * cos(rotation)) - (-pSize * sin(rotation)) + pPos.x, (-pSize * cos(rotation)) + (0 * sin(rotation)) + pPos.y };
	float h = -pSize * sin(pWidthAngle * (PI / 180));
	float v = pSize * cos(pWidthAngle * (PI / 180));
	Vector2 left = { (h * cos(rotation)) - (v * sin(rotation)) + pPos.x, (v * cos(rotation)) + (h * sin(rotation)) + pPos.y };
	float h2 = pSize * sin(pWidthAngle * (PI / 180));
	float v2 = pSize * cos(pWidthAngle * (PI / 180));
	Vector2 right = { (h2 * cos(rotation)) - (v2 * sin(rotation)) + pPos.x, (v2 * cos(rotation)) + (h2 * sin(rotation)) + pPos.y };

	Triangle player = Triangle(top, left, right);
	pieces = BreakTriangle(player, 3);
	piecePositions.clear();
	for (Triangle piece : pieces)
	{
		Vector2 middle = { Lerp(piece.pointLeft.x, piece.pointRight.x, 0.5), Lerp(piece.pointLeft.y, piece.pointRight.y, 0.5) };
		Vector2 pos = { Lerp(middle.x, piece.pointTop.x, 0.5), Lerp(middle.y, piece.pointTop.y, 0.5) };
		piecePositions.push_back(pos);
	}
}

void PlayerDash()
{
	juice -= juiceCostDash;
	Clamp(juice, 0, 1);

	Vector2 newPos = PlayerPrediction();
	int count = sizeof(ghosts) / sizeof(Triangle);
	for (int i = 0; i < (count); i++)
	{
		Vector2 pos = { Lerp(pPos.x, newPos.x, (float)i / (float)count), Lerp(pPos.y, newPos.y, (float)i / (float)count) };
		if (pos.x < 0 || pos.x > screenWidth)
		{
			pos.x += (pos.x < 0) ? screenWidth : -screenWidth;
		}
		if (pos.y < 0 || pos.y > screenHeight)
		{
			pos.y += (pos.y < 0) ? screenHeight : -screenHeight;
		}

		float h = -pSize * sin(pWidthAngle * (PI / 180));
		float v = pSize * cos(pWidthAngle * (PI / 180));
		float h2 = pSize * sin(pWidthAngle * (PI / 180));
		float v2 = pSize * cos(pWidthAngle * (PI / 180));

		ghosts[i].pointTop = { (0 * cos(rotation)) - (-pSize * sin(rotation)) + pos.x, (-pSize * cos(rotation)) + (0 * sin(rotation)) + pos.y };
		ghosts[i].pointLeft = { (h * cos(rotation)) - (v * sin(rotation)) + pos.x, (v * cos(rotation)) + (h * sin(rotation)) + pos.y };
		ghosts[i].pointRight = { (h2 * cos(rotation)) - (v2 * sin(rotation)) + pos.x, (v2 * cos(rotation)) + (h2 * sin(rotation)) + pos.y };
	}

	pPos = newPos;

	for (int j = 0; j < end(enemies) - begin(enemies); j++)
	{
		if (CheckTriangleInRectangle(ghosts[end(ghosts) - begin(ghosts) - 1], enemies[j].GetCollider()) && enemies[j].alive)
		{
			KillPlayer();
		}
	}

	for (int i = 0; i < sizeof(ghosts) / sizeof(Triangle); i++)
	{
		for (int j = 0; j < end(enemies) - begin(enemies); j++)
		{
			if (CheckTriangleInRectangle(ghosts[i], enemies[j].GetCollider()) && enemies[j].alive)
			{
				enemies[j].Destroy();
			}
		}
	}

	ghostFade = 1;
	screenShake = screenShakeTime;
}

void Start()
{
	gameover = false;
	globalTimeScale = 1;
	pPos.x = 400;
	pPos.y = 400;
	pVelocity = Vector2();
	rotation = 0;
	rotationSpeed = minRotSpeed;
	pWidthAngle = maxWidthAngle;
	holdingDash = false;
	ghostFade = 0;

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
			//---- Input ----//
			if (IsKeyPressed(KEY_R))
			{
				Start();
			}

			if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && !(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)))
			{
				rotation -= rotationSpeed * GetFrameTime() * globalTimeScale;
			}
			if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && !(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)))
			{
				rotation += rotationSpeed * GetFrameTime() * globalTimeScale;
			}
			if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
			{
				pVelocity.x -= pSpeed * sin(-rotation) * GetFrameTime() * globalTimeScale;
				pVelocity.y -= pSpeed * cos(-rotation) * GetFrameTime() * globalTimeScale;
			}

			if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(0) && juice > juiceRequiredDash)
			{
				holdingDash = true;
				globalTimeScale = 0.2;
			}
			else if (holdingDash && juice < juiceRequiredDash)
			{
				holdingDash = false;
				globalTimeScale = 1;
			}
			else if ((IsKeyReleased(KEY_SPACE) || IsMouseButtonReleased(0)) && holdingDash)
			{
				holdingDash = false;
				globalTimeScale = 1;
				if (sqrt(pow(pVelocity.x, 2) + pow(pVelocity.y, 2)) >= minDashVelocity && juice >= juiceRequiredDash)
				{
					PlayerDash();
				}
			}

			//---- Dashing ----//
			juice += juiceRecharge * GetFrameTime();
			if (juice > 1) { juice = 1; }

			if (holdingDash && sqrt(pow(pVelocity.x, 2) + pow(pVelocity.y, 2)) >= minDashVelocity)
			{
				Vector2 newPos = PlayerPrediction();
				DrawPlayer(newPos, SKYBLUE, false);
				juice -= juicePlanDecay * GetFrameTime();
			}

			//---- Movement ----//
			pVelocity = DownClampVector2(pVelocity, pMaxVelocity);

			pPos.x += pVelocity.x * GetFrameTime() * globalTimeScale;
			pPos.y += pVelocity.y * GetFrameTime() * globalTimeScale;
			if (pPos.x < 0)
			{
				pPos.x = screenWidth + pPos.x;
			}
			else if (pPos.x > screenWidth)
			{
				pPos.x = pPos.x - screenWidth;
			}
			if (pPos.y < 0)
			{
				pPos.y = screenHeight + pPos.y;
			}
			else if (pPos.y > screenHeight)
			{
				pPos.y = pPos.y - screenHeight;
			}

			pVelocity.x *= (1 - (pDrag * globalTimeScale));
			pVelocity.y *= (1 - (pDrag * globalTimeScale));

			rotationSpeed = Lerp(minRotSpeed, maxRotSpeed, sqrt(pow(pVelocity.x, 2) + pow(pVelocity.y, 2)) / pMaxVelocity);
			pWidthAngle = Lerp(maxWidthAngle, minWidthAngle, sqrt(pow(pVelocity.x, 2) + pow(pVelocity.y, 2)) / pMaxVelocity);

			//---- Enemy Behaviour ----//
			for (int i = 0; i < end(enemies) - begin(enemies); i++)
			{
				enemies[i].MoveEnemy(enemies);
			}

			//---- Collision ----//
			for (int i = 0; i < end(enemies) - begin(enemies); i++)
			{
				if (enemies[i].alive)
				{
					Rectangle circleBox = enemies[i].GetCollider();

					if (CheckTriangleInRectangle(pTriangle, circleBox))
					{
						KillPlayer();
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

		DrawDash();
		if (!gameover) { DrawPlayer(pPos, BLUE, true); }
		for (int i = 0; i < end(enemies) - begin(enemies); i++)
		{
			enemies[i].DrawEnemy();
			enemies[i].bladeRotation += enemies[i].bladeSpeed * GetFrameTime();
		}

		if (gameover) { DrawDeadPlayer(); DrawText("PRESS SPACEBAR TO RESTART", 100, 190, 25, DARKGRAY); }	

		if (!gameover)
		{
			for (int i = 0; i < end(particleEffects) - begin(particleEffects); i++)
			{
				particleEffects[i].Update();
				particleEffects[i].DrawParticleEffect();
			}

			string strr = to_string(ghostFade);
			DrawText(strr.c_str(), 100, 70, 20, DARKGRAY);
			string str = to_string(rotation);
			DrawText(str.c_str(), 100, 100, 20, DARKGRAY);
			string str2 = to_string(pVelocity.x);
			string str3 = to_string(pVelocity.y);
			DrawText(str2.c_str(), 100, 130, 20, DARKGRAY);
			DrawText(str3.c_str(), 100, 160, 20, DARKGRAY);
			if (sqrt(pow(pVelocity.x, 2) + pow(pVelocity.y, 2)) < minDashVelocity)
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