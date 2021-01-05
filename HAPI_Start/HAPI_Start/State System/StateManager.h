#pragma once
#include "../Graphics/TextureManager.h"
#include "State.h"

/**
 * \brief The various states that the game engine's
 * state system can be in
 */
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

	/**
	 * \brief Changes the current state in the state machine
	 * \param state The state to change to
	 */
	void ChangeState(eState state);
	/**
	 * \brief This function needs to be called when the state system
	 * initialises. It sets up the texture manager and initialises the
	 * first state
	 * \param state The state to be first initialised to 
	 */
	void OnCreate(eState state);
	/**
	 * \brief Update gets called every frame, this invokes the state's
	 * Update() function
	 */
	void Update();
	/**
	 * \brief Renders the current state's contents to the screen
	 */
	void Render();

	eState GetCurrentState() const;

private:
	void SetState(State* state);

	eState m_currentState;
	State* m_state;

	TextureManager m_textureManager;

	// On the main menu and controls menu, HAPI still registers the
	// buttons being pressed a cooldown of half a second is used
	// to smooth things out
	float   m_inputCoolDown;
	clock_t m_clock;
};

extern StateManager STATE_MANAGER;
