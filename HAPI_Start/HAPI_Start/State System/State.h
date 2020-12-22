#pragma once
#include <ctime>

class TextureManager;

class State
{
public:
	virtual ~State() = default;
	virtual bool Initialise() = 0;
	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Render(TextureManager& textureManager) = 0;

protected:
	float DeltaTime(clock_t& gameClock);
};