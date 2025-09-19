#pragma once

#include "../Scene/SceneBase.h"

struct Button {
	int x, y;
	const char* label;
};

class StageManager : public SceneBase
{
public:

	// ステージID
	enum class STAGE_ID
	{
		NONE,
		STAGE1,
		STAGE2,
		STAGE3,
		MAX,
	};

	static constexpr int SELECT_STAGE_X = 264;
	static constexpr int SELECT_STAGE_Y = 440;

	const int BUTTON_X = SELECT_STAGE_X;

	// ボタンのレイアウト設定
	const int buttonCount = 2;

	// ボタン間のスペース
	const int SPACE = 60;

	// コンストラクタ
	StageManager(void);
	// デストラクタ
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	bool IsMouseOver(const Button& btn);   // マウスがボタンの上にあるか

	// ステージID
	STAGE_ID stageId_;

	int buttonDefaultImage;    // 通常ボタン画像
	int buttonHoverImage;      // ホバーボタン画像

	Button stageButtons[3];    // ステージボタン（3つ）

	int prevMouseInput_;

};