#pragma once
#include "HAPI_Types.h"
#include "Vector.h"

constexpr int k_screenSize = 1000;

class Star {
public:
	Star();
	
	Vector2 ProjectedPoints(const Vector3& _currentPosition) const;
	
	void Update();
	void Render(HAPISPACE::BYTE* _screen) const;

private:
	int m_eyeDist;
	int m_size;
	Vector2 m_projectedPosition;
	Vector3 m_position;
	Vector3 m_currentVelocity;
	Vector3 m_acceleration;
	Vector3 m_maxVelocity;

	static int RandRange(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}

	void Move();
	void Reset();
	void Grow();
};

