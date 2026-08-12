#include "SoundManager.h"
#include <string>
#include <map>
#include "ImGUI/imgui.h"

namespace SoundManager {
	IXAudio2* xAudio_ = nullptr;
	IXAudio2MasteringVoice* materingVoice_ = nullptr;
	std::map<std::string, SoundData> soundMap_;
}

void SoundManager::Init() {
	soundMap_.clear();
	XAudio2Create(&xAudio_);
	xAudio_->CreateMasteringVoice(&materingVoice_);
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
}

bool SoundManager::Load(const std::string& fileName, bool loop) {
	auto findItr = soundMap_.find(fileName);
	if (findItr != soundMap_.end()) {	// 見つかった場合
        return true;
	}

	SoundData soundData = {};
	std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());
    soundData.fileName = fileName;
	MFCreateSourceReaderFromURL(wFileName.c_str(), nullptr, &soundData.sourceReader);
	MFCreateMediaType(&soundData.mediaType);
    soundData.mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    soundData.mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    soundData.sourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, soundData.mediaType);
    soundData.mediaType->Release();
    soundData.sourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &soundData.mediaType);

    WAVEFORMATEX* waveFormat = nullptr;
    MFCreateWaveFormatExFromMFMediaType(soundData.mediaType, &waveFormat, nullptr);


    while (true) {
        IMFSample* mfSample = nullptr;
        DWORD dwStreamFlag = {};
        soundData.sourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlag, nullptr, &mfSample);

        if (dwStreamFlag & MF_SOURCE_READERF_ENDOFSTREAM) {
            break;
        }

        IMFMediaBuffer* mfMediaBuffer = nullptr;
        mfSample->ConvertToContiguousBuffer(&mfMediaBuffer);

        BYTE* buffer = nullptr;
        DWORD currentLength = {};
        mfMediaBuffer->Lock(&buffer, nullptr, &currentLength);

        soundData.mediaBuffer.resize(soundData.mediaBuffer.size() + currentLength);
        memcpy_s(soundData.mediaBuffer.data() + soundData.mediaBuffer.size() - currentLength, currentLength, buffer, currentLength);

        mfMediaBuffer->Unlock();
        mfSample->Release();

    }

    xAudio_->CreateSourceVoice(&soundData.sourceVoice, waveFormat);

    soundMap_[fileName] = soundData;
    if (soundMap_.find(fileName) != soundMap_.end()) {	
        return true;
    }

    return false;
}

bool SoundManager::Play(const std::string& fileName) {
    if (soundMap_.find(fileName) != soundMap_.end()) {
        SoundData& soundData = soundMap_[fileName];
        XAUDIO2_BUFFER buffer = {};
        buffer.pAudioData = soundData.mediaBuffer.data();
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.AudioBytes = sizeof(BYTE) * (UINT32)soundData.mediaBuffer.size();
        soundData.sourceVoice->SubmitSourceBuffer(&buffer);
        soundData.sourceVoice->Start();
        return true;
    }

    return false;
}

bool SoundManager::Stop(const std::string& fileName) {
    if (soundMap_.find(fileName) != soundMap_.end()) {
        SoundData& soundData = soundMap_[fileName];
        soundData.sourceVoice[0].Stop();
        soundData.sourceVoice[0].FlushSourceBuffers();
        return true;
    }

    return false;
}

bool SoundManager::ChangeVolume(const std::string& fileName, float volume) {
    if (soundMap_.find(fileName) != soundMap_.end()) {
        if (volume < 0) volume = 0;
        SoundData& soundData = soundMap_[fileName];
        soundData.sourceVoice[0].SetVolume(volume);

        return true;
    }

    return false;
}

void SoundManager::DrawDebugImGUI() {
    ImGui::Begin("SoundManager(Debug)");
    for (const auto& pair : soundMap_) {
        SoundData soundData = pair.second;
        if (ImGui::CollapsingHeader(soundData.fileName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            float volume = 0;
            soundData.sourceVoice[0].GetVolume(&volume);
            ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
            soundData.sourceVoice[0].SetVolume(volume);
        }
    }
    ImGui::End();
}
