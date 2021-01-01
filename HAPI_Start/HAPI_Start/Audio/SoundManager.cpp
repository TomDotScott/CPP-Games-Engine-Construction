#include "SoundManager.h"
#include "HAPI_lib.h"

bool SoundManager::AddSoundEffect(const std::string& sfxName, const std::string& fileName)
{
	// Check the name doesn't already exist in the map
	if (!(m_soundBuffers.find(sfxName) == m_soundBuffers.end()))
	{
		HAPI.UserMessage("A sound effect with the name " + sfxName + " already exists.", "Error Occurred");
		return false;
	}
	// Create a buffer
	const auto sfxBuffer = SoundBuffer::GetInstance().AddSoundEffect(fileName.c_str());

	// Add the buffer to the map
	m_soundBuffers[sfxName] = sfxBuffer;
	return true;
}

bool SoundManager::RemoveSoundEffect(const std::string& sfxName)
{
	if (m_soundBuffers.find(sfxName) == m_soundBuffers.end())
	{
		HAPI.UserMessage("The sound effect: " + sfxName + " doesn't exist. It may have already been removed.",
		                 "Error Occurred");
		return false;
	}

	// Remove the entry in the map
	m_soundBuffers.erase(sfxName);
	return true;
}

void SoundManager::PlaySoundEffect(const std::string& sfxName)
{
	if (m_soundBuffers.find(sfxName) == m_soundBuffers.end())
	{
		HAPI.UserMessage("The sound effect: " + sfxName + " doesn't exist. It may have been removed.",
		                 "Error Occurred");
		return;
	}
	m_source.Play(m_soundBuffers[sfxName]);
}

bool SoundManager::AddMusic(const std::string& musicName, const std::string& fileName)
{
	// Check the name doesn't already exist in the map
	if (!(m_soundBuffers.find(musicName) == m_soundBuffers.end()))
	{
		HAPI.UserMessage("A music track with the name " + musicName + " already exists.", "Error Occured");
		return false;
	}
	auto* newMusic = new Music(fileName);

	m_musicBuffer[musicName] = newMusic;

	return true;
}

bool SoundManager::RemoveMusic(const std::string& musicName)
{
	if (m_musicBuffer.find(musicName) == m_musicBuffer.end())
	{
		HAPI.UserMessage("The music track: " + musicName + " doesn't exist. It may have already been removed.",
		                 "Error Occurred");
		return false;
	}
	// Clear the data stored
	delete m_musicBuffer[musicName];

	// Remove the entry in the map
	m_musicBuffer.erase(musicName);

	return true;
}

void SoundManager::PlayMusic(const std::string& musicName)
{
	m_musicBuffer[musicName]->Play();
	m_currentMusicTrackIdentifier = musicName;
}

void SoundManager::UpdateMusicBufferStream()
{
	if (m_musicBuffer[m_currentMusicTrackIdentifier])
	{
		m_musicBuffer[m_currentMusicTrackIdentifier]->UpdateBufferStream();
	}
}

SoundManager::SoundManager() : m_soundDevice(SoundDevice::GetInstance()) {}

SoundManager::~SoundManager()
{
	// Remove the dynamically allocated music tracks
	for (auto& music : m_musicBuffer)
	{
		delete music.second;
	}
}
