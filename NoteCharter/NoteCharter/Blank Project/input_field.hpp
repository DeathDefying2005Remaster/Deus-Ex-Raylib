#pragma once
#include <raylib-cpp.hpp>
#include "global.hpp"

class InputField
{
	private:

	Vector2 centrePosition;
	Vector2 size;
	float min;
	float max;

	public:

	bool active;
	std::string text;
	float& value;

	InputField(float& _value, Vector2 _centrePosition, Vector2 _size, float _min, float _max) : value(_value)
	{
		centrePosition = _centrePosition;
		size = _size;
		min = _min;
		max = _max;
		active = false;

		text = std::to_string(value);
		if (text.find('.') != std::string::npos)
		{
			while (text[text.size() - 1] == '0')
			{
				text.erase(text.length() - 1);
			}
			if (text[text.size() - 1] == '.') text.erase(text.length() - 1);
		}
	}

	void Click()
	{
		if (GetMousePosition().x > centrePosition.x - size.x && GetMousePosition().x < centrePosition.x + size.x)
		{
			if (GetMousePosition().y > centrePosition.y - size.y && GetMousePosition().y < centrePosition.y + size.y)
			{
				active = true;
				return;
			}
		}

		if (active)
		{
			SetValue();
			active = false;
		}
		return;
	}

	void TypeInput()
	{
		if (active)
		{
			int key = GetKeyPressed();
			if (key - 48 >= 0 && key - 48 <= 9 && text.length() < 15) { text += std::to_string(key - 48); }
			else if (key == 46 && text.length() > 0 && text.find('.') == std::string::npos && text.length() < 7) { text += '.'; }
			else if (key == KEY_MINUS && text.find('-') == std::string::npos && text.length() == 0) { text += '-'; }
			else if (key == 259 && text.length() > 0) { text.erase(text.length() - 1); }
			else if (key == 257) 
			{ 
				SetValue();
				return; 
			}
			/*else
			{
				key = GetCharPressed();
				if (key != 0) text += char(key);
			}*/

			//if (IsKeyPressed(KEY_V) && IsKeyDown(KEY_LEFT_CONTROL))
			//{
			//	text += GetClipboardText();
			//	for (int i = 0; i < text.size(); i++)
			//	{
			//		//if (text[i] == )
			//	}
			//}
		}
	}

	void SetValue()
	{
		active = false;
		if (text != "") value = Clamp(stof(text), min, max);

		std::string newText = std::to_string(value);
		if (newText.find('.') != std::string::npos)
		{
			while (newText[newText.size() - 1] == '0')
			{
				newText.erase(newText.length() - 1);
			}
			if (newText[newText.size() - 1] == '.') newText.erase(newText.length() - 1);
		}
		text = newText;
	}

	void Draw()
	{
		Vector2 cornerPos = Vector2Subtract(centrePosition, size);
		DrawRectangleV(cornerPos, Vector2Scale(size, 2), { 0, 0, 0, 100 });
		DrawRectOutline({ cornerPos.x, cornerPos.y, size.x * 2, size.y * 2 }, 2, active ? bigGreen : smallGreen);

		float fontSize = (size.y * 0.8) * 0.8;
		DrawText(text.c_str(), cornerPos.x + (size.x * 0.5), cornerPos.y + size.y - fontSize/2, fontSize, active ? bigGreen : smallGreen);
	}
};