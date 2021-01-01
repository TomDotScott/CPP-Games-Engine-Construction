#pragma once
#include "Entity.h"

enum class ePowerUpType
{
	e_Grower,
	e_FireThrower
};

class PickUpGem final : public Entity
{
public:
	explicit PickUpGem(int entityID, const Vector2& position, ePowerUpType powerUpType, bool visible = false);
	void     Update(float deltaTime) override;
	void     CheckEntityCollisions(Entity& other) override;

	void Initialise(Vector2 position);

	ePowerUpType GetType() const;
	
	void         SetPowerUpType(ePowerUpType type);

private:
	ePowerUpType   m_type;
	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
