#include <raylib-cpp.hpp>
#include "font.hpp"

Color bigGreen = { 0, 255, 0, 255 };
Color smallGreen = { 0, 120, 0, 255 };
Color bigPurple = { 66, 0, 66, 255 };
Color smallPurple = { 36, 0, 36, 255 };
Color krisBlue = { 0, 255, 255, 255 };
Color susiePink = { 255, 0, 255, 255 };
Color ralseiGreen = { 0, 255, 0, 255 };
Color berdlyColor = { 0, 255, 180, 255 };

DeltaFont bigFont = DeltaFont({
	DeltaGlyph(' ', { 198, 122, 6, 32 }, 6),
	DeltaGlyph('!', { 188, 122, 8, 26 }, 12),
	DeltaGlyph('"', { 220, 122, 10, 16 }, 12),
	DeltaGlyph('#', { 104, 2, 14, 26 }, 16),
	DeltaGlyph('$', { 184, 2, 12, 30 }, 14),
	DeltaGlyph('%', { 88, 2, 14, 26 }, 16),
	DeltaGlyph('&', { 136, 2, 14, 26 }, 16),
	DeltaGlyph('\'', { 14, 156, 4, 16 }, 6),
	DeltaGlyph('(', { 178, 122, 8, 26 }, 10),
	DeltaGlyph(')', { 158, 122, 8, 26 }, 10),
	DeltaGlyph('*', { 212, 2, 16, 22 }, 18),
	DeltaGlyph('+', { 16, 122, 12, 22 }, 14),
	DeltaGlyph(',', { 238, 122, 4, 30 }, 6),
	DeltaGlyph('-', { 134, 122, 12, 28 }, 14),
	DeltaGlyph('.', { 2, 156, 4, 26 }, 6),
	DeltaGlyph('/', { 2, 36, 12, 28 }, 14),
	DeltaGlyph('0', { 114, 66, 12, 26 }, 14),
	DeltaGlyph('1', { 58, 66, 12, 26 }, 14),
	DeltaGlyph('2', { 240, 66, 12, 26 }, 14),
	DeltaGlyph('3', { 156, 36, 12, 26 }, 14),
	DeltaGlyph('4', { 100, 36, 12, 26 }, 14),
	DeltaGlyph('5', { 44, 66, 12, 26 }, 14),
	DeltaGlyph('6', { 170, 36, 12, 26 }, 14),
	DeltaGlyph('7', { 226, 36, 12, 26 }, 14),
	DeltaGlyph('8', { 2, 94, 12, 26 }, 14),
	DeltaGlyph('9', { 184, 66, 12, 26 }, 14),
	DeltaGlyph(':', { 244, 122, 4, 26 }, 6),
	DeltaGlyph(';', { 232, 122, 4, 30 }, 6),
	DeltaGlyph('<', { 66, 122, 10, 26 }, 12),
	DeltaGlyph('=', { 120, 122, 12, 20 }, 14),
	DeltaGlyph('>', { 30, 122, 10, 26 }, 12),
	DeltaGlyph('?', { 86, 94, 12, 26 }, 14),
	DeltaGlyph('@', { 72, 94, 12, 26 }, 14),
	DeltaGlyph('A', { 184, 36, 12, 26 }, 14),
	DeltaGlyph('B', { 128, 36, 12, 26 }, 14),
	DeltaGlyph('C', { 240, 36, 12, 26 }, 14),
	DeltaGlyph('D', { 100, 94, 12, 26 }, 14),
	DeltaGlyph('E', { 156, 94, 12, 26 }, 14),
	DeltaGlyph('F', { 142, 94, 12, 26 }, 14),
	DeltaGlyph('G', { 142, 36, 12, 26 }, 14),
	DeltaGlyph('H', { 142, 66, 12, 26 }, 14),
	DeltaGlyph('I', { 30, 36, 12, 26 }, 14),
	DeltaGlyph('J', { 72, 66, 12, 26 }, 14),
	DeltaGlyph('K', { 100, 66, 12, 26 }, 14),
	DeltaGlyph('L', { 184, 94, 12, 26 }, 14),
	DeltaGlyph('M', { 152, 2, 14, 26 }, 16),
	DeltaGlyph('N', { 72, 36, 12, 26 }, 14),
	DeltaGlyph('O', { 170, 66, 12, 26 }, 14),
	DeltaGlyph('P', { 198, 66, 12, 26 }, 14),
	DeltaGlyph('Q', { 198, 2, 12, 30 }, 14),
	DeltaGlyph('R', { 128, 66, 12, 26 }, 14),
	DeltaGlyph('S', { 170, 94, 12, 26 }, 14),
	DeltaGlyph('T', { 86, 66, 12, 26 }, 14),
	DeltaGlyph('U', { 212, 66, 12, 26 }, 14),
	DeltaGlyph('V', { 114, 94, 12, 26 }, 14),
	DeltaGlyph('W', { 72, 2, 14, 26 }, 16),
	DeltaGlyph('X', { 16, 36, 12, 26 }, 14),
	DeltaGlyph('Y', { 128, 94, 12, 26 }, 14),
	DeltaGlyph('Z', { 2, 122, 12, 26 }, 14),
	DeltaGlyph('[', { 168, 122, 8, 26 }, 10),
	DeltaGlyph('\\', { 230, 2, 12, 28 }, 14),
	DeltaGlyph(']', { 148, 122, 8, 26 }, 10),
	DeltaGlyph('^', { 206, 122, 12, 14 }, 14),
	DeltaGlyph('_', { 78, 122, 8, 32 }, 10),
	DeltaGlyph('`', { 20, 156, 6, 10 }, 8),
	DeltaGlyph('a', { 156, 66, 12, 26 }, 14),
	DeltaGlyph('b', { 58, 94, 12, 26 }, 14),
	DeltaGlyph('c', { 86, 36, 12, 26 }, 14),
	DeltaGlyph('d', { 212, 94, 12, 26 }, 14),
	DeltaGlyph('e', { 2, 66, 12, 26 }, 14),
	DeltaGlyph('f', { 16, 66, 12, 26 }, 14),
	DeltaGlyph('g', { 2, 2, 12, 32 }, 14),
	DeltaGlyph('h', { 58, 36, 12, 26 }, 14),
	DeltaGlyph('i', { 16, 94, 12, 26 }, 14),
	DeltaGlyph('j', { 30, 2, 12, 32 }, 14),
	DeltaGlyph('k', { 240, 94, 12, 26 }, 14),
	DeltaGlyph('l', { 226, 66, 12, 26 }, 14),
	DeltaGlyph('m', { 120, 2, 14, 26 }, 16),
	DeltaGlyph('n', { 198, 36, 12, 26 }, 14),
	DeltaGlyph('o', { 44, 36, 12, 26 }, 14),
	DeltaGlyph('p', { 58, 2, 12, 32 }, 14),
	DeltaGlyph('q', { 44, 2, 12, 32 }, 14),
	DeltaGlyph('r', { 226, 94, 12, 26 }, 14),
	DeltaGlyph('s', { 44, 94, 12, 26 }, 14),
	DeltaGlyph('t', { 212, 36, 12, 26 }, 14),
	DeltaGlyph('u', { 198, 94, 12, 26 }, 14),
	DeltaGlyph('v', { 30, 66, 12, 26 }, 14),
	DeltaGlyph('w', { 168, 2, 14, 26 }, 16),
	DeltaGlyph('x', { 114, 36, 12, 26 }, 14),
	DeltaGlyph('y', { 16, 2, 12, 32 }, 14),
	DeltaGlyph('z', { 30, 94, 12, 26 }, 14),
	DeltaGlyph('{', { 54, 122, 10, 26 }, 12),
	DeltaGlyph('|', { 8, 156, 4, 26 }, 6),
	DeltaGlyph('}', { 42, 122, 10, 26 }, 12),
	DeltaGlyph('~', { 104, 122, 14, 18 }, 16)
	});

DeltaFont hpFont = DeltaFont({
	DeltaGlyph('0', { 0, 0, 3, 5 }, 4),
	DeltaGlyph('1', { 4, 0, 3, 5 }, 4),
	DeltaGlyph('2', { 8, 0, 3, 5 }, 4),
	DeltaGlyph('3', { 12, 0, 3, 5 }, 4),
	DeltaGlyph('4', { 16, 0, 3, 5 }, 4),
	DeltaGlyph('5', { 0, 6, 3, 5 }, 4),
	DeltaGlyph('6', { 4, 6, 3, 5 }, 4),
	DeltaGlyph('7', { 8, 6, 3, 5 }, 4),
	DeltaGlyph('8', { 12, 6, 3, 5 }, 4),
	DeltaGlyph('9', { 16, 6, 3, 5 }, 4)
	});

void Print(std::string str)
{
	printf(str.c_str());
	printf("\n");
}
void Print(int n)
{
	printf(std::to_string(n).c_str());
	printf("\n");
}
void Print(float n)
{
	printf(std::to_string(n).c_str());
	printf("\n");
}
void Print(double n)
{
	printf(std::to_string(n).c_str());
	printf("\n");
}
void Print(size_t n)
{
	printf(std::to_string(n).c_str());
	printf("\n");
}

void DrawRectOutline(Rectangle rect, float thickness, Color color)
{
	/*DrawRectangle(rect.x, rect.y, rect.width - thickness, thickness, color);
	DrawRectangle(rect.x, rect.y + thickness, thickness, rect.height - thickness, color);
	DrawRectangle(rect.x + thickness, rect.y + rect.height - thickness, rect.width - thickness, thickness, color);
	DrawRectangle(rect.x + rect.width - thickness, rect.y, thickness, rect.height - thickness, color);*/

	DrawRectangle(rect.x, rect.y, rect.width, thickness, color);
	DrawRectangle(rect.x, rect.y + rect.height - thickness, rect.width, thickness, color);
	DrawRectangle(rect.x, rect.y, thickness, rect.height, color);
	DrawRectangle(rect.x + rect.width - thickness, rect.y, thickness, rect.height, color);
}

float Slerp(float start, float end, float seconds, float& timer)
{
	float multiplier = sin(acos(1 - (timer / seconds)));
	float final = start + (multiplier * (end - start));
	timer += GetFrameTime();
	if (timer >= seconds)
	{
		timer = seconds;
		return end;
	}
	else return final;
}

bool Z()
{
	return (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER));
}
bool X()
{
	return (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_RIGHT_SHIFT));
}