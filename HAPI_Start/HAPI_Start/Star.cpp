#include "Star.h"

#include <iostream>

Star::Star() :
	m_eyeDist(50),
	m_size(0),
	m_physicsClock(),
	m_projectedPosition(),
	m_position(),
	m_velocity(),
	m_acceleration(),
	m_maxVelocity(5, 5, 5) {
	Reset();
}

Vector2 Star::ProjectedPoints(const Vector3& _currentPosition) const {
	// Sx = ((eyeDist * (Px - Cx)) / (eyeDist + Pz)) + Cx
	// Sy = ((eyeDist * (Py - Cy)) / (eyeDist + Pz)) + Cy
	return {
		((m_eyeDist * (_currentPosition.x - k_screenSize / 2)) / (m_eyeDist + _currentPosition.z)) + k_screenSize / 2,
		((m_eyeDist * (_currentPosition.y - k_screenSize / 2)) / (m_eyeDist + _currentPosition.z)) + k_screenSize / 2
	};
}


void Star::Update() {
	Move();
	Grow();
}

void Star::Render(HAPISPACE::BYTE* _screen) const {
	// Calculate the new X and Y
	for (int x = static_cast<int>(m_projectedPosition.x); x < (static_cast<int>(m_projectedPosition.x) + m_size); x++) {
		for (int y = static_cast<int>(m_projectedPosition.y); y < (static_cast<int>(m_projectedPosition.y) + m_size); y++) {
			_screen[(1000 * x + y) * 4] = 255;
			_screen[(1000 * x + y) * 4 + 1] = 219;
			_screen[(1000 * x + y) * 4 + 2] = 38;
			_screen[(1000 * x + y) * 4 + 3] = 255;
		}
	}
}

void Star::Move() {
	//CPU "ticks" since the program started.
	const clock_t programTickCount = clock() - m_physicsClock;

	//Conversion rate between ticks and millaseconds.
	const float ticksToMilliseconds = 1000.0f / CLOCKS_PER_SEC;

	//Convert from ticks to seconds.
	const float deltaTime = (programTickCount * ticksToMilliseconds);

	m_velocity = m_velocity + (m_acceleration * (deltaTime / 1000));

	m_velocity.Limit(m_maxVelocity);

	m_position = m_position + m_velocity;

	// Reset the physics clock for the next loop
	m_physicsClock = clock();

	m_projectedPosition = ProjectedPoints(m_position);

	if (m_position.x < 0 || m_projectedPosition.x < 0) {
		Reset();
	}

	else if (m_position.x > k_screenSize - m_size || m_projectedPosition.x > k_screenSize - m_size) {
		Reset();
	}

	else if (m_position.y < 0 || m_projectedPosition.y < 0) {
		Reset();
	}

	else if (m_position.y > k_screenSize - m_size || m_projectedPosition.y > k_screenSize - m_size) {
		Reset();
	}
}

void Star::Reset() {
	m_size = RandRange(1, 10);
	m_position = { static_cast<float>(RandRange(0, k_screenSize)),
		static_cast<float>(RandRange(0, k_screenSize)),
		static_cast<float>(RandRange(1, 10))
	};
	m_projectedPosition = {};
	m_velocity = {};
	m_acceleration.GenNonZeroVector(2);
}

void Star::Grow() {
}
