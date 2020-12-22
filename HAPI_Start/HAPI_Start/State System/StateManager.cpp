#include "StateManager.h"

#include "../Game Components/Game.h"

void StateManager::OnCreate(const eState state)
{
	m_currentState = state;
	ChangeState(m_currentState);
}

void StateManager::ChangeState(const eState state)
{
	m_currentState = state;
	switch (state)
	{
	case eState::e_Game:
		SetState(new Game());
		break;
	default:
		break;
	}
}


void StateManager::SetState(State* state)
{
	delete m_state;
	m_state = state;
	if (m_state != nullptr)
	{
		m_state->Initialise();
	}
}

void StateManager::Input() const
{
	
}

void StateManager::Update() const
{
	if (m_state != nullptr)
	{
		m_state->Update();
	}
}

void StateManager::Render(TextureManager& textureManager) const
{
	if (m_state)
	{
		m_state->Render(textureManager);
	}
}

StateManager::~StateManager()
{
	delete m_state;
}