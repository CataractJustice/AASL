#include "AASL.h"
#include <map>
#include <string>
#include <thread>
#include <alsa/asoundlib.h>
#include <alsa/control.h>
#include <fstream>
#include <chrono>
#include <string>
#include "Internal.h"
namespace AASL 
{
	

	int soundResourceIdCounter = 0;

	SoundResourceInternal soundResources[max_sound_resources];

	SoundInternal sounds[max_sounds_playing];
	int soundIdCounter = 0;

	std::map<std::string, SoundResource> cachedSoundResouces;
	std::map<std::string, int> outputDeviceHandles = std::map<std::string, int>();
	std::pair<OutputDevice, OutputDeviceInternal> outputDevices[max_output_devices];
	int outputDeviceIdCounter = 0;
	std::thread AASLThread;

	#define DeaultPCMDevice "default"

	//Mixer thread loop
	void Update() 
	{
		while(true) 
		{
			OutputDeviceInternal& output = outputDevices[0].second;
			memset(output.buffer, 0, output.bufferSize);

			bool anyAlive = false;
			for(int i = 0; i < max_sounds_playing; i++) 
			{
				SoundInternal& sound = sounds[i];
				if(sound.paused) continue;

				float soundEndTime = sound.options.endTime > sound.options.startTime ? sound.options.endTime : sound.resource.audio().getLengthInSeconds();
				
				if(sound.alive) 
				{	
					float currentTime = (float)sound.currentSample / (float)sound.resource.audio().getSampleRate();
					if(sound.options.playSpeed >= 0.0f)
					{
						if((currentTime > soundEndTime) || sound.currentSample >= sound.resource.audio().samples[0].size() - 1)
						{
							if(!sound.options.loop && !sound.options.keepAlive) 
							{
								sound.alive = false;
								continue;
							}
							else if(sound.options.loop) 
							{
								sound.controller.setTimeSeconds(sound.options.startTime);
							}
							else if(sound.options.keepAlive) 
							{
								sound.controller.pause();
							}
						}
					}
					else 
					{
						if(currentTime < sound.options.startTime || sound.currentSample < 0)
						{
							if(!sound.options.loop && !sound.options.keepAlive) 
							{
								sound.alive = false;
								continue;
							}
							else if(sound.options.loop) 
							{
								sound.controller.setTimeSeconds(soundEndTime);
							}
							else if(sound.options.keepAlive) 
							{
								sound.controller.pause();
							}
						}
					}

					anyAlive = true;
					for(int i = 0; i < output.bufferLength; i++) 
					{
						//to-do: figure out better channels managment for a case with a single output channel and multiple audio file channels
						int channel = i % output.channels;
						int sampleIndex = sound.currentSample + i / output.channels * sound.options.playSpeed;
						
						if(sound.options.playSpeed >= 0.0f) 
						{
							//check if we've reached the end of audio
							if(sampleIndex >= sound.resource.audio().samples[0].size() - 1)
							{
								if(sound.options.loop) 
								{
									sound.controller.setTimeSeconds(sound.options.startTime);
									sampleIndex = sound.currentSample + i / output.channels * sound.options.playSpeed;
								}
								else 
								{
									break;
								}
							}
						}
						else
						{
							//check if we've reached the end of audio
							if(sampleIndex < 0)
							{
								if(sound.options.loop) 
								{
									sound.controller.setTimeSeconds(sound.options.endTime);
									sampleIndex = sound.currentSample + i / output.channels * sound.options.playSpeed;
									sampleIndex = (sampleIndex < sound.resource.audio().samples[0].size()) ? sampleIndex : sound.resource.audio().samples[0].size(); 
								}
								else 
								{
									break;
								}
							}
						}


						float sample = sound.resource.audio().samples[0][sampleIndex];
						//to-do: apply more options here
						sample *= sound.options.volume;
						output.buffer[i] += sample;
					}
					//advance sound timeline
					sound.currentSample += output.bufferLength * sound.options.playSpeed;
				}
				
			}
			//output sound
			snd_pcm_sframes_t delayFrames;
			snd_pcm_delay(output.pcmHandle, &delayFrames);
			if(anyAlive) 
			{
				auto writeTime = std::chrono::high_resolution_clock::now();
				if (snd_pcm_writei(output.pcmHandle, output.buffer, output.frames) == -EPIPE) {
					printf("XRUN.\n");
					snd_pcm_prepare(output.pcmHandle);
				}

				if(delayFrames > 3000) 
				{
					std::this_thread::sleep_until(writeTime + std::chrono::microseconds(output.periodTime));
				}
			}
		}
	}

	//
	void Init() 
	{
		GetOutputDevice(DeaultPCMDevice);
		AASLThread = std::thread(Update);
	}

	//
	SoundController StartSound(const char* soundFilePath, SoundOptions options) 
	{
		//check whenever we've not yet loaded sound file in given path
		auto it = cachedSoundResouces.find(soundFilePath);
		if(it == cachedSoundResouces.cend()) 
		{
			//save sound file in given path for later use
			SoundResource sr = LoadSound(soundFilePath);
			cachedSoundResouces[soundFilePath] = sr;
			return StartSound(sr, options);
		}
		return StartSound(it->second, options);
	}

	//
	SoundController StartSound(SoundResource sound, SoundOptions options) 
	{
		for(int i = 0; i < max_sounds_playing; i++) 
		{
			SoundInternal& si = sounds[soundIdCounter % max_sounds_playing];
			if(!si.alive) 
			{
				si.alive = true;
				si.id = soundIdCounter;
				si.resource = sound;
				si.options = options;
				si.controller.id = soundIdCounter % max_sounds_playing;
				si.currentSample = 0;
				si.controller.setTimeSeconds(options.startTime);
				return si.controller;
			}
			soundIdCounter++;
		}
		//to-do: add too many sounds playing error handling

		SoundController sc;
		sc.id = -1;
		return sc;
	}

	//
	SoundResource LoadSound(const char* soundFilePath) 
	{
		SoundResourceInternal& sri = soundResources[soundResourceIdCounter % max_sound_resources];
		sri.audio.load(soundFilePath);

		SoundResource sr;
		sr.id = soundResourceIdCounter;

		soundResourceIdCounter++;

		return sr;
	}

	//
	OutputDevice MakeOutputDevice(const char* handle) 
	{
		OutputDeviceInternal odi;
		OutputDevice od;
		odi.channels = 1;

		unsigned int err = snd_pcm_open(&odi.pcmHandle, DeaultPCMDevice, SND_PCM_STREAM_PLAYBACK, 0);
		odi.sampleRate = 48000;


		snd_pcm_hw_params_alloca(&odi.params);
		snd_pcm_hw_params_any(odi.pcmHandle, odi.params);

		err = snd_pcm_hw_params_set_access(odi.pcmHandle, odi.params, SND_PCM_ACCESS_RW_INTERLEAVED);
		if(err) printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(err));
		err = snd_pcm_hw_params_set_format(odi.pcmHandle, odi.params, SND_PCM_FORMAT_FLOAT);
		if(err) printf("ERROR: Can't set format. %s\n", snd_strerror(err));
		err = snd_pcm_hw_params_set_channels(odi.pcmHandle, odi.params, odi.channels);
		if(err) throw "Unnable to set channels";
		err = snd_pcm_hw_params_set_rate_near(odi.pcmHandle, odi.params, &odi.sampleRate, 0);
		if(err) throw "Unnable to set rate";
		err = snd_pcm_hw_params(odi.pcmHandle, odi.params);
		if(err) throw "Unnable to set params";

		snd_pcm_hw_params_get_period_size(odi.params, &odi.frames, 0);

		odi.bufferLength = odi.frames * odi.channels;
		odi.bufferSize = odi.bufferLength * sizeof(float);
		odi.buffer = new float[odi.bufferLength];

		snd_pcm_hw_params_get_period_time(odi.params, &odi.periodTime, NULL);
		
		//to-do: add error handling for unsuccessfull pcm open

		//to-do: add error handling for too many output devices

		od.id = outputDeviceIdCounter;
		outputDevices[outputDeviceIdCounter % max_output_devices] = std::pair<OutputDevice, OutputDeviceInternal>(od, odi);
		
		//save output device handle to id relation
		outputDeviceHandles[handle] = outputDeviceIdCounter;

		outputDeviceIdCounter++;
		return od;
	}

	//
	OutputDevice GetOutputDevice(const char* handle) 
	{
		//we check whenever we already have output device oppened with such handle
		auto it = outputDeviceHandles.find(handle);
		if(it != outputDeviceHandles.cend())
			return outputDevices[it->second].first;
		//
		return MakeOutputDevice(handle);
	}

	//
	const char** OutputDeviceList() 
	{
		//to-do:
		return nullptr;
	}
	
	//
	OutputDevice DefaultOutput() 
	{
		return outputDevices[0].first;
	}

	int Error();
}