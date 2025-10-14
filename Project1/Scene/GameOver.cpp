
#include <DxLib.h>
#include "GameOver.h"
GameOverScene::GameOverScene(void)
{
	gameoverImage = -1;
}
GameOverScene::~GameOverScene(void)
{
}
// 初期化処理(最初の１回のみ実行)
bool GameOverScene::SystemInit(void)
{
	gameoverImage = LoadGraph((Application::PATH_SCENE+"Gameover.png").c_str());
	if (gameoverImage == -1)return false;
	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void GameOverScene::GameInit(void)
{
	nextSceneID = E_SCENE_GAMEOVER;
	prevNextKey = nowNextKey = 0;
}
// 更新処理
void GameOverScene::Update(void)
{
	prevNextKey = nowNextKey;
	nowNextKey = CheckHitKey(KEY_INPUT_SPACE);
	// アップトリガーで判断
	if (prevNextKey == 1 && nowNextKey == 0) {
		nextSceneID = E_SCENE_TITLE;
	}
}
// 描画処理
void GameOverScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - GAMEOVER_SIZE_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - GAMEOVER_SIZE_HIG) / 2;
	DrawGraph(dx, dy, gameoverImage, true);

	DrawFormatString(0, 32, GetColor(0xff, 0xff, 0xff), "ゲームオーバー",
		dx, dy, dx, dy);

	DrawString(Application::SCREEN_SIZE_WID / 2 - 75, Application::SCREEN_SIZE_HIG / 2 + 240, "Press Space to Title", GetColor(255, 255, 255));
}
// 解放処理(最後の１回のみ実行)
bool GameOverScene::Release(void)
{
	if (DeleteGraph(gameoverImage) == -1)return false;
	return true;
}
// ゲッター関数
E_SCENE_ID GameOverScene::GetNextSceneID(void) {
	return nextSceneID;
}