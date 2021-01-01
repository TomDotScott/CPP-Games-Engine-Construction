#include "SoundDevice.h"

#include <cassert>
#include <HAPI_lib.h>

SoundDevice::SoundDevice()
{
	// Open the default device 
	m_alcDevice = alcOpenDevice(nullptr);

	assert(m_ALCDevice);

	// Set up the OpenAL Context
	m_alcContext = alcCreateContext(m_alcDevice, nullptr);

	assert(p_ALCContext);
	assert(alcMakeContextCurrent(p_ALCContext));
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);

	alcDestroyContext(m_alcContext);

	alcCloseDevice(m_alcDevice);
}
