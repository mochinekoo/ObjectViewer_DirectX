#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

struct SoundData {
	std::string fileName = "";
	IMFSourceReader* sourceReader = nullptr;
	IMFMediaType* mediaType = nullptr;
	IXAudio2SourceVoice* sourceVoice = nullptr;
	std::vector<BYTE> mediaBuffer;
};

namespace SoundManager {

	void Init();
	bool Load(const std::string& fileName, bool loop = false);
	bool Play(const std::string& fileName);
	bool Stop(const std::string& fileName);
	bool ChangeVolume(const std::string& fileName, float volume);

	void DrawDebugImGUI();
}