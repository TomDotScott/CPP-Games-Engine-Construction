#include "SoundDevice.h"

#include <cassert>
#include <HAPI_lib.h>

SoundDevice::SoundDevice()
{
	// Open the default device 
	m_alcDevice = alcOpenDevice(nullptr);

	assert(m_alcDevice);

	// Set up the OpenAL Context
	m_alcContext = alcCreateContext(m_alcDevice, nullptr);

	assert(m_alcContext);
	assert(alcMakeContextCurrent(m_alcContext));
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);

	alcDestroyContext(m_alcContext);

	alcCloseDevice(m_alcDevice);
}
