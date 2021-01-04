#pragma once
#include <ctime>
#include "HAPI_Types.h"

enum class eKeyCode;
class TextureManager;

class State
{
public:
	explicit State();

	virtual ~State()                         = default;
	State(const State& other)                = delete;
	State(State&& other) noexcept            = delete;
	State& operator=(const State& other)     = delete;
	State& operator=(State&& other) noexcept = delete;

	virtual bool Initialise(TextureManager& textureManager) = 0;
	virtual bool Unload(TextureManager& textureManager) = 0;
	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Render(TextureManager& textureManager) = 0;
	static float DeltaTime(clock_t& gameClock);

protected:
	const HAPISPACE::HAPI_TKeyboardData&   m_keyboardData;

	bool GetKey(eKeyCode keyCode) const;
};

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
