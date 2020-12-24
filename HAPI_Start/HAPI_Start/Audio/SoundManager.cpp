#include "SoundManager.h"
#include "HAPI_lib.h"

void SoundManager::Initialise()
{
	
}

bool SoundManager::AddSoundEffect(const std::string& sfxName, const std::string& fileName)
{
	// Check the name doesn't already exist in the map
	if (!(m_soundBuffers.find(sfxName) == m_soundBuffers.end()))
	{
		HAPI.UserMessage("A sound effect with the name " + sfxName + " already exists.", "Error Occured");
		return false;
	} else
	{
		// Create a buffer
		const auto sfxBuffer = SoundBuffer::GetInstance().AddSoundEffect(fileName.c_str());

		// Add the buffer to the map
		m_soundBuffers[sfxName] = sfxBuffer;
	}
	return true;
}

void SoundManager::PlaySoundEffect(const std::string& sfxName)
{
	m_source.Play(m_soundBuffers[sfxName]);
}

bool SoundManager::AddMusic(const std::string& musicName, const std::string& fileName)
{
	// Check the name doesn't already exist in the map
	if (!(m_soundBuffers.find(musicName) == m_soundBuffers.end()))
	{
		HAPI.UserMessage("A music track with the name " + musicName + " already exists.", "Error Occured");
		return false;
	} else
	{
		auto* newMusic = new Music(fileName);

		m_musicBuffer[musicName] = newMusic;
	}

	return true;
}

void SoundManager::PlayMusic(const std::string& musicName)
{
	m_musicBuffer[musicName]->Play();
	m_currentMusicTrackIdentifier = musicName;
}

void SoundManager::UpdateMusicBufferStream()
{
	m_musicBuffer[m_currentMusicTrackIdentifier]->UpdateBufferStream();
}

SoundManager::SoundManager() :
	m_soundDevice(SoundDevice::GetInstance())
{
}

SoundManager::~SoundManager()
{
	// Remove the dynamically allocated music tracks
	for(auto& music : m_musicBuffer)
	{
		delete music.second;
	}
}
