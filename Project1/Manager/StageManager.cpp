#include "StageManager.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include <DxLib.h>

StageManager::StageManager(void)
{
	buttonDefaultImage, buttonHoverImage = -1;
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	stageId_ = STAGE_ID::NONE;

	buttonDefaultImage = LoadGraph((Application::PATH_UI + "Default@2x_2.png").c_str());
	buttonHoverImage = LoadGraph((Application::PATH_UI + "Hover@2x_2.png").c_str());

	if (buttonDefaultImage == -1 || buttonHoverImage == -1) {
		MessageBoxA(NULL, "ボタン画像の読み込みに失敗しました。", "エラー", MB_OK);
	}

	// 総ボタン幅 = ボタン3つ + スペース2つ
	const int totalWidth = BUTTON_X * buttonCount + SPACE * (buttonCount - 1);
	const int startX = (Application::SCREEN_SIZE_X - totalWidth) / 2;
	const int y = 150; // 縦位置（中央より少し下）

	for (int i = 0; i < buttonCount; ++i) {
		int x = startX + i * (BUTTON_X + SPACE);
		stageButtons[i] = { x, y, "STAGE" };
	}

	prevMouseInput_ = 0;

}


void StageManager::Update(void)
{
	SceneManager& sce = SceneManager::GetInstance();
	

	int currentInput = GetMouseInput();

	// 左クリックを押した瞬間だけ判定
	if ((currentInput & MOUSE_INPUT_LEFT) && !(prevMouseInput_ & MOUSE_INPUT_LEFT)) {

		for (int i = 0; i < 2; ++i) {
			if (IsMouseOver(stageButtons[i])) {
				if (i == 0) {
					sce.ChangeScene(SceneManager::SCENE_ID::STAGE02);
				}
				else if (i == 1) {
					sce.ChangeScene(SceneManager::SCENE_ID::STAGE01);
				}
			}
		}
	}

	prevMouseInput_ = currentInput;

}

void StageManager::Draw(void)
{
	// 各ボタンを描画
	for (int i = 0; i < 2; ++i) {
		const Button& btn = stageButtons[i];

		int img = IsMouseOver(btn) ? buttonHoverImage : buttonDefaultImage;

		int centerX = btn.x + SELECT_STAGE_X / 2;
		int centerY = btn.y + SELECT_STAGE_Y / 2;

		DrawRotaGraph(centerX, centerY, 1.0, 0.0, img, true);
		DrawString(centerX - 30, centerY + 90, btn.label, GetColor(255, 255, 255));
	}

	// デバッグ用：マウス座標表示
	int mx, my;
	GetMousePoint(&mx, &my);
	//DrawFormatString(0, 0, GetColor(255, 255, 0), "Mouse: (%d, %d)", mx, my);
}

void StageManager::Release(void)
{
	
}

bool StageManager::IsMouseOver(const Button& btn)
{
	int mx, my;
	GetMousePoint(&mx, &my);
	return (mx >= btn.x && mx <= btn.x + SELECT_STAGE_X &&
		my >= btn.y && my <= btn.y + SELECT_STAGE_Y);
}
