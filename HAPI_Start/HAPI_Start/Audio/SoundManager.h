#pragma once
#include <unordered_map>
#include "Music.h"
#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

class SoundManager : public Singleton<SoundManager>
{
public:
	/**
	 * \brief Adds a sound effect to the SoundBuffer unordered_map
	 * \param sfxName The name of the sound effect, used as an identifier in the map
	 * \param fileName The file location of the sound effect file, relative to the working directory
	 * \return True if the sound effect was loaded and added to the buffer successfully
	 */
	bool AddSoundEffect(const std::string& sfxName, const std::string& fileName);
	/**
	 * \brief Removes a sound effect from the SoundBuffer unordered_map
	 * \param sfxName The identifier of the sound effect to be removed
	 * \return True if the sound effect was removed successfully
	 */
	bool RemoveSoundEffect(const std::string& sfxName);
	/**
	 * \brief Plays a given sound effect on the current SoundDevice
	 * \param sfxName The identifier of the sound effect to play
	 */
	void PlaySoundEffect(const std::string& sfxName);
	
	/**
	 * \brief Adds a Music object to the MusicBuffer unordered_map
	 * \param musicName The name of the music track, used as an identifier in the map
	 * \param fileName The file location of the music track file, relative to the working directory
	 * \return True if the music track was loaded and added to the buffer successfully
	 */
	bool AddMusic(const std::string& musicName, const std::string& fileName);
	/**
	 * \brief Removes a music track from the MusicBuffer unordered_map
	 * \param musicName The identifier of the music track to be removed
	 * \return True if the music track was removed and the memory was cleared successfully
	 */
	bool RemoveMusic(const std::string& musicName);
	/**
	 * \brief Plays a given music track on the current SoundDevice
	 * \param musicName The identifier of the music track to play
	 */
	void PlayMusic(const std::string& musicName);

	/**
	 * \brief Updates the streaming of the music file
	 */
	void UpdateMusicBufferStream();

	/**
	 * \brief Returns the name of the current music track that is playing
	 * \return The string identifier of the music track within the MusicBuffer unordered_map
	 */
	const std::string& GetCurrentMusicIdentifier() const;
	
	// Singleton, so non-copyable and non-movable
	SoundManager(const SoundManager& other)                = delete;
	SoundManager(SoundManager&& other) noexcept            = delete;
	SoundManager& operator=(const SoundManager& other)     = delete;
	SoundManager& operator=(SoundManager&& other) noexcept = delete;

private:
	friend class Singleton<SoundManager>;

	std::unordered_map<std::string, ALuint> m_soundBuffers;
	
	// The sound device is the speakers/headphones currently used by the user, identified automatically by OpenAL
	const SoundDevice&                      m_soundDevice; 
	SoundSource                             m_source;
	std::unordered_map<std::string, Music*> m_musicBuffer;
	std::string                             m_currentMusicTrackIdentifier;

	SoundManager();
	~SoundManager();
};
