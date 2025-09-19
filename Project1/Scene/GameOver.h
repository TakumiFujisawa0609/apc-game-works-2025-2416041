
#pragma once

#include "SceneBase.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"

class SceneManager;

class GameOver : public SceneBase
{

public:

	static const int OVER_BUTTON_Y = Application::SCREEN_SIZE_Y * 5 / 7;

	// コンストラクタ
	GameOver(void);

	// デストラクタ
	~GameOver(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void SetPreviousStage(SceneManager::SCENE_ID prevStage);

private:

	// ボタンの位置（Initで設定）
	int homeX_;
	int nextX_;
	int retryX_;

	// 画像
	int imgOver_;
	int imgFramehover_;
	int imgHomeDefault_;
	int imgNextDefault_;
	int imgReplayDefault_;

	SceneManager::SCENE_ID prevStage_;
};

