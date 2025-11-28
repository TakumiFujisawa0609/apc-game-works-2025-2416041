#include "SoundManager.h"
#include "../Application.h"
#include <DxLib.h>

SoundManager::SoundManager(void)
	: currentBGMHandle_(-1), currentSEHandle_(-1), bgmVolume_(255), seVolume_(255), isBGMPaused_(false)
{
}

SoundManager::~SoundManager(void)
{
	Release();
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::Init(void)
{
}

void SoundManager::Update(void)
{
}

void SoundManager::Release(void)
{
	for (auto& bgm : bgmMap_) {
		DeleteSoundMem(bgm.second);
	}
	bgmMap_.clear();

	for (auto& se : seMap_) {
		DeleteSoundMem(se.second);
	}
	seMap_.clear();
}

void SoundManager::LoadBGM(const std::string& name, const std::string& path)
{
	int handle = LoadSoundMem(path.c_str());
	if (handle != -1) {
		bgmMap_[name] = handle;
	}
}

void SoundManager::LoadSE(const std::string& name, const std::string& path)
{
	int handle = LoadSoundMem(path.c_str());
	if (handle != -1) {
		seMap_[name] = handle;
	}
}

void SoundManager::PlayBGM(const std::string& name, bool isLoop)
{
	auto it = bgmMap_.find(name);
	if (it != bgmMap_.end()) {
		if (currentBGMHandle_ != -1) {
			StopSoundMem(currentBGMHandle_);
		}
		currentBGMHandle_ = it->second;
		ChangeVolumeSoundMem(bgmVolume_, currentBGMHandle_);
		PlaySoundMem(currentBGMHandle_, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}
}

void SoundManager::StopBGM(void)
{
	if (currentBGMHandle_ != -1) {
		StopSoundMem(currentBGMHandle_);
		currentBGMHandle_ = -1;
	}
}

void SoundManager::PlaySE(const std::string& name, bool isLoop)
{
	auto it = seMap_.find(name);
	if (it != seMap_.end()) {
		if (currentSEHandle_ != -1) {
			StopSoundMem(currentSEHandle_);
		}
		currentSEHandle_ = it->second;
		ChangeVolumeSoundMem(seVolume_, currentSEHandle_);
		PlaySoundMem(currentSEHandle_, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}
}

void SoundManager::StopSE(void)
{
	if (currentSEHandle_ != -1) {
		StopSoundMem(currentSEHandle_);
		currentSEHandle_ = -1;
	}
}

void SoundManager::PauseBGM(const std::string& name)
{
	auto it = bgmMap_.find(name);
	if (it != bgmMap_.end()) {
		int handle = it->second;
		if (CheckSoundMem(handle)) {
			int pos = GetSoundCurrentPosition(handle);
			bgmPosMap_[name] = pos;  // ³‚µ‚¢ƒ}ƒbƒv‚É•Û‘¶

			StopSoundMem(handle);
			bgmPausedMap_[name] = true;
		}
	}
}

void SoundManager::ResumeBGM(const std::string& name)
{
	auto it = bgmMap_.find(name);
	if (it != bgmMap_.end() && bgmPausedMap_[name]) {
		int handle = it->second;
		ChangeVolumeSoundMem(bgmVolume_, handle);

		if (bgmPosMap_.find(name) != bgmPosMap_.end()) {
			SetSoundCurrentPosition(handle, bgmPosMap_[name]);
		}

		PlaySoundMem(handle, DX_PLAYTYPE_LOOP, false);
		bgmPausedMap_[name] = false;
	}
}