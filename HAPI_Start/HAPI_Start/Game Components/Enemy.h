#pragma once
#include "Entity.h"

enum class EEnemyState {
	eAlive, eDead
};

class Enemy : public Entity {
public:
	explicit Enemy(Vector2 startingPosition, bool canAvoidEdges = false);
	void Update(float deltaTime) override;
	void Render(float playerOffset);
	bool CanAvoidEdges() const;
	void SetIsFalling(bool isFalling);
	void SetEnemyState(EEnemyState state);
	
private:
	EEnemyState m_currentEnemyState;
	bool m_canAvoidEdges;
	bool m_isFalling;
};
