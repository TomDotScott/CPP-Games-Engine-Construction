#pragma once
#include "Enemy.h"


enum class eSnailState
{
	e_Walking,
	e_Squashed,
	e_Sliding,
	e_ProjectileHit,
	e_Cracking
};

/**
 * \brief The snail walks, goes into it's shell when it's jumped on.
 * If it is jumped on when in it's shell, it's shell can slide.
 * If not, after some time, it can get back up and walk again. Snails
 * always stay on platforms unless they are sliding in the shell. Snail
 * shells damage enemies and the player
 */
class Snail final : public Enemy
{
public:
	explicit       Snail(int entityID, const Vector2& startingPos);
	void           Update(float deltaTime) override;
	void           CheckEntityCollisions(Entity& other) override;
	void           CheckSnailShellCollisions(const CollisionBoxes& snailShellCollisionBoxes) override;
	
	/**
	 * \brief Gets the current state of the snail
	 * \return The current state of the snail, detailed in the eSnailState enum
	 */
	eSnailState    GetSnailState() const;
	
	void           Squash() override;
protected:
	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;

private:
	// The snail has different movement speeds depending on whether it is
	// sliding in its shell or walking
	float          m_speedMultiplier;

	// The snails are able to come out of their shells after some time
	float          m_inShellDuration;

	// The current state of the snail
	eSnailState    m_snailState;
};
