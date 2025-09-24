#include <DxLib.h>
#include "TitleScene.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"

TitleScene::TitleScene(void)
{
	imgTitle_ = -1;
}

TitleScene::~TitleScene(void)
{
	imgTitle_ = LoadGraph((Application::PATH_SCENE + "Title_1.png").c_str());
}

void TitleScene::Init(void)
{
	SceneManager& sce = SceneManager::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	// シーン遷移
	if (ins.IsTrgMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::SELECT);
	}

	// -------------------------
	// ゲームオーバー・クリア処理
	// -------------------------
	if (ins.IsTrgDown(KEY_INPUT_SPACE)) { sce.ChangeScene(SceneManager::SCENE_ID::GAMEOVER); }

}

void TitleScene::Update(void)
{
}

void TitleScene::Draw(void)
{
}

void TitleScene::Release(void)
{
}
