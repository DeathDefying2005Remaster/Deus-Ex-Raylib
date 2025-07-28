#include <raylib-cpp.hpp>

Color bigGreen = { 0, 255, 0, 255 };
Color smallGreen = { 0, 120, 0, 255 };
Color bigPurple = { 66, 0, 66, 255 };
Color smallPurple = { 36, 0, 36, 255 };
Color krisBlue = { 36, 232, 237, 255 };
Color susiePink = { 219, 124, 187, 255 };
Color ralseiGreen = { 183, 229, 34, 255 };

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