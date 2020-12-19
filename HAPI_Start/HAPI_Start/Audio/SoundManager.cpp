#include "SoundManager.h"
#include "HAPI_lib.h"

void SoundManager::Initialise()
{
	// Load SFX
	AddSoundEffect("Block_Break", "Res/SFX/Block_Break.wav");
	AddSoundEffect("Block_Bump", "Res/SFX/Block_Bump.wav");
	AddSoundEffect("Brick_Break", "Res/SFX/Brick_Break.wav");
	AddSoundEffect("Coin", "Res/SFX/Coin.wav");
	AddSoundEffect("Entity_Shell_Hit", "Res/SFX/Entity_Shell_Hit.wav");
	AddSoundEffect("Entity_Squash", "Res/SFX/Entity_Squash.wav");
	AddSoundEffect("Entity_Fireball_Hit", "Res/SFX/Entity_Fireball_Hit.wav");
	AddSoundEffect("Fireball_Explosion", "Res/SFX/Fireball_Explosion.wav");
	AddSoundEffect("Fireball_Shoot", "Res/SFX/Fireball_Shoot.wav");
	AddSoundEffect("Fireball_Wall_Hit", "Res/SFX/Fireball_Wall_Hit.wav");
	AddSoundEffect("Flag", "Res/SFX/Flag.wav");
	AddSoundEffect("Player_Dead", "Res/SFX/Player_Dead.wav");
	AddSoundEffect("Player_Jump", "Res/SFX/Player_Jump.wav");
	AddSoundEffect("Player_Power_Down", "Res/SFX/Player_Power_Down.wav");
	AddSoundEffect("Player_Power_Up", "Res/SFX/Player_Power_Up.wav");
	AddSoundEffect("Power_Up_Reveal", "Res/SFX/Power_Up_Reveal.wav");
	AddSoundEffect("Shell_Hit_Wall", "Res/SFX/Shell_Hit_Wall.wav");

	AddMusic("Level1", "Res/Music/Level1.wav");
	AddMusic("Level2", "Res/Music/Level2.wav");
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
