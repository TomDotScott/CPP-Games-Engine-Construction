#pragma once
#include <unordered_map>
#include "Music.h"
#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
public:
	void Initialise();
	
	bool AddSoundEffect(const std::string& sfxName, const std::string& fileName);
	void PlaySoundEffect(const std::string& sfxName);
	
	bool AddMusic(const std::string& musicName, const std::string& fileName);
	void PlayMusic(const std::string& musicName);
	
	// The music buffer needs to be constantly updated to
	// avoid under-running
	void UpdateMusicBufferStream();
	
private:
	std::unordered_map<std::string, ALuint> m_soundBuffers;
	const SoundDevice& m_soundDevice;
	SoundSource m_source;
	std::unordered_map<std::string, Music*> m_musicBuffer;
	std::string m_currentMusicTrackIdentifier;
	
	SoundManager();
	~SoundManager();
};

