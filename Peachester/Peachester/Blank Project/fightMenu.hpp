#pragma once
#include <raylib-cpp.hpp>
#include "global.hpp"
#include "encounter.hpp"
#include <array>

/*class PlayerMenu
{
	public:

	enum MenuState
	{
		strategy,
		fight1,
		act1,
		act2,
		item1,
		item2,
		spare1
	};
	enum MenuStrategy
	{
		fight,
		act,
		item,
		spare,
		defend
	};

	//---- References ----//

	Texture fightMenuTex = Texture();
	Texture charaIcons = Texture();
	Texture menuHeart = Texture();
	Texture hpMercy = Texture();
	Sound menuMove = Sound();
	Sound menuSelect = Sound();

	//---- Properties ----//

	float moveUpSpeed = 0.1;

	//---- Values ----//

	Encounter* encounter;
	Hero* hero;

	Vector2 charaCoords;
	Color charaColor;

	int* turn;
	int lastTurn = 0;

	int actionHighlighted = -1;

	MenuState menuState = strategy;
	MenuStrategy menuStrategy = fight;
	float menuStart = 654, menuTarget = 586, menuTimer = 0;

	std::vector<float> lines = {};
	double lastLine = 0;

	int enemyHighlighted = -1;

	Vector2 menuHeartPos = {};

	PlayerMenu(Encounter* _encounter, int* _turn, int whichHero)
	{
		encounter = _encounter;
		turn = _turn;
		hero = encounter->heroes[whichHero];

		switch (hero->chara)
		{
		case 0:
			charaCoords = { 0, 0 };
			charaColor = krisBlue;
			break;
		case 1:
			charaCoords = { 0, 38 };
			charaColor = susiePink;
			break;
		case 2:
			charaCoords = { 0, 76 };
			charaColor = ralseiGreen;
			break;
		case 3:
			charaCoords = { 213, 0 };
			charaColor = berdlyColor;
			break;
		default:
			charaCoords = { 0, 0 };
			charaColor = krisBlue;
		}
	}

	void Initialize()
	{
		fightMenuTex = LoadTexture("resources/fight_menu.png");
		charaIcons = LoadTexture("resources/chara_icons_no.png");
		menuHeart = LoadTexture("resources/spr_heart.png");
		hpMercy = LoadTexture("resources/hp_mercy.png");
		menuMove = LoadSound("resources/snd_menumove.wav");
		menuSelect = LoadSound("resources/snd_select.wav");

		SetSoundVolume(menuMove, 0.5);
		SetSoundVolume(menuSelect, 0.5);

		if (*turn != hero->place)
		{
			menuStart = 586;
			menuTarget = 654;
			menuTimer = moveUpSpeed;
		}
	}

	void Reset()
	{
		menuTimer = 0;
		hero->strategy = -1;
		hero->choice = -1;
		hero->target = -1;
		menuState = strategy;
		menuStrategy = fight;
	}

	void Update()
	{
		switch (menuState)
		{
		case strategy:
			StrategyState();
			break;
		case fight1:
			FightState();
			break;
		case act1:
			Act1State();
			break;
		case act2:
			Act2State();
			break;
		case item1:
			Item1State();
			break;
		case item2:
			Item2State();
			break;
		case spare1:
			SpareState();
			break;
		}
	}

	void SubmitTurn()
	{
		PlaySound(menuSelect);
		hero->strategy = menuStrategy;
		Print("bogey spotted@");
		Print("har: " + std::to_string(hero->strategy));
		hero->choice = actionHighlighted;
		Print("haha: " + std::to_string(hero->choice));
		hero->target = enemyHighlighted;
		Print("HAGHAGHGA: " + std::to_string(hero->target));
		*turn = hero->place + 1;
		menuTimer = 0;
	}

	void UnsubmitTurn()
	{
		if (*turn != 0)
		{
			PlaySound(menuMove);
			hero->strategy = -1;
			*turn = hero->place - 1;
			menuTimer = 0;
		}
	}

	void EnterEnemySelect()
	{
		enemyHighlighted = 0;
		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };
	}

	bool EnemySelect()
	{
		if (IsKeyPressed(KEY_DOWN) && enemyHighlighted + 1 < encounter->enemies.size())
		{
			enemyHighlighted++;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_UP) && enemyHighlighted >= 1)
		{
			enemyHighlighted--;
			PlaySound(menuMove);
		}
		if (Z())
		{
			PlaySound(menuSelect);
			return true;
		}

		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };

		return false;
	}

	bool PartySelect()
	{
		if (IsKeyPressed(KEY_DOWN) && enemyHighlighted < encounter->heroes.size() - 1)
		{
			enemyHighlighted++;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_UP) && enemyHighlighted >= 1)
		{
			enemyHighlighted--;
			PlaySound(menuMove);
		}
		if (Z())
		{
			PlaySound(menuSelect);
			return true;
		}

		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };

		return false;
	}

	void EnterActionSelect()
	{
		actionHighlighted = 0;
		menuHeartPos = { 110, 770 };
	}

	bool ActionSelect(std::vector<Action*> list)
	{
		if (IsKeyPressed(KEY_DOWN) && actionHighlighted + 2 < list.size())
		{
			actionHighlighted += 2;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_UP) && actionHighlighted >= 2)
		{
			actionHighlighted -= 2;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_LEFT) && actionHighlighted % 2 == 1)
		{
			actionHighlighted--;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_RIGHT) && actionHighlighted % 2 == 0 && actionHighlighted + 1 < list.size())
		{
			actionHighlighted++;
			PlaySound(menuMove);
		}
		if (Z())
		{
			PlaySound(menuSelect);
			return true;
		}

		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };

		return false;
	}

	void StrategyState()
	{
		if (IsKeyPressed(KEY_RIGHT))
		{
			PlaySound(menuMove);
			if (menuStrategy < 4) menuStrategy = MenuStrategy(menuStrategy + 1);
			else menuStrategy = MenuStrategy(0);
		}
		else if (IsKeyPressed(KEY_LEFT))
		{
			PlaySound(menuMove);
			if (menuStrategy > 0) menuStrategy = MenuStrategy(menuStrategy - 1);
			else menuStrategy = MenuStrategy(4);
		}
		else if (Z())
		{
			PlaySound(menuSelect);
			switch (menuStrategy)
			{
			case fight:
				menuState = fight1;
				EnterEnemySelect();
				break;
			case act:
				menuState = act1;
				EnterEnemySelect();
				break;
			case item:
				menuState = item1;
				EnterActionSelect();
				break;
			case spare:
				menuState = spare1;
				EnterEnemySelect();
				break;
			case defend:
				SubmitTurn();
				break;
			}
		}
		else if (X())
		{
			UnsubmitTurn();
		}
	}

	void FightState()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (EnemySelect())
		{
			SubmitTurn();
		}
	}

	void Act1State()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (EnemySelect())
		{
			menuState = act2;
			EnterActionSelect();
			PlaySound(menuSelect);
		}
	}

	void Act2State()
	{
		if (X())
		{
			menuState = act1;
			PlaySound(menuMove);
			return;
		}
		if (ActionSelect(hero->acts))
		{
			SubmitTurn();
		}
	}

	void Item1State()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (ActionSelect(encounter->items))
		{
			menuState = item2;
			EnterEnemySelect();
			PlaySound(menuSelect);
		}
	}

	void Item2State()
	{
		if (X())
		{
			menuState = item1;
			PlaySound(menuMove);
			return;
		}
		if (PartySelect())
		{
			SubmitTurn();
		}
	}

	void SpareState()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (EnemySelect())
		{
			SubmitTurn();
		}
	}

	void Draw()
	{
		menuStart = *turn == hero->place ? 654 : 586;
		menuTarget = *turn == hero->place ? 586 : 654;
		DrawChara();
		if (*turn == hero->place) DrawBottomMenu();
	}

	void DrawChara()
	{
		Vector2 menuPos = hero->place == 0 ? Vector2(40, 666)
			: (hero->place == 1 ? Vector2(466, 666)
				: Vector2(892, 666));
		float borderStart = menuPos.x - 40;
		float borderEnd = borderStart + 422;
		Rectangle fightSource = { 0, float(menuStrategy == fight ? 32 : 0), 31, 32 };
		Rectangle actSource = { 31, float(menuStrategy == act ? 32 : 0), 31, 32 };
		Rectangle magicSource = { 31 * 2, float(menuStrategy == act ? 32 : 0), 31, 32 };
		Rectangle itemSource = { 31 * 3, float(menuStrategy == item ? 32 : 0), 31, 32 };
		Rectangle spareSource = { 31 * 4, float(menuStrategy == spare ? 32 : 0), 31, 32 };
		Rectangle defendSource = { 31 * 5, float(menuStrategy == defend ? 32 : 0), 31, 32 };
		Vector2 charaScale = *turn == hero->place ? Vector2(213, 38) : Vector2(209, 32);
		float menuHeight = Slerp(menuStart, menuTarget, moveUpSpeed, menuTimer);
		Vector2 charaDest = { borderStart, menuHeight };

		DrawRectangle(charaDest.x, charaDest.y, 213 * 2, 32 * 2, BLACK);

		Rectangle source = { charaCoords.x, charaCoords.y, charaScale.x, charaScale.y };
		if (*turn != hero->place) { source.x += 2; source.y += 2; charaDest.x += 4; charaDest.y += 4; }
		DrawTexturePro(charaIcons, source, { charaDest.x, charaDest.y, charaScale.x * 2, charaScale.y * 2 }, {}, 0, WHITE);

		Vector2 choiceIcon = { 150, 115 };
		switch (hero->strategy)
		{
		case 0:
			choiceIcon = { 0, 114 };
			break;
		case 1:
			choiceIcon = { 0, 149 };
			break;
		case 2:
			choiceIcon = { 100, 114 };
			break;
		case 3:
			choiceIcon = { 100, 149 };
			break;
		case 4:
			choiceIcon = { 50, 149 };
			break;
		}
		Rectangle sillyDest = { charaDest.x, charaDest.y, 98, 66 };
		DrawTexturePro(charaIcons, { choiceIcon.x, choiceIcon.y, 49, 33 }, sillyDest, {}, 0, charaColor);

		std::string hpString = std::to_string(hero->hp);
		hpFont.Draw(hpString, { borderStart + 320 - (hpString.size() * 16), menuHeight + 16 }, 4, 0, 56, WHITE);
		hpString = std::to_string(hero->maxHp);
		hpFont.Draw(hpString, { borderStart + 410 - (hpString.size() * 16), menuHeight + 16 }, 4, 0, 56, WHITE);

		if (*turn == hero->place)
		{
			float lineSpeed = 0.5;
			float lineInterval = 0.5;
			if (GetTime() - lastLine >= lineInterval)
			{
				lines.push_back(0);
				lastLine = GetTime();
			}
			for (int i = 0; i < lines.size(); i++)
			{
				Color color = charaColor;
				color.a = floor(255 * float((60 - lines[i]) / 60));
				DrawRectangle(borderStart + lines[i], 662, 4, 62, color);
				DrawRectangle(borderEnd - lines[i], 662, 4, 62, color);

				lines[i] += lineSpeed + (lines[i] / 60 * lineSpeed);
				if (lines[i] > 60) lines.erase(lines.begin() + i);
			}

			DrawRectangle(borderStart, 662, 4, 62, charaColor);
			DrawRectangle(borderEnd, 662, 4, 62, charaColor);

			DrawTexturePro(fightMenuTex, fightSource, { menuPos.x, menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, magicSource, { menuPos.x + (35 * 2), menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, itemSource, { menuPos.x + (35 * 4), menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, spareSource, { menuPos.x + (35 * 6), menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, defendSource, { menuPos.x + (35 * 8), menuPos.y, 62, 64 }, {}, 0, WHITE);
		}
	}

	void DrawActionSelect(std::vector<Action*> list)
	{
		menuHeartPos = { float(20 + ((actionHighlighted % 2) * 440)), float(766 + ((actionHighlighted - (actionHighlighted % 2)) / 2 * 60)) };
		DrawTexturePro(menuHeart, { 0, 0, 16, 16 }, { menuHeartPos.x, menuHeartPos.y, 32, 32 }, {}, 0, WHITE);

		int row = 0;
		int column = 0;
		for (int i = 0; i < list.size(); i++)
		{
			Vector2 namePos = { 60 + (column * 460), 750 + (floor(row) * 60) };
			bigFont.Draw(list[i]->name, namePos, { 2, 2 }, WHITE);
			if (column == 0) column++;
			else { column = 0; row++; }
		}

		if (list.size() != 0) bigFont.Draw(list[actionHighlighted]->description, { 1000, 724 + 42 }, 2, 0, 64, Color(128, 128, 128, 255));
	}

	void DrawEnemySelect()
	{
		DrawTexturePro(menuHeart, { 0, 0, 16, 16 }, { menuHeartPos.x, menuHeartPos.y, 32, 32 }, {}, 0, WHITE);

		Rectangle source = { 0, 0, 362, 30 };
		Rectangle dest = { 840, 730 + 5, 362, 30 };
		DrawTexturePro(hpMercy, source, dest, {}, 0, WHITE);

		for (int i = 0; i < encounter->enemies.size(); i++)
		{
			Vector2 namePos = { 160, 750 + (i * 60) };
			bigFont.Draw(encounter->enemies.at(i).name, namePos, 2, 0, 56, WHITE);

			int offset = 10;
			int hpPercent = round(float(encounter->enemies.at(i).hp) / float(encounter->enemies.at(i).maxHp) * 100);
			Rectangle hpRectRed = { 840, 760 + (i * 60) + offset, 162, 32 };
			Rectangle hpRectGreen = { 840, 760 + (i * 60) + offset, 162 * (float)hpPercent / 100, 32 };
			Rectangle mercyRect = { 1040, 760 + (i * 60) + offset, 162, 32 };
			DrawRectangleRec(hpRectRed, { 255, 0, 0, 255 });
			DrawRectangleRec(hpRectGreen, { 0, 255, 0, 255 });
			DrawRectangleRec(mercyRect, { 255, 80, 32, 255 });

			std::string hpString = std::to_string(hpPercent); hpString.append("%");
			std::string mercyString = std::to_string(encounter->enemies.at(i).mercy); mercyString.append("%");
			bigFont.Draw(hpString, { hpRectGreen.x + 8, hpRectGreen.y }, { 2, 1 }, WHITE);
			bigFont.Draw(mercyString, { mercyRect.x + 8, mercyRect.y }, { 2, 1 }, { 128, 0, 0, 255 });
		}
	}

	void DrawPartySelect()
	{
		DrawTexturePro(menuHeart, { 0, 0, 16, 16 }, { menuHeartPos.x, menuHeartPos.y, 32, 32 }, {}, 0, WHITE);

		for (int i = 0; i < 3; i++)
		{
			Vector2 namePos = { 160, 750 + (i * 60) };
			bigFont.Draw(encounter->heroes[i]->name, namePos, 2, 0, 56, WHITE);

			int offset = 10;
			int hpPercent = round(float(encounter->heroes[i]->hp) / float(encounter->heroes[i]->maxHp) * 100);
			Rectangle hpRectRed = { 840, 760 + (i * 60) + offset, 162, 32 };
			Rectangle hpRectGreen = { 840, 760 + (i * 60) + offset, 162 * (float)hpPercent / 100, 32 };
			DrawRectangleRec(hpRectRed, { 255, 0, 0, 255 });
			DrawRectangleRec(hpRectGreen, { 0, 255, 0, 255 });
		}
	}

	void DrawBottomMenu()
	{
		switch (menuState)
		{
		case strategy:
			bigFont.Draw(encounter->description, { 60 + 2, 750 + 2 }, 2, 2, 56, Color(36, 33, 78, 255));
			bigFont.Draw(encounter->description, { 60, 750 }, 2, 2, 56, WHITE);
			break;
		case fight1:
			DrawEnemySelect();
			break;
		case act1:
			DrawEnemySelect();
			break;
		case act2:
			DrawActionSelect(hero->acts);
			break;
		case item1:
			DrawActionSelect(encounter->items);
			break;
		case item2:
			DrawPartySelect();
			break;
		case spare1:
			DrawEnemySelect();
			break;
		}
	}

	void AddAct(Action* act)
	{
		hero->acts.push_back(act);
	}

	void AddItem(Action* item)
	{
		encounter->items.push_back(item);
	}
};*/

class PlayerMenu
{
	public:

	enum MenuState
	{
		strategy,
		fight1,
		act1,
		act2,
		item1,
		item2,
		spare1
	};
	enum MenuStrategy
	{
		fight,
		act,
		item,
		spare,
		defend
	};

	//---- References ----//

	Texture fightMenuTex = Texture();
	Texture charaIcons = Texture();
	Texture menuHeart = Texture();
	Texture hpMercy = Texture();
	Sound menuMove = Sound();
	Sound menuSelect = Sound();

	//---- Properties ----//

	float moveUpSpeed = 0.1;

	//---- Values ----//

	Encounter* encounter;
	Hero* hero;

	Vector2 charaCoords;
	Color charaColor;

	int* turn;
	int lastTurn = 0;

	int actionHighlighted = -1;

	MenuState menuState = strategy;
	MenuStrategy menuStrategy = fight;
	float menuStart = 654, menuTarget = 586, menuTimer = 0;

	std::vector<float> lines = {};
	double lastLine = 0;

	int enemyHighlighted = -1;

	Vector2 menuHeartPos = {};

	PlayerMenu(Encounter* _encounter, int* _turn, int whichHero)
	{
		encounter = _encounter;
		turn = _turn;
		hero = encounter->heroes[whichHero];

		switch (hero->chara)
		{
		case 0:
			charaCoords = { 0, 0 };
			charaColor = krisBlue;
			break;
		case 1:
			charaCoords = { 0, 38 };
			charaColor = susiePink;
			break;
		case 2:
			charaCoords = { 0, 76 };
			charaColor = ralseiGreen;
			break;
		case 3:
			charaCoords = { 213, 0 };
			charaColor = berdlyColor;
			break;
		default:
			charaCoords = { 0, 0 };
			charaColor = krisBlue;
		}
	}

	void Initialize()
	{
		fightMenuTex = LoadTexture("resources/fight_menu.png");
		charaIcons = LoadTexture("resources/chara_icons_no.png");
		menuHeart = LoadTexture("resources/spr_heart.png");
		hpMercy = LoadTexture("resources/hp_mercy.png");
		menuMove = LoadSound("resources/snd_menumove.wav");
		menuSelect = LoadSound("resources/snd_select.wav");

		SetSoundVolume(menuMove, 0.5);
		SetSoundVolume(menuSelect, 0.5);

		if (*turn != hero->place)
		{
			menuStart = 586;
			menuTarget = 654;
			menuTimer = moveUpSpeed;
		}
	}

	void Reset()
	{
		menuTimer = 0;
		hero->strategy = -1;
		hero->choice = -1;
		hero->target = -1;
		menuState = strategy;
		menuStrategy = fight;
	}

	void Update()
	{
		switch (menuState)
		{
		case strategy:
			StrategyState();
			break;
		case fight1:
			FightState();
			break;
		case act1:
			Act1State();
			break;
		case act2:
			Act2State();
			break;
		case item1:
			Item1State();
			break;
		case item2:
			Item2State();
			break;
		case spare1:
			SpareState();
			break;
		}
	}

	void SubmitTurn()
	{
		PlaySound(menuSelect);
		hero->strategy = int(menuStrategy);
		Print("bogey spotted@");
		Print("har: " + std::to_string(hero->strategy.value));
		hero->choice = actionHighlighted;
		Print("haha: " + std::to_string(hero->choice.value));
		hero->target = enemyHighlighted;
		Print("HAGHAGHGA: " + std::to_string(hero->target.value));
		*turn = hero->place + 1;
		menuTimer = 0;
	}

	void UnsubmitTurn()
	{
		if (*turn != 0)
		{
			PlaySound(menuMove);
			hero->strategy = -1;
			*turn = hero->place - 1;
			menuTimer = 0;
		}
	}

	void EnterEnemySelect()
	{
		enemyHighlighted = 0;
		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };
	}

	bool EnemySelect()
	{
		if (IsKeyPressed(KEY_DOWN) && enemyHighlighted + 1 < encounter->enemies.size())
		{
			enemyHighlighted++;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_UP) && enemyHighlighted >= 1)
		{
			enemyHighlighted--;
			PlaySound(menuMove);
		}
		if (Z())
		{
			PlaySound(menuSelect);
			return true;
		}

		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };

		return false;
	}

	bool PartySelect()
	{
		if (IsKeyPressed(KEY_DOWN) && enemyHighlighted < encounter->heroes.size() - 1)
		{
			enemyHighlighted++;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_UP) && enemyHighlighted >= 1)
		{
			enemyHighlighted--;
			PlaySound(menuMove);
		}
		if (Z())
		{
			PlaySound(menuSelect);
			return true;
		}

		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };

		return false;
	}

	void EnterActionSelect()
	{
		actionHighlighted = 0;
		menuHeartPos = { 110, 770 };
	}

	bool ActionSelect(std::vector<Action*> list)
	{
		if (IsKeyPressed(KEY_DOWN) && actionHighlighted + 2 < list.size())
		{
			actionHighlighted += 2;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_UP) && actionHighlighted >= 2)
		{
			actionHighlighted -= 2;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_LEFT) && actionHighlighted % 2 == 1)
		{
			actionHighlighted--;
			PlaySound(menuMove);
		}
		if (IsKeyPressed(KEY_RIGHT) && actionHighlighted % 2 == 0 && actionHighlighted + 1 < list.size())
		{
			actionHighlighted++;
			PlaySound(menuMove);
		}
		if (Z())
		{
			PlaySound(menuSelect);
			return true;
		}

		menuHeartPos = { 110, 770 + float(enemyHighlighted * 60) };

		return false;
	}

	void StrategyState()
	{
		if (IsKeyPressed(KEY_RIGHT))
		{
			PlaySound(menuMove);
			if (menuStrategy < 4) menuStrategy = MenuStrategy(menuStrategy + 1);
			else menuStrategy = MenuStrategy(0);
		}
		else if (IsKeyPressed(KEY_LEFT))
		{
			PlaySound(menuMove);
			if (menuStrategy > 0) menuStrategy = MenuStrategy(menuStrategy - 1);
			else menuStrategy = MenuStrategy(4);
		}
		else if (Z())
		{
			PlaySound(menuSelect);
			switch (menuStrategy)
			{
			case fight:
				menuState = fight1;
				EnterEnemySelect();
				break;
			case act:
				menuState = act1;
				EnterEnemySelect();
				break;
			case item:
				menuState = item1;
				EnterActionSelect();
				break;
			case spare:
				menuState = spare1;
				EnterEnemySelect();
				break;
			case defend:
				SubmitTurn();
				break;
			}
		}
		else if (X())
		{
			UnsubmitTurn();
		}
	}

	void FightState()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (EnemySelect())
		{
			SubmitTurn();
		}
	}

	void Act1State()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (EnemySelect())
		{
			menuState = act2;
			EnterActionSelect();
			PlaySound(menuSelect);
		}
	}

	void Act2State()
	{
		if (X())
		{
			menuState = act1;
			PlaySound(menuMove);
			return;
		}
		if (ActionSelect(hero->acts))
		{
			SubmitTurn();
		}
	}

	void Item1State()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (ActionSelect(encounter->items))
		{
			menuState = item2;
			EnterEnemySelect();
			PlaySound(menuSelect);
		}
	}

	void Item2State()
	{
		if (X())
		{
			menuState = item1;
			PlaySound(menuMove);
			return;
		}
		if (PartySelect())
		{
			SubmitTurn();
		}
	}

	void SpareState()
	{
		if (X())
		{
			menuState = strategy;
			PlaySound(menuMove);
			return;
		}
		if (EnemySelect())
		{
			SubmitTurn();
		}
	}

	void Draw()
	{
		menuStart = *turn == hero->place ? 654 : 586;
		menuTarget = *turn == hero->place ? 586 : 654;
		DrawChara();
		if (*turn == hero->place) DrawBottomMenu();
	}

	void DrawChara()
	{
		Vector2 menuPos = hero->place == 0 ? Vector2(40, 666)
			: (hero->place == 1 ? Vector2(466, 666)
				: Vector2(892, 666));
		float borderStart = menuPos.x - 40;
		float borderEnd = borderStart + 422;
		Rectangle fightSource = { 0, float(menuStrategy == fight ? 32 : 0), 31, 32 };
		Rectangle actSource = { 31, float(menuStrategy == act ? 32 : 0), 31, 32 };
		Rectangle magicSource = { 31 * 2, float(menuStrategy == act ? 32 : 0), 31, 32 };
		Rectangle itemSource = { 31 * 3, float(menuStrategy == item ? 32 : 0), 31, 32 };
		Rectangle spareSource = { 31 * 4, float(menuStrategy == spare ? 32 : 0), 31, 32 };
		Rectangle defendSource = { 31 * 5, float(menuStrategy == defend ? 32 : 0), 31, 32 };
		Vector2 charaScale = *turn == hero->place ? Vector2(213, 38) : Vector2(209, 32);
		float menuHeight = Slerp(menuStart, menuTarget, moveUpSpeed, menuTimer);
		Vector2 charaDest = { borderStart, menuHeight };

		DrawRectangle(charaDest.x, charaDest.y, 213 * 2, 32 * 2, BLACK);

		Rectangle source = { charaCoords.x, charaCoords.y, charaScale.x, charaScale.y };
		if (*turn != hero->place) { source.x += 2; source.y += 2; charaDest.x += 4; charaDest.y += 4; }
		DrawTexturePro(charaIcons, source, { charaDest.x, charaDest.y, charaScale.x * 2, charaScale.y * 2 }, {}, 0, WHITE);

		Vector2 choiceIcon = { 150, 115 };
		switch (hero->strategy.value)
		{
		case 0:
			choiceIcon = { 0, 114 };
			break;
		case 1:
			choiceIcon = { 0, 149 };
			break;
		case 2:
			choiceIcon = { 100, 114 };
			break;
		case 3:
			choiceIcon = { 100, 149 };
			break;
		case 4:
			choiceIcon = { 50, 149 };
			break;
		}
		Rectangle sillyDest = { charaDest.x, charaDest.y, 98, 66 };
		DrawTexturePro(charaIcons, { choiceIcon.x, choiceIcon.y, 49, 33 }, sillyDest, {}, 0, charaColor);

		std::string hpString = std::to_string(hero->hp.value);
		hpFont.Draw(hpString, { borderStart + 320 - (hpString.size() * 16), menuHeight + 16 }, 4, 0, 56, WHITE);
		hpString = std::to_string(hero->maxHp);
		hpFont.Draw(hpString, { borderStart + 410 - (hpString.size() * 16), menuHeight + 16 }, 4, 0, 56, WHITE);

		if (*turn == hero->place)
		{
			float lineSpeed = 0.5;
			float lineInterval = 0.5;
			if (GetTime() - lastLine >= lineInterval)
			{
				lines.push_back(0);
				lastLine = GetTime();
			}
			for (int i = 0; i < lines.size(); i++)
			{
				Color color = charaColor;
				color.a = floor(255 * float((60 - lines[i]) / 60));
				DrawRectangle(borderStart + lines[i], 662, 4, 62, color);
				DrawRectangle(borderEnd - lines[i], 662, 4, 62, color);

				lines[i] += lineSpeed + (lines[i] / 60 * lineSpeed);
				if (lines[i] > 60) lines.erase(lines.begin() + i);
			}

			DrawRectangle(borderStart, 662, 4, 62, charaColor);
			DrawRectangle(borderEnd, 662, 4, 62, charaColor);

			DrawTexturePro(fightMenuTex, fightSource, { menuPos.x, menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, magicSource, { menuPos.x + (35 * 2), menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, itemSource, { menuPos.x + (35 * 4), menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, spareSource, { menuPos.x + (35 * 6), menuPos.y, 62, 64 }, {}, 0, WHITE);
			DrawTexturePro(fightMenuTex, defendSource, { menuPos.x + (35 * 8), menuPos.y, 62, 64 }, {}, 0, WHITE);
		}
	}

	void DrawActionSelect(std::vector<Action*> list)
	{
		menuHeartPos = { float(20 + ((actionHighlighted % 2) * 440)), float(766 + ((actionHighlighted - (actionHighlighted % 2)) / 2 * 60)) };
		DrawTexturePro(menuHeart, { 0, 0, 16, 16 }, { menuHeartPos.x, menuHeartPos.y, 32, 32 }, {}, 0, WHITE);

		int row = 0;
		int column = 0;
		for (int i = 0; i < list.size(); i++)
		{
			Vector2 namePos = { 60 + (column * 460), 750 + (floor(row) * 60) };
			bigFont.Draw(list[i]->name, namePos, { 2, 2 }, WHITE);
			if (column == 0) column++;
			else { column = 0; row++; }
		}

		if (list.size() != 0) bigFont.Draw(list[actionHighlighted]->description, { 1000, 724+42 }, 2, 0, 64, Color(128, 128, 128, 255));
	}

	void DrawEnemySelect()
	{
		DrawTexturePro(menuHeart, { 0, 0, 16, 16 }, { menuHeartPos.x, menuHeartPos.y, 32, 32 }, {}, 0, WHITE);

		Rectangle source = { 0, 0, 362, 30 };
		Rectangle dest = { 840, 730 + 5, 362, 30 };
		DrawTexturePro(hpMercy, source, dest, {}, 0, WHITE);

		for (int i = 0; i < encounter->enemies.size(); i++)
		{
			Vector2 namePos = { 160, 750 + (i * 60) };
			bigFont.Draw(encounter->enemies.at(i).name, namePos, 2, 0, 56, WHITE);

			int offset = 10;
			int hpPercent = round(float(encounter->enemies.at(i).hp) / float(encounter->enemies.at(i).maxHp) * 100);
			Rectangle hpRectRed = { 840, 760 + (i * 60) + offset, 162, 32 };
			Rectangle hpRectGreen = { 840, 760 + (i * 60) + offset, 162 * (float)hpPercent / 100, 32 };
			Rectangle mercyRect = { 1040, 760 + (i * 60) + offset, 162, 32 };
			DrawRectangleRec(hpRectRed, { 255, 0, 0, 255 });
			DrawRectangleRec(hpRectGreen, { 0, 255, 0, 255 });
			DrawRectangleRec(mercyRect, { 255, 80, 32, 255 });

			std::string hpString = std::to_string(hpPercent); hpString.append("%");
			std::string mercyString = std::to_string(encounter->enemies.at(i).mercy); mercyString.append("%");
			bigFont.Draw(hpString, { hpRectGreen.x + 8, hpRectGreen.y }, { 2, 1 }, WHITE);
			bigFont.Draw(mercyString, { mercyRect.x + 8, mercyRect.y }, { 2, 1 }, { 128, 0, 0, 255 });
		}
	}

	void DrawPartySelect()
	{
		DrawTexturePro(menuHeart, { 0, 0, 16, 16 }, { menuHeartPos.x, menuHeartPos.y, 32, 32 }, {}, 0, WHITE);

		for (int i = 0; i < 3; i++)
		{
			Vector2 namePos = { 160, 750 + (i * 60) };
			bigFont.Draw(encounter->heroes[i]->name, namePos, 2, 0, 56, WHITE);

			int offset = 10;
			int hpPercent = round(float(encounter->heroes[i]->hp.value) / float(encounter->heroes[i]->maxHp) * 100);
			Rectangle hpRectRed = { 840, 760 + (i * 60) + offset, 162, 32 };
			Rectangle hpRectGreen = { 840, 760 + (i * 60) + offset, 162 * (float)hpPercent / 100, 32 };
			DrawRectangleRec(hpRectRed, { 255, 0, 0, 255 });
			DrawRectangleRec(hpRectGreen, { 0, 255, 0, 255 });
		}
	}

	void DrawBottomMenu()
	{
		switch (menuState)
		{
		case strategy:
			bigFont.Draw(encounter->description, { 60 + 2, 750 + 2 }, 2, 2, 56, Color(36, 33, 78, 255));
			bigFont.Draw(encounter->description, { 60, 750 }, 2, 2, 56, WHITE);
			break;
		case fight1:
			DrawEnemySelect();
			break;
		case act1:
			DrawEnemySelect();
			break;
		case act2:
			DrawActionSelect(hero->acts);
			break;
		case item1:
			DrawActionSelect(encounter->items);
			break;
		case item2:
			DrawPartySelect();
			break;
		case spare1:
			DrawEnemySelect();
			break;
		}
	}

	void AddAct(Action* act)
	{
		hero->acts.push_back(act);
	}

	void AddItem(Action* item)
	{
		encounter->items.push_back(item);
	}
};