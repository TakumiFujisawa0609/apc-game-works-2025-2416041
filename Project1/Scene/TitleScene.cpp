#include <DxLib.h>
#include "TitleScene.h"
#include "../Application.h"
TitleScene::TitleScene(void)
{
	bgImage = -1;
}
TitleScene::~TitleScene(void)
{

}

bool TitleScene::SystemInit(void)
{
	//SetTransColor(0xff, 0x00, 0xff);

	bgImage = LoadGraph((Application::PATH_SCENE + "Title_1.png").c_str());
	if (bgImage == -1) {
		return false;
	}
	return true;
}
void TitleScene::GameInit(void)
{
	nextSceneID = E_SCENE_TITLE;

	prevSpaceKey = nowSpaceKey = 0;
}

void TitleScene::Update(void)
{
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	if (prevSpaceKey == 1 && nowSpaceKey == 0) {
		nextSceneID = E_SCENE_GAME;
	}
}

void TitleScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - TITLE_SIZE_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - TITLE_SIZE_HIG) / 2;
	DrawGraph(dx, dy, bgImage, true);

	DrawString(Application::SCREEN_SIZE_WID / 2 - 75, Application::SCREEN_SIZE_HIG / 2 + 240, "Press Space to Start", GetColor(255, 255, 255));
}

bool TitleScene::Release(void)
{
	if (DeleteGraph(bgImage) == -1)return false;

	return true;
}

E_SCENE_ID TitleScene::GetNextSceneID(void) { return nextSceneID; }
