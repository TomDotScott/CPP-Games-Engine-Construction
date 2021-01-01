#include "State.h"

State::State(const HAPISPACE::HAPI_TKeyboardData& keyboardData, const HAPISPACE::HAPI_TControllerData& controllerData) :
	m_keyboardData(keyboardData),
	m_controllerData(controllerData) {}

float State::DeltaTime(clock_t& gameClock)
{
	const clock_t programTickCount = clock() - gameClock;

	const float ticksToMilliseconds = 1000.f / CLOCKS_PER_SEC;

	return (static_cast<float>(programTickCount) * ticksToMilliseconds) / 1000.f;
}

bool State::GetKey(const eKeyCode keyCode) const
{
	return m_keyboardData.scanCode[static_cast<int>(keyCode)] ? true : false;
}
