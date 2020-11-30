#pragma once
#include <string>
#include <vector>

namespace animations
{
	// Animation identifiers
	// THE PLAYER
	extern std::vector<std::string> PLAYER_IDLE;
	extern std::vector<std::string> PLAYER_WALK;
	extern std::vector<std::string> PLAYER_JUMP;
	extern std::vector<std::string> PLAYER_CLIMB;
	// THE COINS
	extern std::vector<std::string> COIN_SPIN;
	// THE SNAILS
	extern std::vector<std::string> SNAIL_WALK;
	extern std::vector<std::string> SNAIL_IN_SHELL;
	extern std::vector<std::string> SNAIL_SHELL_HIT;
	extern std::vector<std::string> SNAIL_SHELL_CRACK;
	// THE SLIMES
	extern std::vector<std::string> SLIME_WALK;
	extern std::vector<std::string> SLIME_SQUASHED;
	extern std::vector<std::string> SLIME_SHELL_HIT;
}
