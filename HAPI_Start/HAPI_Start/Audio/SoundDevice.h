#pragma once
#include <AL/alc.h>

#include "../Design Patterns/Singleton.h"

class SoundDevice : public Singleton<SoundDevice>
{
	friend class Singleton<SoundDevice>;
private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};