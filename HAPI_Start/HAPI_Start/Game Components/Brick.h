#pragma once
#include "../Utilities/Vector.h"

class Brick {
public:
	explicit Brick(Vector2 position);
	void Render();
	Vector2 m_position;
private:
	bool m_isActive;
};
