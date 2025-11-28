#pragma once

#include <string>
#include <map>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager& GetInstance();

	void Init(void);
	void Update(void);
	void Release(void);

	void LoadBGM(const std::string& name, const std::string& path);
	void PlayBGM(const std::string& name, bool isLoop = true);
	void StopBGM(void);

	void LoadSE(const std::string& name, const std::string& path);
	void PlaySE(const std::string& name, bool isLoop = false);
	void StopSE(void);

	void PauseBGM(const std::string& name);
	void ResumeBGM(const std::string& name);

private:
	std::map<std::string, int> bgmMap_;
	std::map<std::string, int> seMap_;

	std::map<std::string, bool> bgmPausedMap_;

	std::map<std::string, int> bgmPosMap_;

	int currentBGMHandle_ = -1;
	int currentSEHandle_ = -1;

	int bgmVolume_ = 255;
	int seVolume_ = 255;

	bool isBGMPaused_ = false;
};

