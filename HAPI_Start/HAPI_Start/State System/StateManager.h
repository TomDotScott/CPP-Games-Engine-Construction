#pragma once
#include "../Graphics/TextureManager.h"
#include "State.h"

enum class eState
{
	e_MainMenu,
	e_ControlsMenu,
	e_LevelSelectionMenu,
	e_Game,
	e_GameOver
};

class StateManager
{
public:
	StateManager();
	~StateManager();

	// Non-copyable and non-moveable
	StateManager(const StateManager& other)                = delete;
	StateManager(StateManager&& other) noexcept            = delete;
	StateManager& operator=(const StateManager& other)     = delete;
	StateManager& operator=(StateManager&& other) noexcept = delete;

	void ChangeState(eState state);
	void OnCreate(eState state);
	void Update();
	void Render();

	eState GetCurrentState() const;

private:
	void SetState(State* state);

	eState m_currentState;
	State* m_state;

	TextureManager m_textureManager;

	/*On the main menu and controls menu, HAPI still registers the
	space button being pressed a cooldown of half a second is used
	to smooth things out*/
	float   m_inputCoolDown;
	clock_t m_clock;
};

extern StateManager STATE_MANAGER;
