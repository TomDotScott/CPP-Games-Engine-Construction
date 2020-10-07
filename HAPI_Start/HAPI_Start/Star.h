#pragma once
#include "Vector3.h"
class Star {
public:
	explicit Star(const int _size, const Vector3 _pos, const Vector3 _vel) :
		m_size(_size),
		m_position(_pos),
		m_velocity(_vel) {
	};

	void Move();
	void Reset();
	void Grow();
	void Render();

private:
	int m_size{};
	Vector3 m_position;
	Vector3 m_velocity;
};

