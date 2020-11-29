#include "Snail.h"
Snail::Snail(const Vector2 startingPos) :
	Enemy(startingPos,
		Vector2(constants::k_spriteSheetCellWidth, 36),
		e_Direction::eLeft,
		true),
	m_speedMultiplier(0.5f),
	m_inShellDuration(0.f),
	m_snailState(e_SnailState::eWalking)
{

	m_entityType = e_EntityType::eSnail;
	std::vector<std::string> walk{ "Snail_1", "Snail_2" };
	AddAnimation(walk, true, 500.f);
	std::vector<std::string> squashed{ "Snail_Shell" };
	AddAnimation(squashed, true);
	// For sliding
	AddAnimation(squashed, true);
	std::vector<std::string> shellHit{ "Snail_Snail_Shell_Hit" };
	AddAnimation(shellHit, false, 1000.f);
	std::vector<std::string> shellCrack{ "Snail_Shell", "Snail_Shell_Crack_1", "Snail_Shell_Crack_2" , "Snail_Shell_Crack_3" };
	AddAnimation(shellCrack, false, 50.f);
}

void Snail::Update(const float deltaTime)
{
	if (m_snailState == e_SnailState::eWalking || m_snailState == e_SnailState::eSliding)
	{
		Move(deltaTime);
	} else
	{
		if (m_inShellDuration < 5000.f)
		{
			m_inShellDuration += deltaTime;
		} else
		{
			m_snailState = e_SnailState::eWalking;
			m_canAvoidEdges = true;
			m_animator.SetAnimationIndex(static_cast<int>(m_snailState));
		}
	}
	m_animator.Update(deltaTime);
	m_currentCollisionBoxes = GenerateCollisionBoxes();
}

void Snail::CheckEntityCollisions(Entity* other)
{
	const auto otherEntColBox = other->GetCurrentCollisionBoxes();
	if (m_currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		if (other->GetEntityType() == e_EntityType::ePlayer)
		{
			if (m_currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox))
			{
				// If not in shell...
				switch (m_snailState)
				{
				case e_SnailState::eWalking:
					m_inShellDuration = 0.f;
					m_snailState = e_SnailState::eSquashed;
					break;
				case e_SnailState::eSquashed:
					m_snailState = e_SnailState::eSliding;
					m_speedMultiplier = 4.f;
					m_canAvoidEdges = false;
					// Work out to go left or right
					if (otherEntColBox.m_bottomCollisionBox.TOP_LEFT.x < m_currentCollisionBoxes.m_rightCollisionBox.TOP_LEFT.x)
					{
						SetDirection(e_Direction::eRight);
					} else
					{
						SetDirection(e_Direction::eLeft);
					}
					break;
				case e_SnailState::eSliding:
					m_snailState = e_SnailState::eSquashed;
					m_speedMultiplier = 0.5f;
					break;
				default:;
				}
				m_animator.SetAnimationIndex(static_cast<int>(m_snailState));
			}
		}
	}
}

void Snail::CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes)
{
	if (m_currentCollisionBoxes.m_globalBounds.Overlapping(snailShellCollisionBoxes.m_globalBounds))
	{
		if (m_currentCollisionBoxes.m_leftCollisionBox.Overlapping(snailShellCollisionBoxes.m_rightCollisionBox) ||
			m_currentCollisionBoxes.m_rightCollisionBox.Overlapping(snailShellCollisionBoxes.m_leftCollisionBox))
		{
			m_snailState = e_SnailState::eShellHit;
			m_animator.SetAnimationIndex(static_cast<int>(m_snailState));
		}
	}
}

e_SnailState Snail::GetSnailState() const
{
	return m_snailState;
}

void Snail::Squash()
{
	m_snailState = e_SnailState::eCracking;
	m_currentEntityState = e_EntityState::eDead;
	m_animator.SetAnimationIndex(static_cast<int>(m_snailState));
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
