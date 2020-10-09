#pragma once
struct Vector2 {
	Vector2(const int _x = 0, const int _y = 0) :
		x(_x),
		y(_y) {
	};

	void Add(Vector2 _other) {
		x += _other.x;
		y += _other.y;
	}

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

struct Vector3 {
	Vector3(const int _x = 0, const int _y = 0, const int _z = 0) :
		x(_x),
		y(_y),
		z(_z) {
	};

	void Add(Vector3 _other) {
		x += _other.x;
		y += _other.y;
		z += _other.z;
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
		x = GenValue(_max);
		y = GenValue(_max);
		z = GenValue(_max);
	}


	int x, y, z;

private:
	static int GenValue(const int _max) {
		const int coinFlip = 0 + (rand() % (1 - 0 + 1));
		if (static_cast<bool>(coinFlip) == true) {
			return 1 + (rand() % (_max - 1));
		} else {
			return -1 * (1 + (rand() % (_max - 1)));
		}
	}
};


