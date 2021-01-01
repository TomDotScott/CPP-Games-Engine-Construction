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

	void SetActive(bool active);
	bool GetIsActive() const;

	bool GetBattleStarted() const;

private:
	const Player& m_player;
	bool          m_active;
	bool          m_battleStarted;

	CollisionBoxes GenerateCollisionBoxes() override;
	void           Move(float deltaTime) override;
	void           Shoot() override;
};
