#pragma once
#include <string>
#include <vector>

namespace animations
{
	// Animation identifiers
	// SMALL PLAYER
	extern const std::vector<std::string> PLAYER_SMALL_DEAD;
	extern const std::vector<std::string> PLAYER_SMALL_IDLE;
	extern const std::vector<std::string> PLAYER_SMALL_WALK;
	extern const std::vector<std::string> PLAYER_SMALL_JUMP;
	extern const std::vector<std::string> PLAYER_SMALL_CLIMB;
	
	// THE PLAYER
	extern const std::vector<std::string> PLAYER_DEAD;
	extern const std::vector<std::string> PLAYER_IDLE;
	extern const std::vector<std::string> PLAYER_WALK;
	extern const std::vector<std::string> PLAYER_JUMP;
	extern const std::vector<std::string> PLAYER_CLIMB;

	// POWERED UP PLAYER
	extern const std::vector<std::string> PLAYER_POWER_UP_DEAD;
	extern const std::vector<std::string> PLAYER_POWER_UP_IDLE;
	extern const std::vector<std::string> PLAYER_POWER_UP_WALK;
	extern const std::vector<std::string> PLAYER_POWER_UP_JUMP;
	extern const std::vector<std::string> PLAYER_POWER_UP_CLIMB;

	// FIREBALL AND EXPLOSION
	extern const std::vector<std::string> FIREBALL_SPIN;
	extern const std::vector<std::string> FIREBALL_EXPLOSION;
	
	// COINS
	extern const std::vector<std::string> COIN_SPIN;

	// GEMS
	extern const std::vector<std::string> FIRE_GEM_SPIN;
	extern const std::vector<std::string> GROW_GEM_SPIN;
	
	// THE SNAILS
	extern const std::vector<std::string> SNAIL_WALK;
	extern const std::vector<std::string> SNAIL_IN_SHELL;
	extern const std::vector<std::string> SNAIL_SHELL_HIT;
	extern const std::vector<std::string> SNAIL_SHELL_CRACK;
	
	// THE SLIMES
	extern const std::vector<std::string> SLIME_WALK;
	extern const std::vector<std::string> SLIME_SQUASHED;
	extern const std::vector<std::string> SLIME_SHELL_HIT;

	// THE FLAG
	extern const std::vector<std::string> FLAG_WAVE;
}
