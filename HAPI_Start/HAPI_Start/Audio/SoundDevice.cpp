#include "SoundDevice.h"

#include "HAPI_lib.h"

SoundDevice::SoundDevice()
{
	// Open the default device 
	p_ALCDevice = alcOpenDevice(nullptr);

	if (!p_ALCDevice)
	{
		HAPI.UserMessage("Failed to get sound device", "An error occured");
		throw("Failed to get sound device");
	}

	// Set up the OpenAL Context
	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
	{
		HAPI.UserMessage("Failed to set sound context", "An error occured");
		throw("Failed to set sound context");
	}

	if (!alcMakeContextCurrent(p_ALCContext))
	{
		HAPI.UserMessage("Failed to make context current", "An error occured");
		throw("Failed to make context current");
	}

	const ALCchar* name{ nullptr };
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	}
	if (!name || alcGetError(p_ALCDevice) != ALC_NO_ERROR)
	{
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	}
}

SoundDevice::~SoundDevice()
{
	if(!alcMakeContextCurrent(NULL))
	{
		HAPI.UserMessage("Failed to set context to nullptr", "An error occured");
		throw("Failed to set context to nullptr");
	}

	alcDestroyContext(p_ALCContext);

	if(p_ALCContext)
	{
		HAPI.UserMessage("Failed to unset during close", "An error occured");
		throw("Failed to unset during close");
	}

	if(!alcCloseDevice(p_ALCDevice))
	{
		HAPI.UserMessage("Failed to close sound device", "An error occured");
		throw("Failed to close sound device");
	}
}
