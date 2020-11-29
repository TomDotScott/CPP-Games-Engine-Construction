#include "Coin.h"

Coin::Coin(const int entityID, const Vector2 position, const bool visible) :
	Entity(e_EntityType::eCoin, 
		entityID, 
		{ constants::k_spriteSheetCellWidth, constants::k_spriteSheetCellWidth },
		e_Direction::eNone,
		position,
		{ 0, 1 },
		Vector2::ZERO
	),
	m_lifeTime(0.f),
	m_isVisible(visible)
{
	// Spin Animation
	std::vector<std::string> spin{ "Coin_1", "Coin_2" , "Coin_3" , "Coin_4" , "Coin_5" , "Coin_6" , "Coin_7" , "Coin_8" };
	AddAnimation(spin, true, 100.f);
}

void Coin::Update(const float deltaTime)
{
	if (m_isVisible)
	{
		m_lifeTime += deltaTime / 1000.f;
		m_animator.Update(deltaTime);
		m_currentCollisionBoxes = GenerateCollisionBoxes();
	}
}

void Coin::CheckEntityCollisions(Entity& other)
{
	// Only collide with the player...
	if (other.GetEntityType() == e_EntityType::ePlayer)
	{
		if (m_currentCollisionBoxes.m_globalBounds.Overlapping(other.GetCurrentCollisionBoxes().m_globalBounds))
		{
			if (m_currentCollisionBoxes.m_topCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_bottomCollisionBox) ||
				m_currentCollisionBoxes.m_leftCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_rightCollisionBox) ||
				m_currentCollisionBoxes.m_rightCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_bottomCollisionBox) ||
				m_currentCollisionBoxes.m_bottomCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_topCollisionBox))
			{
				m_isVisible = false;
			}
		}
	}
}

CollisionBoxes Coin::GenerateCollisionBoxes()
{
	auto entityCollisionBox = BoundsRectangle({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = BoundsRectangle({ 12, 11 }, { 51, 16 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = BoundsRectangle({ 12, 16 }, { 32, 46 });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({20, 0}),
		topBottomCollisionBox.Translate({0, 35})
	};
}

void Coin::SetVisible(const bool visibility)
{
	if (visibility)
	{
		m_lifeTime = 0.f;
	}
	m_isVisible = visibility;
}

bool Coin::GetIsVisible() const
{
	return m_isVisible;
}

void Coin::Move(float deltaTime)
{
	// Slight hover
	m_position.y += cos(m_lifeTime);
}
