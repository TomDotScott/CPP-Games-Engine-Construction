#pragma once
#include <AL/alc.h>

#include "../Design Patterns/Singleton.h"

class SoundDevice : public Singleton<SoundDevice>
{
private:
	friend class Singleton<SoundDevice>;

public:
	SoundDevice(const SoundDevice& other)                = delete;
	SoundDevice(SoundDevice&& other) noexcept            = delete;
	SoundDevice& operator=(const SoundDevice& other)     = delete;
	SoundDevice& operator=(SoundDevice&& other) noexcept = delete;
private:
	ALCdevice*  m_alcDevice;
	ALCcontext* m_alcContext;

	SoundDevice();
	~SoundDevice();
};
