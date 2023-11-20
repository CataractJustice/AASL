# AASL
An audio library with ease of use as first priority

## Usage
Initialisation
```
AASL::Init();
```

Play a sound
```
SoundController audio = AASL::PlaySound("sound.wav");
```

Pause/Resume sound
```
audio.pause();
```
```
audio.resume();
```

Set sound volume
```
audio.options().volume = 0.5f;
```

Audio time controll
```
audio.rewindTime(5);
```
```
audio.forwardTime(5);
```
```
audio.setTimeSeconds(30);
```
```
audio.restart();
```
Set audio looping
```
audio.options().loop = true;
```

Controll audio playback speed
```
audio.options().playSpeed = 2.0f;
```

Play sound with preconfigured options
```
SoundOptions options;
options.playSpeed = 0.5f;
options.volume = 0.5f;
StartSound('sound.wav', options);
```
