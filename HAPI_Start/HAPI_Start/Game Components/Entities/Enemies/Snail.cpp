#include "Snail.h"
Snail::Snail(const int entityID, const Vector2 startingPos) :
	Enemy(eEntityType::e_Snail,
		entityID,
		startingPos,
		Vector2(constants::k_spriteSheetCellSize, 36),
		eDirection::e_Left,
		true),
	m_speedMultiplier(0.5f),
	m_inShellDuration(0.f),
	m_snailState(eSnailState::e_Walking)
{
	AddAnimation(animations::SNAIL_WALK, true, 500.f);

	AddAnimation(animations::SNAIL_IN_SHELL, true);

	// Sliding
	AddAnimation(animations::SNAIL_IN_SHELL, true);

	AddAnimation(animations::SNAIL_SHELL_HIT, false, 1000.f);

	AddAnimation(animations::SNAIL_SHELL_CRACK, false, 50.f);
}

void Snail::Update(const float deltaTime)
{
	switch (m_snailState)
	{
	case eSnailState::e_Walking:;
	case eSnailState::e_Sliding:
		Move(deltaTime);
		break;
	case eSnailState::e_Squashed:
		if (m_inShellDuration < 5000.f)
		{
			m_inShellDuration += deltaTime;
		} else
		{
			m_snailState = eSnailState::e_Walking;
			m_canAvoidEdges = true;
			SetAnimationIndex(static_cast<int>(m_snailState));
		}
		break;
	default:;
	}

	PlayAnimation(deltaTime);
}

void Snail::CheckEntityCollisions(Entity& other)
{
	const auto& currentCollisionBoxes = GenerateCollisionBoxes();
	const auto& otherEntColBox = other.GetCurrentCollisionBoxes();
	if (currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		if (other.GetEntityType() == eEntityType::e_Player)
		{
			if (currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox))
			{
				// If not in shell...
				switch (m_snailState)
				{
				case eSnailState::e_Walking:
					m_inShellDuration = 0.f;
					m_snailState = eSnailState::e_Squashed;
					break;
				case eSnailState::e_Squashed:
					m_snailState = eSnailState::e_Sliding;
					m_speedMultiplier = 4.f;
					m_canAvoidEdges = false;
					// Work out to go left or right
					if (otherEntColBox.m_bottomCollisionBox.TOP_LEFT.x < currentCollisionBoxes.m_rightCollisionBox.TOP_LEFT.x)
					{
						SetDirection(eDirection::e_Right);
					} else
					{
						SetDirection(eDirection::e_Left);
					}
					break;
				case eSnailState::e_Sliding:
					m_snailState = eSnailState::e_Squashed;
					m_speedMultiplier = 0.5f;
					break;
				default:;
				}
				SetAnimationIndex(static_cast<int>(m_snailState));
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
				if (m_snailState != eSnailState::e_ProjectileHit)
				{
					m_snailState = eSnailState::e_ProjectileHit;
					SetAnimationIndex(static_cast<int>(m_snailState));

					m_currentEntityState = eEntityState::e_Dead;
				}
			}
		}
	}
}

void Snail::CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes)
{
	const auto& currentCollisionBoxes = GenerateCollisionBoxes();
	if (currentCollisionBoxes.m_globalBounds.Overlapping(snailShellCollisionBoxes.m_globalBounds))
	{
		if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(snailShellCollisionBoxes.m_rightCollisionBox) ||
			currentCollisionBoxes.m_rightCollisionBox.Overlapping(snailShellCollisionBoxes.m_leftCollisionBox))
		{
			m_snailState = eSnailState::e_ProjectileHit;
			SetAnimationIndex(static_cast<int>(m_snailState));

			m_currentEntityState = eEntityState::e_Dead;
		}
	}
}

eSnailState Snail::GetSnailState() const
{
	return m_snailState;
}

void Snail::Squash()
{
	m_snailState = eSnailState::e_Cracking;
	SetAnimationIndex(static_cast<int>(m_snailState));

	m_currentEntityState = eEntityState::e_Dead;
}

void Snail::Move(const float deltaTime)
{
	if (m_isFalling)
	{
		m_velocity.y += constants::k_gravity * deltaTime;
	}
	m_position = m_position + m_velocity * (deltaTime / 1000.f) * m_speedMultiplier;
}

CollisionBoxes Snail::GenerateCollisionBoxes()
{
	auto entityCollisionBox = BoundsRectangle(Vector2::ZERO, m_size);
	entityCollisionBox.Translate(m_position);

	auto topCollisionBox = BoundsRectangle({ 22.f, 0.f }, { 52.f, 7.f });
	topCollisionBox.Translate(m_position);

	auto bottomCollisionBox = BoundsRectangle({ 7.f, 30.f }, { 75.f, 36.f });
	bottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = BoundsRectangle({ 7.f, 7.f }, { 32.f, 30.f });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({25.f, 0.f}),
		bottomCollisionBox
	};
}
