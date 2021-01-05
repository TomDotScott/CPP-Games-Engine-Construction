#pragma once
#include <string>
#include <vector>

/**
 * \brief The current state of the animation
 */
enum class eAnimationState
{
	e_Started, e_Paused, e_Ended
};

/**
 * \brief The AnimationPlayer class holds the information required
 * to play animations within the engine
 */
class AnimationPlayer
{
public:
	/**
	 * \brief Constructs an Animation object
	 * \param frames the string identifiers for each frame
	 * of the animation
	 * \param looping True if the animation should loop upon completion
	 * \param duration The time difference between each frame of the animation
	 */
	explicit        AnimationPlayer(std::vector<std::string> frames, bool looping = true, float duration = 100.f);
	/**
	 * \brief Plays (transitions the frame when the correct amount of
	 * time has passed) the animation
	 * \param deltaTime The time difference between Game Update() calls,
	 * used for platform independence and also to allow for the correct
	 * duration of animation frames
	 */
	void            Play(float deltaTime);
	/**
	 * \brief Resets the animation
	 */
	void            Reset();
	
	/**
	 * \brief Gets the current frame of the animation
	 * \return The frame number of the animation
	 */
	unsigned short  GetCurrentFrame() const;
	/**
	 * \brief Gets the current frame identifier for the current
	 * frame of the animation
	 * \return The string name identifier for the current animation frame
	 */
	std::string     GetCurrentFrameIdentifier() const;
	/**
	 * \brief Gets the current animation state (playing, paused or stopped)
	 * of the animation
	 * \return The current animation state
	 */
	eAnimationState GetCurrentAnimationState() const;

private:
	std::vector<std::string> m_frames;
	unsigned short m_currentFrame;
	float m_duration;
	float m_playTime;
	bool m_looping;
	eAnimationState m_state;
	
};