#pragma once
#include "vec2.h"

class Room
{
	unsigned int width;
	unsigned int height;
public:
	Room() : width(1), height(1) {};
	Room(unsigned int width, unsigned int height) : width(width), height(height) {};

	inline bool InsideBounds(Vec2 pos)
	{
		return pos.x >= 0 && pos.x < width&& pos.y >= 0 && pos.y < height;
	}

	inline unsigned int GetRoomWidth() { return width; };
	inline unsigned int GetRoomHeight() { return height; };
};

