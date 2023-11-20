#include "SoundController.h"
#include "Internal.h"
namespace AASL 
{
	extern SoundInternal sounds[max_sounds_playing];
	SoundResource SoundController::soundResource() 
	{
		return sounds[id % max_sounds_playing].resource;
	}

	SoundOptions& SoundController::options() 
	{
		return sounds[id % max_sounds_playing].options;
	}

	float SoundController::getTimeSeconds() 
	{
		return (float)sounds[id % max_sounds_playing].currentSample / (float)sounds[id % max_sounds_playing].resource.audio().getSampleRate();
	}

	void SoundController::setTimeSeconds(float seconds) 
	{
		sounds[id % max_sounds_playing].currentSample = sounds[id % max_sounds_playing].resource.audio().getSampleRate() * seconds;
	}

	void SoundController::pause() 
	{
		sounds[id % max_sounds_playing].paused = true;
	}

	void SoundController::resume() 
	{
		sounds[id % max_sounds_playing].paused = false;
	}

	int& SoundController::sample() 
	{
		return sounds[id % max_sounds_playing].currentSample;
	}

	bool SoundController::isPlaying() 
	{
		return !sounds[id % max_sounds_playing].paused && sounds[id % max_sounds_playing].alive;
	}

	bool SoundController::isPaused() 
	{
		return sounds[id % max_sounds_playing].paused && sounds[id % max_sounds_playing].alive;
	}

	bool SoundController::isAlive() 
	{
		return sounds[id % max_sounds_playing].alive && sounds[id % max_sounds_playing].id == this->id;
	}

	void SoundController::restart() 
	{
		return this->setTimeSeconds(sounds[id % max_sounds_playing].options.startTime);
	}

	void SoundController::rewindTime(float seconds) 
	{
		this->setTimeSeconds(this->getTimeSeconds() - seconds);
	}

	void SoundController::forwardTime(float seconds) 
	{
		this->setTimeSeconds(this->getTimeSeconds() + seconds);
	}
}