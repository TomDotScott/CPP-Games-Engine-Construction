#pragma once
#include "Entity.h"

/**
 * \brief The current state of the lever, used to determine the
 * appropriate animation to play
 */
enum class eLeverState
{
	e_Static,
	e_Swinging
};

/**
 * \brief The lever is present near the end of the last level of the
 * demo. When it is touched by the player, it swings, triggering the bridge
 * to collapse, allowing for the player to kill the boss
 */
class Lever final : public Entity
{
public:
	Lever(int entityID, const Vector2& startPosition);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	/**
	 * \brief Gets the current lever state, as the lever is used as a trigger
	 * within the final level of the demo
	 * \return The current state (static or swinging) of the lever 
	 */
	eLeverState GetLeverState() const;

private:
	eLeverState m_leverState;

	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
