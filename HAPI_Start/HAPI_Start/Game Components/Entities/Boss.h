#pragma once
#include "Alien.h"

class Player;

class Boss final : public Alien
{
public:
	Boss(int entityID, const Vector2& startingPosition, const Player& player);

	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	void Render(TextureManager& textureManager, float playerOffset) override;

	bool GetBattleStarted() const;
	void SetBattleStarted(bool battleStarted);

private:
	const Player& m_player;
	bool          m_battleStarted;

	CollisionBoxes GenerateCollisionBoxes() override;
	void           Move(float deltaTime) override;
	void           Shoot() override;
};
