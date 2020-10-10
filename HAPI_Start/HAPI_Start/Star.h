#pragma once
#include <chrono>
#include "HAPI_Types.h"
#include "Vector.h"

constexpr int k_screenSize = 1000;

class Star {
public:
	Star();
	
	Vector2 ProjectedPoints(const Vector3& _currentPosition) const;
	
	void Update();
	void Render(HAPISPACE::BYTE& _screen) const;

	void SetEyeDistance(float _eyeDist);
	void SetMaxVelocity(Vector3 _newMax);

private:
	float m_eyeDist;
	float m_size;
	clock_t m_physicsClock;
	Vector2 m_projectedPosition;
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_maxVelocity;

	static int RandRange(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}

	void Move();
	void CheckBounds();
	void Reset();
	void Grow();
};

