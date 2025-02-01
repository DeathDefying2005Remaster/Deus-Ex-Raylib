#include <raylib-cpp.hpp>
#include <map>
#include "machine.hpp"
using namespace std;

Image image = LoadImage("resources/veeonetwo.png");
int spriteWidth = 96;
int spriteHeight = 64;
float scaleFactor = 1;
vector<Vector2>  dirToIndex = { { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };

raylib::Rectangle GetSprite(Machine* machine)
{
	int row;
	int column;

	if (!Vector2Equals(machine->shootDir, {}))
	{
		if (machine->state == machine->sliding && machine->left) { row = 0; }
		else if (machine->state == machine->sliding && !machine->left) { row = 1; }
		else if (machine->state != machine->sliding) { row = 2; }

		for (int i = 0; i < dirToIndex.size(); i++)
		{
			if (Vector2Equals(machine->shootDir, dirToIndex[i]))
			{
				column = i;
				break;
			}
		}
	}
	else
	{
		if (machine->state == machine->sliding)
		{
			row = machine->left ? 0 : 1;
			column = 0;
		}
		else if (machine->state == machine->running)
		{
			row = 4;
			if (machine->left && machine->runTimer < machine->runInterval / 2) { column = 0; }
			else if (machine->left && machine->runTimer >= machine->runInterval / 2) { column = 1; }
			else if (!machine->left && machine->runTimer < machine->runInterval / 2) { column = 2; }
			else if (!machine->left && machine->runTimer >= machine->runInterval / 2) { column = 3; }
		}
		else if (machine->state != machine->running)
		{
			row = 3;
			if (machine->runTimer < machine->runInterval / 2) { column = machine->left ? 0 : 2; }
			else if (machine->runTimer >= machine->runInterval / 2) { column = machine-> left ? 1 : 3; }
			else if (!machine->left) { column = 2; }
		}
	}

	return raylib::Rectangle(column * spriteWidth, row * spriteHeight, spriteWidth, spriteHeight);
}

void SetMachineRect(Machine* machine)
{
	machine->rect = { machine->position.x - (float)(spriteWidth * scaleFactor * 0.5), machine->position.y - (float)(spriteHeight * scaleFactor * 0.5), scaleFactor * spriteWidth, scaleFactor * spriteHeight };
}

void DrawMachine(Machine* machine)
{
	Rectangle sourceRec = GetSprite(machine);
	SetMachineRect(machine);
	Vector2 origin = { 0, 0 };
	int rotation = 0;

	Texture2D sheet = LoadTextureFromImage(image);
	DrawTexturePro(sheet, sourceRec, machine->rect, origin, (float)rotation, WHITE);
}