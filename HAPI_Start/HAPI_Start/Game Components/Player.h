#pragma once
#include "Entity.h"

class Player final : public Entity {
public:
	Player(const std::string& textureFileName, const std::string& textureIdentifier, Vector2 startingPosition);
	Player(const std::string& spriteSheetIdentifier, int spriteSheetLocation, Vector2 startingPosition);

	void Update(float deltaTime) override;
	void Render() override;
	void SetDirection(Vector2 direction);

private:
	Vector2 m_currentDirection;
	
	void Move(float deltaTime);
};
