#pragma once
#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#undef PlaySound
#endif

#include <raylib-cpp.hpp>

class PlayerData
{
	public:

	enet_uint32 connectId;
	Vector2 position;

	PlayerData(enet_uint32 id, Vector2 pos)
	{
		connectId = id;
		position = pos;
	}
};