#include <raylib-cpp.hpp>
using namespace std;

class InputField
{
	private:

	Vector2 centrePosition;
	Vector2 size;
	Color backColourInactive;
	Color backColourActive;
	Color textColour;

	public:

	bool active;
	string text;

	InputField(Vector2 _centrePosition, Vector2 _size, Color _backColourInactive, Color _backColourActive, Color _textColour)
	{
		centrePosition = _centrePosition;
		size = _size;
		backColourInactive = _backColourInactive;
		backColourActive = _backColourActive;
		textColour = _textColour;
		active = false;
	}

	void Click()
	{
		active = false;
		if (GetMousePosition().x > centrePosition.x - size.x && GetMousePosition().x < centrePosition.x + size.x)
		{
			if (GetMousePosition().y > centrePosition.y - size.y && GetMousePosition().y < centrePosition.y + size.y)
			{
				active = true;
			}
		}
	}

	void TypeInput()
	{
		if (active)
		{
			int key = GetKeyPressed();
			if (key - 48 >= 0 && key - 48 <= 9 && text.length() < 15) { text += to_string(key - 48); }
			else if (key == 46 && text.length() > 0 && text.at(text.length() - 1) != '.' && text.length() < 13) { text += '.'; }
			else if (key == 259 && text.length() > 0) { text.erase(text.length() - 1); }
		}
	}

	void Draw()
	{
		Vector2 cornerPos = Vector2Subtract(centrePosition, size);
		DrawRectangleV(cornerPos, Vector2Scale(size, 2), active ? backColourActive : backColourInactive);
		DrawText(text.c_str(), cornerPos.x + (size.x * 0.1), cornerPos.y + (size.y * 0.1), (size.y - 0.2) * 2, textColour);
	}
};