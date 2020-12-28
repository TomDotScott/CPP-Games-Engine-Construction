#pragma once
#include "Entity.h"

enum class eLeverState
{
	e_Static, e_Swinging
};

class Lever final : public Entity
{
public:
	Lever(int entityID, Vector2 startPosition);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	bool GetIsActive() const;
	eLeverState GetLeverState() const;
	
	void SetActive(bool active);
	
private:
	bool m_active;
	eLeverState m_leverState;
	
	void Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};