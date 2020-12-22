#include "State.h"

float State::DeltaTime(clock_t& gameClock)
{
	const clock_t programTickCount = clock() - gameClock;

	const float ticksToMilliseconds = 1000.f / CLOCKS_PER_SEC;

	return (programTickCount * ticksToMilliseconds) / 1000.f;
}
