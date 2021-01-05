#pragma once
#include "Alien.h"

class Player;

/**
 * \brief The boss is an alien that appears on the final level of the demo
 * It has basic AI behaviour, waiting for the player to approach, and jumping
 * and shooting when the player is within range to try and kill him
 */
class Boss final : public Alien
{
public:
	Boss(int entityID, const Vector2& startingPosition, const Player& player);

	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	void Render(TextureManager& textureManager, float playerOffset) override;

	/**
	 * \brief Gets whether the player is close enough to start the "battle"
	 * \return True if the player is close enough for the boss to start moving
	 */
	bool GetBattleStarted() const;
	
	void SetBattleStarted(bool battleStarted);

private:
	const Player& m_player;
	bool          m_battleStarted;

	CollisionBoxes GenerateCollisionBoxes() override;
	void           Move(float deltaTime) override;
	void           Shoot() override;
};
