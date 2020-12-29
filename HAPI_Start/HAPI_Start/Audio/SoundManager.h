#pragma once
#include <unordered_map>
#include "Music.h"
#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

class SoundManager : public Singleton<SoundManager>
{
public:	
	bool AddSoundEffect(const std::string& sfxName, const std::string& fileName);
	bool RemoveSoundEffect(const std::string& sfxName);
	void PlaySoundEffect(const std::string& sfxName);
	
	bool AddMusic(const std::string& musicName, const std::string& fileName);
	bool RemoveMusic(const std::string& musicName);
	void PlayMusic(const std::string& musicName);
	
	// The music buffer needs to be constantly updated to
	// avoid under-running
	void UpdateMusicBufferStream();

	// Singleton, so non-copyable and non-movable
	SoundManager(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) noexcept = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& other) noexcept = delete;
	
private:
	friend class Singleton<SoundManager>;
	std::unordered_map<std::string, ALuint> m_soundBuffers;
	const SoundDevice& m_soundDevice;
	SoundSource m_source;
	std::unordered_map<std::string, Music*> m_musicBuffer;
	std::string m_currentMusicTrackIdentifier;
	
	SoundManager();
	~SoundManager();
};

