#include "GameClear.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>

GameClear::GameClear(void)
{
	gameclearImage = -1;
}

GameClear::~GameClear(void)
{
}

bool GameClear::Init(void)
{
	gameclearImage = LoadGraph((Application::PATH_SCENE+"Gameclear.png").c_str());
	if (gameclearImage == -1)return false;
	return true;
}

void GameClear::GameInit(void)
{
	nextSceneID = E_SCENE_GAMECLEAR;
	prevNextKey = nowNextKey = 0;
}

void GameClear::Update(void)
{
	prevNextKey = nowNextKey;
	nowNextKey = CheckHitKey(KEY_INPUT_SPACE);
	// アップトリガーで判断
	if (prevNextKey == 1 && nowNextKey == 0) {
		nextSceneID = E_SCENE_TITLE;
	}
}

void GameClear::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - GAMECLEAR_SIZE_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - GAMECLEAR_SIZE_HIG) / 2;
	DrawGraph(dx, dy, gameclearImage, true);

	DrawFormatString(0, 32, GetColor(0xff, 0xff, 0xff), "ゲームクリア",
		dx, dy, dx, dy);

	DrawString(Application::SCREEN_SIZE_WID / 2 - 75, Application::SCREEN_SIZE_HIG / 2 + 240, "Press Space to Title", GetColor(255, 255, 255));
}

bool GameClear::Release(void)
{
	if (DeleteGraph(gameclearImage) == -1)return false;
	return false;
}

// ゲッター関数
E_SCENE_ID GameClear::GetNextSceneID(void) {
	return nextSceneID;
}
