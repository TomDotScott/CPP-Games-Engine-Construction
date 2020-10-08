#include "Star.h"

void Star::Update() {
	// move
	Move();
	// reset if off screen
	if (m_position.x < 0 || m_position.x > 1000 || m_position.y < 0 || m_position.y > 1000)
	{
		Reset();
	}
}

void Star::Render(HAPISPACE::BYTE* _screen) const {
	// Use the Projection Matrix....
	Matrix pointMat{ 3, 1, {{static_cast<float>(m_position.x)}, {static_cast<float>(m_position.y)}, {static_cast<float>(m_position.z)}} };

	Matrix projectedMatrix = Matrix::MatMul(m_projectionMatrix, pointMat);

	// Hard coding for now, will fix later...
	for (int x = 0; x < 1000; x++) {
		for (int y = 0; y < 1000; y++) {
			if (x <= m_size + projectedMatrix.GetValue(0, 0) && x >= projectedMatrix.GetValue(0, 0) && y <= m_size + projectedMatrix.GetValue(1, 0) && y >= projectedMatrix.GetValue(1, 0)) {
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
