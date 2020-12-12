#pragma once
#include "Entity.h"

class PickUpGem final : public Entity
{
public:
	explicit PickUpGem(const int entityID, Vector2 position, bool visible = false);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	CollisionBoxes GenerateCollisionBoxes() override;

	void Initialise(Vector2 position);
	
	void SetActiveState(bool visibility);
	bool GetActiveState() const;

private:
	bool m_activeState;
	void Move(float deltaTime) override;
};

