#include "Slime.h"

#include "../Fireball.h"

Slime::Slime(const int entityID, const Vector2& startingPosition, const bool canAvoidEdges) :
	Enemy(eEntityType::e_Slime,
	      entityID,
	      startingPosition,
	      {
		      static_cast<float>(constants::k_spriteSheetCellSize),
		      static_cast<float>(constants::k_spriteSheetCellSize) / 2.f
	      },
	      eDirection::e_Left,
	      canAvoidEdges)
{
	AddAnimation(animations::SLIME_WALK, true, 500.f);
	AddAnimation(animations::SLIME_SQUASHED, false, 2000.f);
	AddAnimation(animations::SLIME_SHELL_HIT, false, 1000.f);
}

void Slime::Update(const float deltaTime)
{
	if (m_currentEntityState == eEntityState::e_Alive)
	{
		if (m_isFalling)
		{
			m_velocity.y += 100 * constants::k_gravity * deltaTime;
		}
		Move(deltaTime);
	}

	PlayAnimation(deltaTime);
}

void Slime::CheckEntityCollisions(Entity& other)
{
	const auto& currentCollisionBoxes = GenerateCollisionBoxes();
	const auto& otherEntColBox        = other.GetCurrentCollisionBoxes();
	if (currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		if (other.GetEntityType() == eEntityType::e_Player)
		{
			// If player's feet are on the top of the enemy, squash it
			if (currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox))
			{
				// Don't squash if already dead
				if (m_currentEntityState != eEntityState::e_ProjectileHit)
				{
					m_currentEntityState = eEntityState::e_Dead;
					SetAnimationIndex(static_cast<int>(GetCurrentEntityState()));
				}
			}
		}
			// Check against fireballs
		else if (other.GetEntityType() == eEntityType::e_Fireball)
		{
			if (currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox) ||
			    currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
			    currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox) ||
			    currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
			{
				if (m_currentEntityState != eEntityState::e_ProjectileHit)
				{
					dynamic_cast<Fireball*>(&other)->Explode();
					m_currentEntityState = eEntityState::e_ProjectileHit;
					SetAnimationIndex(static_cast<int>(m_currentEntityState));
					PlaySFX("Entity_Fireball_Hit");
				}
			}
		}
	}
}

void Slime::Squash()
{
	m_currentEntityState = eEntityState::e_Dead;
	SetAnimationIndex(static_cast<int>(GetCurrentEntityState()));
}

CollisionBoxes Slime::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox(Vector2::ZERO, m_size);
	entityCollisionBox.Translate(m_position);

	auto topCollisionBox = CollisionBox({21.f, 5.f}, {43.f, 9.f});
	topCollisionBox.Translate(m_position);

	auto bottomCollisionBox = CollisionBox({6.f, 28.f}, {58.f, 32.f});
	bottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({6.f, 9.f}, {32.f, 28.f});
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({26, 0}),
		bottomCollisionBox
	};
}
