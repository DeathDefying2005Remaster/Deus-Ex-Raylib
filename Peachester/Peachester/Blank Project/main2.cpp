/*#include <raylib-cpp.hpp>
#include "grid.hpp"
#include "stateMachine.hpp"
#include "global.hpp"
#include "fightMenu.hpp"
#include "encounter.hpp"
#include <array>
using namespace std;

const int screenWidth = 1280;
const int screenHeight = 960;
const int targetFps = 60;

Grid grid;
Music girtSong;

int turn = 0;

Hero krisHero = Hero("Berdly", 160, 3, 0, {});
Hero susieHero = Hero("Berdly", 160, 3, 1, {});
Hero berdlyHero = Hero("Berdly", 160, 3, 2, {});

Enemy evilKris = Enemy(100, 160, 0, "Evil Kris");
Enemy evilSusie = Enemy(15, 190, 0, "Evil Susie");
Enemy evilBerdly = Enemy(60, 150, 0, "Evil Berdly");
std::array<Enemy, 3> enemies = { evilKris, evilSusie, evilBerdly };

Encounter encounter = Encounter({ evilKris, evilSusie, evilBerdly }, { &krisHero, &susieHero, &berdlyHero }, "* The  Roaring  Berdly  roars  berdishly.");

PlayerMenu berdly = PlayerMenu(&encounter, &turn, 0);
PlayerMenu susie = PlayerMenu(&encounter, &turn, 1);
PlayerMenu ralsei = PlayerMenu(&encounter, &turn, 2);

class Explode : public Action
{
	public:

	Explode()
	{
		name = "Explode";
		description = "Berdly\nblows up";
	}

	bool Available() override
	{
		return true;
	}

	void Use(Hero* user, Hero* reciever) override
	{
		Print("AAAAGGGGHHHHHH");
		evilKris.hp = 0;
		evilSusie.hp = 0;
		evilBerdly.hp = 0;
		user->hp = 0;
	}
};
Explode explodeAct;

class KFC : public Action
{
	public:

	KFC()
	{
		name = "KFC";
		description = "Delectable fried chicken.";
	}

	bool Available() override
	{
		return true;
	}

	void Use(Hero* user, Hero* reciever) override
	{
		Print("Yum.");
		user->hp = Clamp(user->hp + 50, 0, user->maxHp);
	}
};
KFC kfcItem;

void Start()
{
	bigFont.texture = LoadTexture("resources/fnt_mainbig.png");
	hpFont.texture = LoadTexture("resources/hp_font.png");
	girtSong = LoadMusicStream("resources/girt234.mp3");

	berdly.Initialize();
	susie.Initialize();
	ralsei.Initialize();

	krisHero.acts.push_back(&explodeAct);
	susieHero.acts.push_back(&explodeAct);
	berdlyHero.acts.push_back(&explodeAct);
	encounter.items.push_back(&kfcItem);

	krisHero.hp = 100;
	susieHero.hp = 80;
}

int main()
{
	//---- Start ----//
	InitWindow(screenWidth, screenHeight, "DELTAGOON");
	SetTargetFPS(targetFps);
	InitAudioDevice();
	Start();

	//PlayMusicStream(girtSong);

	while (!WindowShouldClose())
	{
		//UpdateMusicStream(girtSong);

		int unalteredTurn = turn;
		if (unalteredTurn == 0) berdly.Update();
		if (unalteredTurn == 1) susie.Update();
		if (unalteredTurn == 2) ralsei.Update();
		if (turn != unalteredTurn)
		{
			if (turn == 0) berdly.Reset();
			if (turn == 1) susie.Reset();
			if (turn == 2) ralsei.Reset();
		}
		if (unalteredTurn == 3)
		{
			Print("ARE YOU READY FOR THIS?");
		}

		//---- Rendering ----//
		BeginDrawing();

		ClearBackground(BLACK);

		grid.Draw();
		Color prurle = { 51, 32, 51, 255 };
		DrawRectangle(0, 654, screenWidth, 753, BLACK);
		DrawRectangle(0, 650, screenWidth, 4, prurle);
		DrawRectangle(0, 724, screenWidth, 6, prurle);
		berdly.Draw();
		susie.Draw();
		ralsei.Draw();

		EndDrawing();
	}

	//StopMusicStream(girtSong);
	//UnloadMusicStream(girtSong);

	CloseAudioDevice();
	CloseWindow();

	return 0;
}*/