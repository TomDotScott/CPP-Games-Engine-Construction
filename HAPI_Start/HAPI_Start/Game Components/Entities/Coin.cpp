#include "Coin.h"

#include "Player.h"

Coin::Coin(const int      entityID,
           const Vector2& position,
           const bool     visible) :
	Entity(eEntityType::e_Coin,
	       entityID,
	       {constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize},
	       eDirection::e_None,
	       position,
	       {0, 1},
	       visible
	      )
{
	// Spin Animation
	AddAnimation(animations::COIN_SPIN);
}

void Coin::Update(const float deltaTime)
{
	if (m_activeState)
	{
		PlayAnimation(deltaTime);
	}
}

void Coin::CheckEntityCollisions(Entity& other)
{
	// Only collide with the player...
	if (other.GetEntityType() == eEntityType::e_Player)
	{
		const auto& currentCollisionBoxes = GenerateCollisionBoxes();
		if (currentCollisionBoxes.m_globalBounds.Overlapping(other.GetCurrentCollisionBoxes().m_globalBounds))
		{
			if (currentCollisionBoxes.m_topCollisionBox.
			                          Overlapping(other.GetCurrentCollisionBoxes().m_bottomCollisionBox) ||
			    currentCollisionBoxes.m_leftCollisionBox.
			                          Overlapping(other.GetCurrentCollisionBoxes().m_rightCollisionBox)  ||
			    currentCollisionBoxes.m_rightCollisionBox.
			                          Overlapping(other.GetCurrentCollisionBoxes().m_bottomCollisionBox) ||
			    currentCollisionBoxes.m_bottomCollisionBox.
			                          Overlapping(other.GetCurrentCollisionBoxes().m_topCollisionBox))
			{
				if (m_activeState)
				{
					PlaySFX("Coin");
					auto* player = dynamic_cast<Player*>(&other);
					player->AddCoin();
					player->AddToScore(200);
				}
				m_activeState = false;
			}
		}
	}
}

CollisionBoxes Coin::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({0, 0}, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({12, 11}, {51, 16});
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({12, 16}, {32, 46});
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({20, 0}),
		topBottomCollisionBox.Translate({0, 35})
	};
}

void Coin::Move(float deltaTime) {}
