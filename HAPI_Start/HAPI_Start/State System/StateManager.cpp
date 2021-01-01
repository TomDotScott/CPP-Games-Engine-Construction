#include "StateManager.h"
#include "../Game Components/ControlsMenu.h"
#include "../Game Components/Game.h"
#include "../Game Components/GameOver.h"
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
		case eState::e_ControlsMenu:
			SetState(new ControlsMenu(HAPI.GetKeyboardData(), HAPI.GetControllerData(0)));
			break;
		case eState::e_Game:
			SetState(new Game(HAPI.GetKeyboardData(), HAPI.GetControllerData(0)));
			break;
		case eState::e_GameOver:
			SetState(new GameOver(HAPI.GetKeyboardData(), HAPI.GetControllerData(0)));
			break;
		default:
			break;
	}
}


void StateManager::SetState(State* state)
{
	if (m_state)
	{
		m_state->Unload(m_textureManager);
	}

	delete m_state;
	m_state = state;
	if (m_state)
	{
		m_state->Initialise(m_textureManager);
	}
	m_inputCoolDown = 0.f;
}

void StateManager::Update()
{
	if (m_state)
	{
		if (m_currentState == eState::e_Game)
		{
			m_state->Input();
			m_state->Update();
		}
		else
		{
			if (m_inputCoolDown >= 0.5f)
			{
				m_state->Input();
				m_state->Update();
			}
			else
			{
				m_inputCoolDown += State::DeltaTime(m_clock);
			}
		}
	}
	m_clock = clock();
}

void StateManager::Render()
{
	if (m_state)
	{
		m_state->Render(m_textureManager);
	}
}

eState StateManager::GetCurrentState() const
{
	return m_currentState;
}

StateManager::StateManager() :
	m_currentState(eState::e_MainMenu),
	m_state(nullptr),
	m_inputCoolDown(0.f),
	m_clock() {}

StateManager::~StateManager()
{
	delete m_state;
}
