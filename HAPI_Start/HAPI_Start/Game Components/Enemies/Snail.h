#pragma once
#include "Enemy.h"

// The snail walks, goes into it's shell when it's jumped on
// If it is jumped on when in it's shell, it's shell can slide
// If not, after some time, it can get back up and walk again
enum class e_SnailState {
	eWalking, eSquashed, eSliding
};

class Snail final : public Enemy{
public:
	explicit Snail(Vector2 startingPos);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity* other) override;
	e_SnailState GetSnailState() const;
protected:
	void Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
	
private:
	float m_speedMultiplier;
	float m_inShellDuration;
	e_SnailState m_snailState;
};
