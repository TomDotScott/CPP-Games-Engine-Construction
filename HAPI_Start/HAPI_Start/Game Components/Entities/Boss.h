#pragma once
#include "Alien.h"

class Player;

class Boss : public Alien 
{
public:
	Boss(int entityID, Vector2 startingPosition, const Player& player);
	
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	void Render(TextureManager& textureManager, float playerOffset) override;

	void SetVisible(bool visible);
	bool GetIsVisible() const;

	bool GetBattleStarted() const;
	
private:
	const Player& m_player;
	bool m_visible;
	bool m_battleStarted;
	
	CollisionBoxes GenerateCollisionBoxes() override;
	void Move(float deltaTime) override;
	void Shoot() override;
};

