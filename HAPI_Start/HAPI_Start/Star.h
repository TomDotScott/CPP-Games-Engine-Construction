#pragma once
#include "HAPI_Types.h"
#include "Vector.h"
#include <iostream>

constexpr int k_screenSize = 1000;

class Star {
public:
	explicit Star(const int _size, const Vector3 _pos, const Vector3 _acc) :
		m_size(_size),
		m_position(_pos),
		m_acceleration(_acc) {
	};


	Vector2 ProjectedPoints(Vector3& _currentPosition) const
	{
		// Sx = ((eyeDist * (Px - Cx)) / (eyeDist + Pz)) + Cx
		// Sy = ((eyeDist * (Py - Cy)) / (eyeDist + Pz)) + Cy
		return {
			((m_eyeDist * (_currentPosition.x - 500)) / (m_eyeDist + _currentPosition.z)) + 500,
			((m_eyeDist * (_currentPosition.y - 500)) / (m_eyeDist + _currentPosition.z)) + 500
		};
	}

	
	void Update();
	void Render(HAPISPACE::BYTE* _screen) const;
	
	void Move();
	void Reset();
	void Grow();

private:

	static int RandRange(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}
	
	int m_eyeDist{ 50 };

	int m_size{};

	Vector2 m_projectedPosition{};
	
	Vector3 m_position;
	Vector3 m_currentVelocity{};
	Vector3 m_acceleration;

	Vector3 m_maxVelocity{ 5, 5, 5 };
};

