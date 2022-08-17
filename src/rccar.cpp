#include "RCCar.h"
#include <iostream>
#include <cmath>
#define PI 3.1415926535

RCCar::RCCar(Room* room, Vec2 start_pos, Vec2 start_dir) : room(room), pos(start_pos), dir(start_dir)
{
	if (!room)
	{
		std::cerr << "Invalid room assigned to RCCar\n";
		return;
	}

	if (room->GetRoomWidth() < pos.x || room->GetRoomHeight() < pos.y || pos.x < 0 || pos.y < 0)
	{
		std::cerr << "Position assigned to RCCar (" << pos.x << ", " << pos.y << ") is outside of the room size ("
			<< room->GetRoomWidth() << ", " << room->GetRoomHeight() << ")\n";
		return;
	}

	if (dir.x < 0) dir.x = -1;
	if (dir.y < 0) dir.y = -1;
	if (dir.x > 0) dir.x =  1;
	if (dir.y > 0) dir.y =  1;

	if (dir.x != 0 && dir.y != 0)
	{
		std::cerr << "Bad facing direction for RCCar. Can't be facing diagonally\n";
		PrintPosition();
		return;
	}

	valid = true;
}

bool RCCar::IsValid()
{
	return valid;
}

bool RCCar::PerformCommand(RCCommand command)
{

	switch (command)
	{
	case RCCommand::FORWARD:
		if (verbose_commands) std::cout << "Command: FORWARD\n";
		return Move(false);
		break;

	case RCCommand::BACK:
		if (verbose_commands) std::cout << "Command: BACK\n";
		return Move(true);
		break;

	case RCCommand::LEFT:
		if (verbose_commands) std::cout << "Command: LEFT\n";
		Rotate(90);
		break;

	case RCCommand::RIGHT:
		if (verbose_commands) std::cout << "Command: RIGHT\n";
		Rotate(-90);
		break;
	}

	return true;
}

void RCCar::PrintPosition()
{
	std::cout << "RCCar position (" << pos.x << ", " << pos.y << "), direction (" << dir.x << ", " << dir.y << ")\n";
}

bool RCCar::Move(bool Reversed)
{
	const int MOVE_FORWARD = 1 * !Reversed - 1 * Reversed;
	pos.x += dir.x * MOVE_FORWARD;
	pos.y += dir.y * MOVE_FORWARD;

	if (verbose_commands)
	{
		std::cout << "New position (" << pos.x << ", " << pos.y << ")\n";
	}

	return room->InsideBounds(pos);
}

void RCCar::Rotate(int degrees)
{
	const int COS = static_cast<int>(cosf(PI / 180.0f * static_cast<float>(degrees)));
	const int SIN = static_cast<int>(sinf(PI / 180.0f * static_cast<float>(degrees)));

	const int X = dir.x;
	const int Y = dir.y;

	dir.x = X * COS - Y * SIN;
	dir.y = X * SIN + Y * COS;

	if (verbose_commands)
	{
		std::cout << "New direction (" << dir.x << ", " << dir.y << ")\n";
	}
}
