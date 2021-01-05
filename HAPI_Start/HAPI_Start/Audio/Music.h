#pragma once
#include <AL/al.h>
#include <sndfile.h>
#include <string>
#include "../Utilities/Constants.h"

class Music
{
public:
	explicit Music(const std::string& filename);
	~Music();

	Music(const Music& other)                = default;
	Music(Music&& other) noexcept            = default;
	Music& operator=(const Music& other)     = default;
	Music& operator=(Music&& other) noexcept = default;

	
	/**
	 * \brief Plays the current music source
	 */
	void Play();
	
	/**
	 * \brief Stops the current music source
	 */
	void Stop() const;

	
	/**
	 * \brief Prevents the music source from under-running and keeps the file streaming from the music's file location
	 */
	void UpdateBufferStream() const;

private:
	ALuint   m_source;
	ALuint   m_buffers[constants::k_numBuffers];
	SNDFILE* m_musicFile;
	SF_INFO  m_musicFileInfo;
	short*   m_memBuff;
	ALenum   m_musicFormat;
};
