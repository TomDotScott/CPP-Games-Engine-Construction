#pragma once
#include "HAPI_Types.h"
#include "Vector3.h"
#include "Matrix.h"

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
	int m_size{};
	Vector3 m_position;
	Vector3 m_velocity;

	Matrix m_projectionMatrix{ 2, 3, { {1, 0, 0}, {0, 1, 0} } };
};

