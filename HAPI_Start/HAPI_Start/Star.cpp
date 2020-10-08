#include "Star.h"

void Star::Update() {
	// move
	Move();
	// reset position if off screen
	if ((m_position.x < 0 || m_position.x > 999 - m_size || m_position.y < 0 || m_position.y > 999 - m_size) && (
		m_projectedPosition.x < 0 || m_projectedPosition.x > 999 - m_size || m_projectedPosition.y < 0 ||
		m_projectedPosition.y > 999 - m_size))
	{
		Reset();
	}
}

void Star::Render(HAPISPACE::BYTE* _screen) const {
	// Calculate the new X and Y
	for (int x = m_projectedPosition.x; x < (m_projectedPosition.x + m_size); x++) {
		for (int y = m_projectedPosition.y; y < (m_projectedPosition.y + m_size); y++) {
			_screen[(1000 * x + y) * 4] = 255;
			_screen[(1000 * x + y) * 4 + 1] = 219;
			_screen[(1000 * x + y) * 4 + 2] = 38;
			_screen[(1000 * x + y) * 4 + 3] = 255;
		}
	}
}

void Star::Move()
{
	m_currentVelocity.Add(m_acceleration);

	m_currentVelocity.Limit(m_maxVelocity);
	
	m_position.Add(m_currentVelocity);

	m_projectedPosition = ProjectedPoints(m_position);
}

void Star::Reset()
{
	// Hard coding is bad....
	m_size = RandRange(1, 5);

	m_position = { RandRange(0, k_screenSize), RandRange(0, k_screenSize), RandRange(0, 100) };

	m_acceleration = { RandRange(0, 2), RandRange(0, 2), -RandRange(0, 2) };
}

void Star::Grow()
{
}
