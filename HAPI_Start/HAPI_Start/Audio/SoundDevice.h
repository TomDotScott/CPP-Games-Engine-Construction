#pragma once
#include <AL/alc.h>

class SoundDevice
{
public:
	static SoundDevice* GetInstance();
private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};
