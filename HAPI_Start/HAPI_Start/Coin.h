#pragma once
#include "Game Components/Entity.h"

class Coin : public Entity {
public:
	explicit Coin(Vector2 position, bool visible);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity* other) override;
	
	CollisionBoxes GenerateCollisionBoxes() override;
	
	void SetVisible(bool visibility);
	bool GetIsVisible() const;

private:
	float m_lifeTime;
	
	bool m_isVisible;

	void Move(float deltaTime) override;
};

