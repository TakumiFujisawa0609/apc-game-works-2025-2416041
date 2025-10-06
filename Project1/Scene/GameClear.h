#pragma once

#include "SceneBase.h"
#include "../Application.h"
#include "../Manager/SoundManager.h"

class SceneManager;

class GameClear : public SceneManager
{
public:

	// 定数
	static const int BUTTON_WIDTH = 96;
	static const int BUTTON_HEIGHT = 96;
	static const int BUTTON_SPACING = 50;
	static const int CLEAR_BUTTON_Y = Application::SCREEN_SIZE_WID * 3 / 5;

	GameClear(void);
	~GameClear(void);

	void Init(void);
	void Update(void);
	void Draw(void) ;
	void Release(void);

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


