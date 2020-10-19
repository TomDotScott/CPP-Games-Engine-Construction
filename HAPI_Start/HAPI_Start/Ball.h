#pragma once
#include "Entity.h"

class Ball final : public Entity {
public:
	explicit Ball(const std::string& _filename, Vector2 _position, Vector2 _velocity, Entity* _p1, Entity* _p2);
	void Update() override;

private:
	Entity* m_player1;
	Entity* m_player2;
	
	void Reset();
};
