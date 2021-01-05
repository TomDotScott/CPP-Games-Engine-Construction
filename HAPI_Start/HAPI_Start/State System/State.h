#pragma once
#include <ctime>
#include "HAPI_Types.h"

enum class eKeyCode;
class TextureManager;

/**
 * \brief An outline for each state in the game's state system
 */
class State
{
public:
	explicit State();

	virtual ~State()                         = default;
	
	// States are non-moveable and non-copyable
	State(const State& other)                = delete;
	State(State&& other) noexcept            = delete;
	State& operator=(const State& other)     = delete;
	State& operator=(State&& other) noexcept = delete;

	/**
	 * \brief Initialises the state, loading the appropriate
	 * sprites, textures, music and sound effect files
	 * \param textureManager A reference to the texture manager class
	 * \return True if the state initialised successfully
	 */
	virtual bool Initialise(TextureManager& textureManager) = 0;
	/**
	 * \brief Before the destructor gets called, the state has to
	 * unload. This means the textures, sprites, music and sound effects
	 * are removed from their buffers
	 * \param textureManager A reference to the TextureManager class
	 * \return True if the state unloads successfully
	 */
	virtual bool Unload(TextureManager& textureManager) = 0;
	/**
	 * \brief Every state has keyboard and controller input
	 */
	virtual void Input() = 0;
	/**
	 * \brief Update() is called every frame
	 */
	virtual void Update() = 0;
	/**
	 * \brief Renders the state's components to the screen
	 * \param textureManager A reference to the texturemanager
	 */
	virtual void Render(TextureManager& textureManager) = 0;
	static float DeltaTime(clock_t& gameClock);

protected:
	const HAPISPACE::HAPI_TKeyboardData&   m_keyboardData;

	bool GetKey(eKeyCode keyCode) const;
};

/**
 * \brief Each key in the game
 */
enum class eKeyCode
{
	NONE = -1,
	ENTER = 13,
	SHIFT = 16,
	ESCAPE = 27,
	SPACE = 32,
	LEFT = 37,
	UP = 38,
	RIGHT = 39,
	DOWN = 40,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	G = 71,
	P = 80,
	R = 82,
	S = 83,
	W = 87,
	Y = 89,
	F1 = 112,
	F2 = 113,
	F3 = 114
};
