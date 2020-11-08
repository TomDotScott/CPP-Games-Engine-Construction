#pragma once
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Vector.h"
#include "Entity.h"

enum class EBrickType {
	eRed, eGreen
};

class Brick : public Entity {
public:
	explicit Brick(Vector2 position, EBrickType type);
	void Update(float deltaTime) override;
	
	EBrickType GetType() const;
	bool GetIsActive() const;
	void CheckCollision(const BoundsRectangle other);
private:
	bool m_isActive;
	Vector2 m_position;
	EBrickType m_type;
};
