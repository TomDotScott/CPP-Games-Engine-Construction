#include "Star.h"

void Star::Update() {
	// move
	Move();
	// reset if off screen
	if(m_position.x < 0 || m_position.x > 1000 || m_position.y < 0 || m_position.y > 1000)
	{
		Reset();
	}
}

void Star::Render(HAPISPACE::BYTE* _screen) const {
	// Hard coding for now, will fix later...
	for (int x = 0; x < 1000; x++) {
		for (int y = 0; y < 1000; y++) {
			if (x <= m_size + m_position.x && x >= m_position.x && y <= m_size + m_position.y && y >= m_position.y) {
				_screen[(1000 * x + y) * 4] = 255;
				_screen[(1000 * x + y) * 4 + 1] = 219;
				_screen[(1000 * x + y) * 4 + 2] = 38;
				_screen[(1000 * x + y) * 4 + 3] = 255;
			}
		}
	}
}

void Star::Move()
{
	// add the velocity to the position
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
	m_position.z += m_velocity.z;	
}

void Star::Reset()
{
	// Hard coding is bad....
	m_size = 5 + (rand() % (100 - 5 + 1));
	m_position.x = 0 + (rand() % (1000 - 0 + 1));
	m_position.y = 0 + (rand() % (1000 - 0 + 1));
	m_position.z = 0 + (rand() % (1000 - 0 + 1));
	m_velocity.x = 0 + (rand() % (10 - 0 + 1));
	m_velocity.x = 0 + (rand() % (10 - 0 + 1));
	m_velocity.x = 0 + (rand() % (10 - 0 + 1));
}

void Star::Grow()
{
}
