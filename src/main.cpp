
#include <vector>
#include <iostream>
#include <string>
#include "rccommands.h"
#include "rccar.h"
#include "room.h"

#define ENABLE_DEBUG_CALL
#ifdef ENABLE_DEBUG_CALL
#define DEBUG_CALL(x) x
#else
#define DEBUG_CALL(x)
#endif

void TestAllCommands(RCCar car)
{
	std::cout << "Testing all commands with verbose logging on.\n\n";
	const std::vector<RCCommand> commands{ RCCommand::FORWARD, RCCommand::BACK, RCCommand::LEFT, RCCommand::RIGHT };

	car.SetVerboseCommands(true);
	for (auto command : commands)
	{
		car.PrintPosition();

		if (!car.PerformCommand(command))
		{
			std::cout << "unsuccessful, rccar hit wall! ";
			car.PrintPosition();

			return;
		}
	}

	std::cout << "Successful! ";
	car.PrintPosition();
}

void TokenizeLine(std::string line, const char separator, std::vector<std::string>& out_tokens)
{
	size_t start = 0;
	size_t end = 0;
	while ((end = line.find(separator, start)) != std::string::npos)
	{
		out_tokens.push_back(line.substr(start, end - start));
		start = end + 1;
	}
	if (start < line.length())
	{
		std::string token = line.substr(start, line.length());
		out_tokens.push_back(token);
	}
}

bool ParseQuitCondition(std::vector<std::string>& tokens)
{
	for (auto token : tokens)
	{
		if (token.compare("Q") == 0 || token.compare("q") == 0)
		{
			return true;
		}
	}
	return false;
}

bool ParsePositiveIntegers(std::vector<std::string>& tokens, std::vector<int>& out_integers, const int expected_count)
{
	int i = 0;
	for (auto token : tokens)
	{
		if (out_integers.size() >= expected_count)
		{
			break;
		}

		if (!std::isdigit(token[0]))
		{
			std::cerr << "Input error: Input must be integers.\n";
			return false;
		}
		DEBUG_CALL(std::cout << "Parsed Input: [" << token << "]\n");

		out_integers.push_back(std::stoi(token));
	}

	if (out_integers.size() != expected_count)
	{
		std::cerr << "Input error: Expected " << expected_count << " integers, received " << out_integers.size() << "\n";
		return false;
	}

	return true;
}

bool ParseRoomSize(int& width, int& height, const char token_separator)
{
	while (width <= 0 || height <= 0)
	{
		std::cout << "Enter room width and height separated with a space. Eg. 4 8. Enter Q to quit.\n";

		std::string line;
		std::vector<std::string> tokens;
		std::vector<int> integers;

		std::getline(std::cin, line);
		TokenizeLine(line, token_separator, tokens);

		if (ParseQuitCondition(tokens))
		{
			return false;
		}

		if (ParsePositiveIntegers(tokens, integers, 2))
		{
			unsigned int token_index = 0;
			width  = token_index < integers.size() ? integers[token_index++] : -1;
			height = token_index < integers.size() ? integers[token_index++] : -1;

			DEBUG_CALL(std::cout << "Width: " << width << ", Height: " << height << "\n");
		}
	}

	return true;
}

bool ParseDirection(Vec2& dir, std::string token)
{
	if (token.length() != 1)
	{
		std::cerr << "No direction specified.\n";
		return false;
	}

	switch (token[0])
	{
	case 'N':
		dir = { 0, 1 };
		break;
	case 'S':
		dir = { 0, -1 };
		break;
	case 'W':
		dir = { -1, 0 };
		break;
	case 'E':
		dir = { 1, 0 };
		break;
	default:
		std::cerr << "Bad direction specified. [" << token << "]\n";
		return false;
	}

	DEBUG_CALL(std::cout << "Dir X: " << dir.x << ", Y: " << dir.y << "\n");
	return true;
}

bool ParseStartingLocation(Vec2& start_pos, Vec2& start_dir, const char token_separator)
{
	int start_x = -1;
	int start_y = -1;
	bool valid_dir = false;
	while (start_x <= -1 || start_y <= -1 || !valid_dir)
	{
		std::cout << "Enter starting location x and y followed by a direction (N, S, W, E) separated with a space. Eg. 2 3 E. Enter Q to quit.\n";

		std::string line;
		std::vector<std::string> tokens;
		std::vector<int> integers;

		std::getline(std::cin, line);
		TokenizeLine(line, token_separator, tokens);

		if (ParseQuitCondition(tokens))
		{
			return false;
		}

		unsigned int token_index = 0;
		if (ParsePositiveIntegers(tokens, integers, 2))
		{
			start_x = token_index < integers.size() ? integers[token_index++] : -1;
			start_y = token_index < integers.size() ? integers[token_index++] : -1;

			DEBUG_CALL(std::cout << "X: " << start_x << ", Y: " << start_y << "\n");

			std::string direction_token = token_index < tokens.size() ? tokens[token_index++] : "";
			valid_dir = ParseDirection(start_dir, direction_token);
		}
	}
	start_pos = { start_x, start_y };

	return true;
}


bool ParseSingleCommand(RCCommand& command, const char token)
{
	switch (token)
	{
	case 'F':
		command = RCCommand::FORWARD;
		break;
	case 'B':
		command = RCCommand::BACK;
		break;
	case 'L':
		command = RCCommand::LEFT;
		break;
	case 'R':
		command = RCCommand::RIGHT;
		break;
	default:
		std::cerr << "Bad command specified. [" << token << "]\n";
		return false;
	}
}

bool ParseCommands(RCCar& car)
{
	std::cout << "Input all commands in series to execute. Eg. FLBLRF\n";

	std::string line;
	std::getline(std::cin, line);

	for (int i = 0; i < line.length(); i++)
	{
		RCCommand command;
		if (!ParseSingleCommand(command, line[i]))
		{
			return false;
		}

		if (!car.PerformCommand(command))
		{
			std::cerr << "unsuccessful, rccar hit wall! ";
			car.PrintPosition();
			return false;
		}
	}
}

int main(int argc, char* argv[])
{
	bool valid_room_size = false;
	int width = -1;
	int height = -1;
	const char token_separator = ' ';
	if (!ParseRoomSize(width, height, token_separator))
	{
		return -1;
	}
	Room room(width, height);

	Vec2 car_position = {};
	Vec2 car_direction = {};
	if (!ParseStartingLocation(car_position, car_direction, token_separator))
	{
		return -1;
	}

	RCCar car(&room, car_position, car_direction);
	if (!car.IsValid())
	{
		return -1;
	}
	
	DEBUG_CALL(TestAllCommands(car));
	DEBUG_CALL(car.SetVerboseCommands(true));
	if (!ParseCommands(car))
	{
		return -1;
	}

	std::cout << "Successful! ";
	car.PrintPosition();

	return 0;
}