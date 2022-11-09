#pragma once
#include <xaudio2.h>
#include <map>
#include <string>

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

struct AudioInfo
{
public:
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
};

class AudioEngineVoice
{
public:
	AudioEngineVoice();
	~AudioEngineVoice();
	HRESULT Initialize(IXAudio2SourceVoice* pSourceVoice, AudioInfo* audioInfo);
	void SetVolume(float volume);
	bool IsDone();
	void SetWillLoop(bool willLoop);
	HRESULT PeriodicManage();

protected:
	AudioInfo* _audioInfo;
	IXAudio2SourceVoice* _voice;
	bool _willLoop;
};

class AudioEngine
{
public:
	HRESULT Initialize();
	HRESULT LoadSound(std::wstring fileName, int& soundId, bool loop = false);
	HRESULT PlaySound(int soundId);
	// Use the second PlaySound method if you want to control the voice volume after you've created it.
	HRESULT PlaySound(int soundId, int& playingSoundId);
	void SetVoiceVolume(int playingSoundId, float volume);
	void ReleaseSound(int soundId);
	void DestroyVoice(int playingSoundId);
	// Make sure to call PeriodicReleaseResources() periodically to release the unused resources.
	void PeriodicReleaseResources();
	bool IsVoiceDone(int voiceId);

protected:
	IXAudio2* _audio;
	IXAudio2MasteringVoice* _masterVoice = nullptr;
	std::map<int, AudioInfo> _soundMap{};
	int _soundIdIncrementer = 0;
	std::map<int, AudioEngineVoice*> _playingVoiceList{};
	int _playingSoundIdIncrementer = 0;
};