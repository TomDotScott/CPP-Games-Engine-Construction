#pragma once
#include "Enemy.h"

// The snail walks, goes into it's shell when it's jumped on
// If it is jumped on when in it's shell, it's shell can slide
// If not, after some time, it can get back up and walk again
enum class eSnailState
{
	e_Walking, e_Squashed, e_Sliding, e_ShellHit, e_Cracking
};

class Snail final : public Enemy
{
public:
	explicit Snail(const int entityID, Vector2 startingPos);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	void CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes) override;
	eSnailState GetSnailState() const;
	void Squash() override;
protected:
	void Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;

private:
	float m_speedMultiplier;
	float m_inShellDuration;
	eSnailState m_snailState;
};
