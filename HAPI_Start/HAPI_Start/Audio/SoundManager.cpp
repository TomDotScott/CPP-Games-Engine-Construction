#include "SoundManager.h"

#include "HAPI_lib.h"

bool SoundManager::AddSoundEffect(const std::string& sfxName, const std::string& filename)
{
	// Check the name doesn't already exist in the map
	if (!(m_soundBuffers.find(sfxName) == m_soundBuffers.end()))
	{
		HAPI.UserMessage("A sound effect with the name " + sfxName + " already exists.", "Error Occured");
		return false;
	} else
	{
		// Create a buffer
		const auto sfxBuffer = SoundBuffer::GetInstance().AddSoundEffect("Data/Music.wav");

		// Add the buffer to the map
		m_soundBuffers[sfxName] = sfxBuffer;
	}
	return true;
}

void SoundManager::PlaySoundEffect(const std::string& sfxName)
{
	m_source.Play(m_soundBuffers[sfxName]);
}

SoundManager::SoundManager() :
	m_soundDevice(SoundDevice::GetInstance())
{
}
