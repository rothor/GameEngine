#include "AudioEngine.h"

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);

HRESULT AudioEngine::Initialize()
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&_audio, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;
	if (FAILED(hr = _audio->CreateMasteringVoice(&_masterVoice)))
		return hr;
}

HRESULT AudioEngine::LoadSound(std::wstring fileName, int& soundId, bool loop)
{
	HRESULT hr;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	HANDLE hFile = CreateFile(
		fileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.LoopCount = loop ? 0xFF : 0;
	//buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	
	soundId = _soundIdIncrementer++;
	AudioInfo audioInfo = AudioInfo();
	audioInfo.wfx = wfx;
	audioInfo.buffer = buffer;
	_soundMap[soundId] = audioInfo;
	return S_OK;
}

HRESULT AudioEngine::PlaySound(int soundId)
{
	AudioInfo* audioInfo = &_soundMap[soundId];
	IXAudio2SourceVoice* pSourceVoice;
	HRESULT hr;
	if (FAILED(hr = _audio->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&audioInfo->wfx)))
		return hr;
	AudioEngineVoice* voice = new AudioEngineVoice();
	hr = voice->Initialize(pSourceVoice, audioInfo);
	if (FAILED(hr))
		return hr;
	_playingVoiceList[_playingSoundIdIncrementer++] = voice;
	return S_OK;
}

HRESULT AudioEngine::PlaySound(int soundId, int& playingSoundId)
{
	AudioInfo* audioInfo = &_soundMap[soundId];
	HRESULT hr;
	IXAudio2SourceVoice* pSourceVoice;
	if (FAILED(hr = _audio->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&audioInfo->wfx)))
		return hr;
	AudioEngineVoice* voice = new AudioEngineVoice();
	hr = voice->Initialize(pSourceVoice, audioInfo);
	if (FAILED(hr))
		return hr;
	playingSoundId = _playingSoundIdIncrementer;
	_playingVoiceList[_playingSoundIdIncrementer++] = voice;
	return S_OK;
}

void AudioEngine::SetVoiceVolume(int playingSoundId, float volume)
{
	_playingVoiceList[playingSoundId]->SetVolume(volume);
}

void AudioEngine::ReleaseSound(int soundId)
{
	_soundMap.erase(soundId);
}

void AudioEngine::DestroyVoice(int playingSoundId)
{
	delete _playingVoiceList[playingSoundId];
	_playingVoiceList.erase(playingSoundId);
}

void AudioEngine::PeriodicReleaseResources()
{
	for (auto it = _playingVoiceList.begin(); it != _playingVoiceList.end(); it++) {
		(*it).second->PeriodicManage();
		if ((*it).second->IsDone()) {
			delete (*it).second;
			it = _playingVoiceList.erase(it);
		}
		if (it == _playingVoiceList.end())
			break;
	}
}

bool AudioEngine::IsVoiceDone(int voiceId)
{
	return _playingVoiceList[voiceId]->IsDone();
}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK) {
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType) {
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc) {
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}
	return S_OK;
}

AudioEngineVoice::AudioEngineVoice()
{

}

AudioEngineVoice::~AudioEngineVoice()
{
	_voice->DestroyVoice();
}

HRESULT AudioEngineVoice::Initialize(IXAudio2SourceVoice* pSourceVoice, AudioInfo* audioInfo)
{
	_audioInfo = audioInfo;
	_voice = pSourceVoice;
	HRESULT hr;
	if (FAILED(hr = _voice->SubmitSourceBuffer(&_audioInfo->buffer)))
		return hr;
	if (FAILED(hr = _voice->Start(0)))
		return hr;
}

void AudioEngineVoice::SetVolume(float volume)
{
	_voice->SetVolume(volume);
}

bool AudioEngineVoice::IsDone()
{
	XAUDIO2_VOICE_STATE* state = new XAUDIO2_VOICE_STATE;
	_voice->GetState(state);
	bool isDone;
	if (state->BuffersQueued == 0)
		isDone = true;
	else
		isDone = false;
	delete state;
	return isDone;
}

void AudioEngineVoice::SetWillLoop(bool willLoop)
{
	_willLoop = false;
}

HRESULT AudioEngineVoice::PeriodicManage()
{
	if (_willLoop) {
		XAUDIO2_VOICE_STATE* state = new XAUDIO2_VOICE_STATE;
		_voice->GetState(state);
		bool isDone;
		int buffersQueued = state->BuffersQueued;
		delete state;
		if (buffersQueued == 1) {
			HRESULT hr;
			//if (FAILED(hr = _voice->SubmitSourceBuffer(&_audioInfo->buffer)))
			//	return hr;
		}
	}
	return S_OK;
}
