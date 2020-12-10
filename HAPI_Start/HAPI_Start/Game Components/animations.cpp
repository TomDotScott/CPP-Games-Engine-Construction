#include "Animations.h"
/* PLAYER BEGIN */
const std::vector<std::string> animations::PLAYER_IDLE
{
	"Player_Idle_Body_1", "Player_Idle_Body_2"
};

const std::vector<std::string> animations::PLAYER_WALK
{
	"Player_Walk_Body_1", "Player_Walk_Body_2"
};

const std::vector<std::string> animations::PLAYER_JUMP{
	"Player_Jump_Body_1"
};

const std::vector<std::string> animations::PLAYER_CLIMB
{
	"Player_Climb_Body_1", "Player_Climb_Body_2"
};

// POWERED UP PLAYER
const std::vector<std::string> animations::PLAYER_POWER_UP_IDLE
{
	"Player_Power_Up_Idle_Body_1", "Player_Power_Up_Idle_Body_2"
};

const std::vector<std::string> animations::PLAYER_POWER_UP_WALK
{
	"Player_Power_Up_Walk_Body_1", "Player_Power_Up_Walk_Body_2"
};

const std::vector<std::string> animations::PLAYER_POWER_UP_JUMP{
	"Player_Power_Up_Jump_Body_1"
};

const std::vector<std::string> animations::PLAYER_POWER_UP_CLIMB
{
	"Player_Power_Up_Climb_Body_1", "Player_Climb_Body_2"
};

/* FIREBALL BEGIN */
const std::vector<std::string> animations::FIREBALL_SPIN
{
	"Fireball_1", "Fireball_2", "Fireball_3", "Fireball_4",
	"Fireball_5", "Fireball_6", "Fireball_7", "Fireball_8"
};

const std::vector<std::string> animations::FIREBALL_EXPLOSION
{
	"Explosion_1", "Explosion_2", "Explosion_3", "Explosion_4",
	"Explosion_5", "Explosion_6", "Explosion_7", "Explosion_8",
};

/* COIN BEGIN */
const std::vector<std::string> animations::COIN_SPIN
{
	"Coin_1", "Coin_2" , "Coin_3" , "Coin_4" ,
	"Coin_5" , "Coin_6" , "Coin_7" , "Coin_8"
};

/* SNAIL BEGIN */
const std::vector<std::string> animations::SNAIL_WALK
{
	"Snail_1", "Snail_2"
};

const std::vector<std::string> animations::SNAIL_IN_SHELL
{
	"Snail_Shell"
};

const std::vector<std::string> animations::SNAIL_SHELL_HIT
{
	"Snail_Snail_Shell_Hit"
};

const std::vector<std::string> animations::SNAIL_SHELL_CRACK
{
	"Snail_Shell", "Snail_Shell_Crack_1",
	"Snail_Shell_Crack_2" , "Snail_Shell_Crack_3"
};

/* SLIME BEGIN */
const std::vector<std::string> animations::SLIME_WALK
{
	"Slime_1", "Slime_2"
};

const std::vector<std::string> animations::SLIME_SQUASHED
{
	"Slime_Squashed"
};

const std::vector<std::string> animations::SLIME_SHELL_HIT
{
	"Slime_Snail_Shell_Hit"
};