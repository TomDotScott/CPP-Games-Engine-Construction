#pragma once
#include <string>
#include <vector>

namespace animations
{
	// Animation identifiers
	// THE PLAYER
	extern const std::vector<std::string> PLAYER_IDLE;
	extern const std::vector<std::string> PLAYER_WALK;
	extern const std::vector<std::string> PLAYER_JUMP;
	extern const std::vector<std::string> PLAYER_CLIMB;

	// POWERED UP PLAYER
	extern const std::vector<std::string> PLAYER_POWER_UP_IDLE;
	extern const std::vector<std::string> PLAYER_POWER_UP_WALK;
	extern const std::vector<std::string> PLAYER_POWER_UP_JUMP;
	extern const std::vector<std::string> PLAYER_POWER_UP_CLIMB;

	// FIREBALL AND EXPLOSION
	extern const std::vector<std::string> FIREBALL_SPIN;
	extern const std::vector<std::string> FIREBALL_EXPLOSION;
	
	// THE COINS
	extern const std::vector<std::string> COIN_SPIN;
	
	// THE SNAILS
	extern const std::vector<std::string> SNAIL_WALK;
	extern const std::vector<std::string> SNAIL_IN_SHELL;
	extern const std::vector<std::string> SNAIL_SHELL_HIT;
	extern const std::vector<std::string> SNAIL_SHELL_CRACK;
	
	// THE SLIMES
	extern const std::vector<std::string> SLIME_WALK;
	extern const std::vector<std::string> SLIME_SQUASHED;
	extern const std::vector<std::string> SLIME_SHELL_HIT;
}
