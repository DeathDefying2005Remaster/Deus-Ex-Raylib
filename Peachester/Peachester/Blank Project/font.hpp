#pragma once
#include <raylib-cpp.hpp>
#include <unordered_map>

struct DeltaGlyph
{
	char ch;
	Rectangle source;
	int shift;

	DeltaGlyph()
	{
		ch = 0;
		source = {};
		shift = 0;
	}
	DeltaGlyph(char _ch, Rectangle _source, int _shift)
	{
		ch = _ch;
		source = _source;
		shift = _shift;
	}
};

class DeltaFont
{
	public:

	Texture texture;
	std::unordered_map<char, DeltaGlyph> glyphs;

	DeltaFont(std::vector<DeltaGlyph> _glyphs)
	{
		texture = Texture();
		for (int i = 0; i < _glyphs.size(); i++)
		{
			glyphs[_glyphs[i].ch] = _glyphs[i];
		}
	}

	void InitializeFont(Texture _texture)
	{
		texture = _texture;
	}

	void Draw(std::string text, Vector2 dest, Vector2 size, Color color)
	{
		int shift = 0;
		for (int i = 0; i < text.size(); i++)
		{
			if (glyphs.contains(text[i]))
			{
				DeltaGlyph glyph = glyphs[text[i]];
				Rectangle destination = { dest.x + (shift * size.x), dest.y, glyph.source.width * size.x, glyph.source.height * size.y };
				DrawTexturePro(texture, glyph.source, destination, {}, 0, color);
				shift += glyph.shift;
			}
		}
	}
	void Draw(std::string text, Vector2 dest, float size, int extraShift, int lineWidth, Color color)
	{
		int shift = 0;
		int newLine = 0;
		for (int i = 0; i < text.size(); i++)
		{
			if (text[i] == '\n') { newLine += lineWidth; shift = 0; }
			else if (glyphs.contains(text[i]))
			{
				DeltaGlyph glyph = glyphs[text[i]];
				Rectangle destination = { dest.x + (shift * size), dest.y + newLine, glyph.source.width * size, glyph.source.height * size };
				DrawTexturePro(texture, glyph.source, destination, {}, 0, color);
				shift += glyph.shift + extraShift;
			}
		}
	}
	void DrawWrap(std::string text, Vector2 dest, float size, int lineGap, int wrapWidth, Color color) //UNFHINISHED
	{
		int lineLength = 0;
		int i = 0;
		int wordLength = 0;
		while (text[i] != ' ' && i != text.size() - 1)
		{
			wordLength += glyphs[text[i]].shift * size;
			i++;
		}

		int shift = 0;
		for (int j = 0; j < text.size(); j++)
		{
			if (glyphs.contains(text[j]))
			{
				DeltaGlyph glyph = glyphs[text[j]];
				Rectangle destination = { dest.x + (shift * size), dest.y, glyph.source.width * size, glyph.source.height * size };
				DrawTexturePro(texture, glyph.source, destination, {}, 0, color);
				shift += glyph.shift;
			}
		}
	}
};