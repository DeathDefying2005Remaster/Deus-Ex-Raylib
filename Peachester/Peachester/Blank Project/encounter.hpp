#pragma once
#include <raylib-cpp.hpp>
#include "netcode.hpp"

struct Hero;

struct Enemy
{
	int hp;
	int maxHp;
	int mercy;
	std::string name;

	Enemy(int _hp, int _maxHp, int _mercy, std::string _name)
	{
		hp = _hp;
		maxHp = _maxHp;
		mercy = _mercy;
		name = _name;
	}
};

class Character
{
	int chara;

	Character(int _chara)
	{
		chara = _chara;
	}

	Rectangle GetMenuSprite()
	{
		switch (chara)
		{
		case 0:
			return { 0, 0,  };
		}
	}
};

class Action
{
	public:

	std::string name;
	std::string description;

	virtual bool Available() { return true; }
	virtual void Use(Hero* user, Hero* reciever) {}
};

//struct Hero
//{
//	std::string name;
//	int hp;
//	int maxHp;
//	int chara;
//	int place;
//	int strategy = -1;
//	int choice = -1;
//	int target = -1;
//	std::vector<Action*> acts;
//
//	Hero(std::string _name, int _maxHp, int _chara, int _place, std::vector<Action*> _acts)
//	{
//		name = _name;
//		hp = _maxHp;
//		maxHp = _maxHp;
//		chara = _chara;
//		place = _place;
//		acts = _acts;
//	}
//};

struct Hero
{
	std::string name;
	NetworkVariable<int> hp;
	int maxHp;
	int chara;
	int place;
	NetworkVariable<int> strategy;
	NetworkVariable<int> choice;
	NetworkVariable<int> target;
	std::vector<Action*> acts;
	
	Hero(std::string _name, int _maxHp, int _chara, int _place, std::vector<Action*> _acts)
	{
		strategy = -1;
		choice = -1;
		target = -1;
		name = _name;
		hp = _maxHp;
		maxHp = _maxHp;
		chara = _chara;
		place = _place;
		acts = _acts;
	}
};

struct Encounter
{
	public:

	std::vector<Enemy> enemies;
	std::vector<Hero*> heroes;
	std::vector<Action*> items;
	std::string description;

	Encounter(std::vector<Enemy> _enemies, std::vector<Hero*> _heroes, std::string _description)
	{
		enemies = _enemies;
		heroes = _heroes;
		description = _description;
	}
};