#pragma once
class Vector2 {
public:
	Vector2(const int _x = 0, const int _y = 0) :
		x(_x),
		y(_y) {
	};

	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator*(const int rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	
	void Limit(Vector2 _max) {
		if (x > _max.x) {
			x = _max.x;
		}
		if (y > _max.y) {
			y = _max.y;
		}
	}

	int x, y;
};

inline Vector2 Vector2::operator +(const Vector2& rhs) const {
	return{ this->x + rhs.x, this->y + rhs.y };
}

inline Vector2 Vector2::operator*(const int rhs) const {
	return{ this->x * rhs, this->y * rhs };
}

inline Vector2 Vector2::operator -(const Vector2& rhs) const {
	return{ rhs.x - this->x, rhs.y - this->y };
}


class Vector3 {
public:
	Vector3(const float _x = 0, const float _y = 0, const float _z = 0) :
		x(_x),
		y(_y),
		z(_z) {
	}

	void Limit(Vector3 _max) {
		if (x > _max.x) {
			x = _max.x;
		}
		if (y > _max.y) {
			y = _max.y;
		}
		if (z > _max.z) {
			z = _max.z;
		}
	}

	void GenNonZeroVector(const int _max) {
		x = static_cast<float>(GenNonZeroInteger(_max));
		y = static_cast<float>(GenNonZeroInteger(_max));
		z = static_cast<float>(GenNonZeroInteger(_max));
	}

	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator*(const float rhs) const;
	Vector3 operator/(const float rhs) const;
	Vector3 operator-(const Vector3& rhs) const;



	float x, y, z;

private:
	/// <summary>
	/// Generates a non-zero integer in the range -max < x < max
	/// </summary>
	static int GenNonZeroInteger(const int _max) {
		const int coinFlip = 0 + (rand() % (1 - 0 + 1));
		if (static_cast<bool>(coinFlip) == true) {
			return 1 + (rand() % (_max - 1));
		} else {
			return -(1 * (1 + (rand() % (_max - 1))));
		}
	}
};

inline Vector3 Vector3::operator +(const Vector3& rhs) const {
	return{ this->x + rhs.x, this->y + rhs.y, this->z + rhs.z };
}

inline Vector3 Vector3::operator*(const float rhs) const {
	return{ this->x * rhs, this->y * rhs, this->z * rhs };
}

inline Vector3 Vector3::operator/(const float rhs) const {
	return{ this->x / rhs, this->y / rhs, this->z / rhs };
}

inline Vector3 Vector3::operator -(const Vector3& rhs) const {
	return{ rhs.x - this->x, rhs.y - this->y, rhs.z - this->z };
}

