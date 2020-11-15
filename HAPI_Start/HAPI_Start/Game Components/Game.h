#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include "Ball.h"
#include "Brick.h"
#include "Player.h"

enum class EKeyCode;

class Game {
public:
	Game();
	void Update();
	void Render();
	bool PLAYER_WON;
	bool PLAYER_LOST;

private:
	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData;
	const HAPISPACE::HAPI_TControllerData& m_controllerData;
	Player m_player;
	clock_t m_gameClock;
	
	int m_gameScore;
	int m_currentChunk{ 0 };
	int m_currentSprite;

	Vector2 m_backgroundPosition;

	std::vector<std::vector<std::string>> m_levelData;
	
	void CreateSprite(const std::string& spriteSheetIdentifier);
	void RenderBackground();
	void RenderChunk(int chunkNum);
	bool GetKey(EKeyCode keyCode) const;
	void HandleKeyBoardInput();
	void HandleControllerInput();
	float DeltaTime() const;
	void LoadLevel();
};

enum class EKeyCode {
	None = 0, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, A = 65, B = 66, C = 67, D = 68, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};