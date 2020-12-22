#include "StateManager.h"

#include "../Game Components/Game.h"
#include "../Game Components/MainMenu.h"

void StateManager::OnCreate(const eState state)
{
	m_textureManager.Initialise(HAPI.GetScreenPointer());
	m_currentState = state;
	ChangeState(m_currentState);
}

void StateManager::ChangeState(const eState state)
{
	m_currentState = state;
	switch (state)
	{
	case eState::e_MainMenu:
		SetState(new MainMenu(HAPI.GetKeyboardData(), HAPI.GetControllerData(0)));
		break;

	case eState::e_Game:
		SetState(new Game(HAPI.GetKeyboardData(), HAPI.GetControllerData(0)));
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
		m_state->Initialise(m_textureManager);
	}
}

void StateManager::Update() const
{
	if (m_state != nullptr)
	{
		m_state->Input();
		m_state->Update();
	}
}

void StateManager::Render()
{
	if (m_state)
	{
		m_state->Render(m_textureManager);
	}
}

StateManager::StateManager() :
	m_currentState(eState::e_MainMenu),
	m_state(nullptr)
{
}

StateManager::~StateManager()
{
	delete m_state;
}
