#pragma once
#include "Entity.h"

enum class ePowerUpType
{
	e_Grower, e_FireThrower
};

class PickUpGem final : public Entity
{
public:
	explicit PickUpGem(const int entityID, Vector2 position, ePowerUpType powerUpType, bool visible = false);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	
	void Initialise(Vector2 position);

	ePowerUpType GetType() const;
	void SetPowerUpType(ePowerUpType type);
	
	void SetActiveState(bool visibility);
	bool GetActiveState() const;

private:
	ePowerUpType m_type;
	bool m_activeState;
	void Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};

