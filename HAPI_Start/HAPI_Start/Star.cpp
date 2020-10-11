#include "Star.h"
#include <iostream>
#include "Constants.h"

Star::Star() :
	m_eyeDist(50),
	m_size(0),
	m_physicsClock(),
	m_projectedPosition(),
	m_position(),
	m_velocity(),
	m_acceleration(),
	m_maxVelocity(7.5, 7.5, 10) {
	Reset();
}

Vector2 Star::ProjectedPoints(const Vector3& _currentPosition) const {
	// Sx = ((eyeDist * (Px - Cx)) / (eyeDist + Pz)) + Cx
	// Sy = ((eyeDist * (Py - Cy)) / (eyeDist + Pz)) + Cy
	return {
		((m_eyeDist * (_currentPosition.x - static_cast<float>(constants::k_screenWidth) / 2)) / (m_eyeDist + _currentPosition.z)) +
		static_cast<float>(constants::k_screenWidth) / 2,
		((m_eyeDist * (_currentPosition.y - static_cast<float>(constants::k_screenHeight) / 2)) / (m_eyeDist + _currentPosition.z)) +
		static_cast<float>(constants::k_screenHeight) / 2
	};
}


void Star::Update() {
	Move();
	Grow();
	CheckBounds();
}

void Star::Render(HAPISPACE::BYTE* _screen) const {
	// Calculate the new X and Y
	for (int x = static_cast<int>(m_projectedPosition.x); x < (static_cast<int>(m_projectedPosition.x + m_size)); x++) {
		for (int y = static_cast<int>(m_projectedPosition.y); y < (static_cast<int>(m_projectedPosition.y + m_size)); y++) {
			_screen[(constants::k_screenWidth * x + y) * 4] = 255;
			_screen[(constants::k_screenWidth * x + y) * 4 + 1] = 219;
			_screen[(constants::k_screenWidth * x + y) * 4 + 2] = 38;
			_screen[(constants::k_screenWidth * x + y) * 4 + 3] = 255;
		}
	}
}

void Star::SetEyeDistance(const float _eyeDist) {
	m_eyeDist = _eyeDist;
}

void Star::SetMaxVelocity(const Vector3 _newMax) {
	m_maxVelocity = _newMax;
}

void Star::Move() {
	//CPU "ticks" since the program started.
	const clock_t programTickCount = clock() - m_physicsClock;

	//Conversion rate between ticks and milliseconds.
	const float ticksToMilliseconds = 1000.0f / CLOCKS_PER_SEC;

	//Convert from ticks to seconds.
	const float deltaTime = (programTickCount * ticksToMilliseconds);

	m_velocity = m_velocity + (m_acceleration * (deltaTime / 1000));

	m_velocity.Limit(m_maxVelocity);

	m_position = m_position + m_velocity;

	m_projectedPosition = ProjectedPoints(m_position);

	// Reset the physics clock for the next loop
	m_physicsClock = clock();
}

void Star::CheckBounds() {
	if (m_position.x < 0 ||
		m_projectedPosition.x < 0 ||
		m_position.x > constants::k_screenWidth - m_size ||
		m_projectedPosition.x > constants::k_screenWidth - m_size ||
		m_position.y < 0 ||
		m_projectedPosition.y < 0 ||
		m_position.y > constants::k_screenHeight - m_size ||
		m_projectedPosition.y > constants::k_screenHeight - m_size) {
		Reset();
	}
}

void Star::Reset() {
	m_size = 0.f;
	m_position = { static_cast<float>(constants::rand_range(0, constants::k_screenWidth)),
		static_cast<float>(constants::rand_range(0, constants::k_screenHeight)),
		static_cast<float>(constants::rand_range(1, 10))
	};
	m_projectedPosition = {};
	m_velocity = {};
	m_acceleration.GenNonZeroVector(2);
	m_acceleration.z = -abs(m_acceleration.z);
}

void Star::Grow() {
	m_size += abs(m_position.z) / 100;
}
