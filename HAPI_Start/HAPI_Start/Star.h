#pragma once
#include "HAPI_Types.h"
#include "Vector.h"
#include <iostream>

class Star {
public:
	explicit Star(const int _size, const Vector3 _pos, const Vector3 _vel) :
		m_size(_size),
		m_position(_pos),
		m_velocity(_vel) {
	};

	void Update();
	void Render(HAPISPACE::BYTE* _screen) const;
	
	void Move();
	void Reset();
	void Grow();

private:

	int m_eyeDist{ 50 };

	int m_size{};
	Vector3 m_position;
	Vector3 m_velocity;
};

