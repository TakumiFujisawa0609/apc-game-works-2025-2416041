#pragma once

#include "SceneBase.h"
#include "../Application.h"
#include "../Manager/SoundManager.h"

class SceneManager;

class GameClear : public SceneBase
{
public:

	// 定数
	static const int BUTTON_WIDTH = 96;
	static const int BUTTON_HEIGHT = 96;
	static const int BUTTON_SPACING = 50;
	static const int CLEAR_BUTTON_Y = Application::SCREEN_SIZE_Y * 3 / 5;

	GameClear(void);
	~GameClear(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:


	// ボタンの位置（Initで設定）
	int homeX_;
	int nextX_;
	int retryX_;

	// 画像ハンドル
	int imgClear_;
	int imgFramehover_;
	int imgHomeDefault_;
	int imgNextDefault_;
	int imgReplayDefault_;

	int prevSpacekey, nowSpacekey;
};


