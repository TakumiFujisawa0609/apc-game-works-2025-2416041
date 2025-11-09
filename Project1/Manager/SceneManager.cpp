#include <DxLib.h>
#include "SceneManager.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOver.h"
#include "../Scene/GameClear.h"
SceneManager::SceneManager(void)
{
	fader = nullptr;
	titleInst = nullptr;
	gameInst = nullptr;
	gameover = nullptr;
	gameclear = nullptr;
	scene_ID = waitScene = E_SCENE_NON;
}
SceneManager::~SceneManager(void)
{
}
// 初期化処理(最初の１回のみ実行)
bool SceneManager::SystemInit(void)
{
	// インスタンスの生成
	fader = new Fader();
	if (fader == nullptr)return false;
	SetTransColor(0xff, 0x00, 0xff); // 透過色の設定
	fader->SystemInit();
	sceneChangeFlg = false;
	ChangeScene(E_SCENE_TITLE);
	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void SceneManager::GameInit(void)
{
}
// 更新処理
void SceneManager::Update(void)
{
	fader->Update();
	if (sceneChangeFlg) {
		// シーンチェンジ実行中
		if (fader->IsEnd() && waitScene != E_SCENE_NON) {
			ChangeScene(waitScene);
			waitScene = E_SCENE_NON;
			fader->SetFade(E_STAT_FADE_IN);
		}
		else if (fader->IsEnd() && waitScene == E_SCENE_NON) {
			sceneChangeFlg = false;
		}
	}
	else {
		E_SCENE_ID nextSceneID = scene_ID;
		switch (scene_ID) {
		case E_SCENE_TITLE:
			titleInst->Update();
			nextSceneID = titleInst->GetNextSceneID();
			break;
		case E_SCENE_GAME:
			gameInst->Update();
			nextSceneID = gameInst->GetNextSceneID();
			break;
		case E_SCENE_GAMEOVER:
			gameover->Update();
			nextSceneID = gameover->GetNextSceneID();
			break;
		case E_SCENE_GAMECLEAR:
			gameclear->Update();
			nextSceneID = gameclear->GetNextSceneID();
			break;
		}
		// シーン遷移判定
		if (scene_ID != nextSceneID) {
			sceneChangeFlg = true;
			waitScene = nextSceneID;
			fader->SetFade(E_STAT_FADE_OUT);
		}
	}
}
// 描画処理
void SceneManager::Draw(void)
{
	switch (scene_ID) {
	case E_SCENE_TITLE:
		titleInst->Draw();
		break;
	case E_SCENE_GAME:
		gameInst->Draw();
		break;
	case E_SCENE_GAMEOVER:
		gameover->Draw();
		break;
	case E_SCENE_GAMECLEAR:
		gameclear->Draw();
		break;
	}
	fader->Draw();
}
// 解放処理(最後の１回のみ実行)
bool SceneManager::Release(void)
{
	ReleaseScene(E_SCENE_TITLE);
	ReleaseScene(E_SCENE_GAME);
	ReleaseScene(E_SCENE_GAMEOVER);

	fader->Release();

	delete fader;
	fader = nullptr;
	return true;
}
bool SceneManager::ChangeScene(E_SCENE_ID id)
{
	// 現在のシーンを解放
	ReleaseScene(scene_ID);
	// シーンIDを変更
	scene_ID = id;
	switch (scene_ID) {
	case E_SCENE_TITLE:
		if (titleInst == nullptr) {
			titleInst = new TitleScene();
			if (titleInst == nullptr)return false;
			if (titleInst->SystemInit() == false)return false;
			titleInst->GameInit();
		}
		break;
	case E_SCENE_GAME:
		if (gameInst == nullptr) {
			gameInst = new GameScene();
			if (gameInst == nullptr)return false;
			gameInst->SystemInit();
			gameInst->GameInit();
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gameover == nullptr) {
			gameover = new GameOverScene();
			if (gameover == nullptr)return false;
			gameover->SystemInit();
			gameover->GameInit();
		}
		break;
	case E_SCENE_GAMECLEAR:
		if (gameclear == nullptr) {
			gameclear = new GameClear();
			if (gameclear == nullptr)return false;
			gameclear->Init();
			gameclear->GameInit();
		}
		break;
	}
	return true;
}
void SceneManager::ReleaseScene(E_SCENE_ID id)
{
	switch (id) {
	case E_SCENE_TITLE:
		if (titleInst != nullptr) {
			titleInst->Release();
			delete titleInst;
			titleInst = nullptr;
		}
		break;
	case E_SCENE_GAME:
		if (gameInst != nullptr) {
			gameInst->Release();
			delete gameInst;
			gameInst = nullptr;
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gameover != nullptr) {
			gameover->Release();
			delete gameover;
			gameover = nullptr;
		}
		break;
	case E_SCENE_GAMECLEAR:	
		if (gameclear != nullptr) {
			gameclear->Release();
			delete gameclear;
			gameclear = nullptr;
		}
		break;
	}
}
