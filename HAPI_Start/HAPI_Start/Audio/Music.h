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

	void Play();
	void Stop() const;
	void UpdateBufferStream() const;

private:
	ALuint   m_source;
	ALuint   m_buffers[constants::k_numBuffers];
	SNDFILE* m_musicFile;
	SF_INFO  m_musicFileInfo;
	short*   m_memBuff;
	ALenum   m_musicFormat;
};
