#pragma once
#include "room.h"
#include "rccommands.h"
#include "vec2.h"

class RCCar
{
	Vec2 pos;
	Vec2 dir;

	Room* room;

	bool verbose_commands = false;
	bool valid = false;

public:
	RCCar() = delete;
	RCCar(Room* room, Vec2 start_pos, Vec2 start_dir);

	bool IsValid();

	bool PerformCommand(RCCommand command);

	inline void SetVerboseCommands(bool verbose) { verbose_commands = verbose; };
	void PrintPosition();

private:
	bool Move(bool Reversed);
	void Rotate(int degrees);
};