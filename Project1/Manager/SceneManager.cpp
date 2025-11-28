#include <DxLib.h>
#include "SceneManager.h"
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOver.h"
#include "../Scene/GameClear.h"

#include "../Manager/InputManager.h"

SceneManager::SceneManager(void)
{
	fader = nullptr;
	titleInst = nullptr;
	gameInst = nullptr;
	gameover = nullptr;
	gameclear = nullptr;
	scene_ID = waitScene = E_SCENE_NON;

	gamePause = ON_OFF::GAME;
	pauseSelect = PAUSE_SELECT::RESUME;
	isGameEnd = false;
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
	InputManager::CreateInstance();

	gamePause = ON_OFF::GAME;
	pauseSelect = PAUSE_SELECT::RESUME;
	isGameEnd = false;

	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void SceneManager::GameInit(void)
{
}
// 更新処理
void SceneManager::Update(void)
{
	InputManager& input = InputManager::GetInstance();

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

		switch (gamePause)
		{
		case SceneManager::ON_OFF::GAME:
			// こっちはゲーム画面の処理

			// ゲームシーン中に ESC でポーズへ
			if (scene_ID == E_SCENE_GAME &&
				input.IsTrgDown(KEY_INPUT_ESCAPE)) {
				gamePause = ON_OFF::PAUSE;
				pauseSelect = PAUSE_SELECT::RESUME; // 毎回上からスタート
				break;
			}

			// 各シーンの更新
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

			break;

		case SceneManager::ON_OFF::PAUSE:
			// ここがポーズ画面の処理

			// ↑↓ で項目移動（使うキーは好きに変えていい）
			if (input.IsTrgDown(KEY_INPUT_UP)) {
				int v = static_cast<int>(pauseSelect);
				--v;
				if (v < 0) {
					v = static_cast<int>(PAUSE_SELECT::MAX) - 1;
				}
				pauseSelect = static_cast<PAUSE_SELECT>(v);
			}
			else if (input.IsTrgDown(KEY_INPUT_DOWN)) {
				int v = static_cast<int>(pauseSelect);
				++v;
				if (v >= static_cast<int>(PAUSE_SELECT::MAX)) {
					v = 0;
				}
				pauseSelect = static_cast<PAUSE_SELECT>(v);
			}

			// Z で決定（決定キーはゲームの仕様に合わせて変えてよい）
			if (input.IsTrgDown(KEY_INPUT_Z)) {
				switch (pauseSelect) {
				case PAUSE_SELECT::RESUME:
					// ゲームに戻る
					gamePause = ON_OFF::GAME;
					break;
				case PAUSE_SELECT::EXIT_GAME:
					// ゲーム終了フラグ ON
					isGameEnd = true;
					break;
				default:
					break;
				}
			}

			// ESC で即ポーズ解除するショートカットも残しておく（好みで）
			if (input.IsTrgDown(KEY_INPUT_ESCAPE)) {
				gamePause = ON_OFF::GAME;
			}

			break;
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

	// ポーズ中ならポーズ画面をオーバーレイ
	if (gamePause == ON_OFF::PAUSE) {
		DrawPauseMenu();
	}
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

void SceneManager::DrawPauseMenu()
{
	const int sw = Application::SCREEN_SIZE_WID;
	const int sh = Application::SCREEN_SIZE_HIG;

	// 画面全体を半透明黒で覆う
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, sw, sh, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int white = GetColor(255, 255, 255);

	// タイトル "PAUSE"
	const char* title = "PAUSE";
	int titleW = GetDrawStringWidth(title, -1);
	int titleX = (sw - titleW) / 2;
	int titleY = sh / 2 - 80;
	DrawString(titleX, titleY, title, white);

	// メニュー項目
	struct Item {
		const char* text;
		PAUSE_SELECT id;
	};

	Item items[] = {
		{ "ゲームに戻る",      PAUSE_SELECT::RESUME },
		{ "ゲームを終了する",  PAUSE_SELECT::EXIT_GAME },
	};

	const int itemCount = 2;
	const int baseY = sh / 2 - 20;
	const int lineH = 30;

	for (int i = 0; i < itemCount; ++i) {
		const char* txt = items[i].text;
		int w = GetDrawStringWidth(txt, -1);
		int x = (sw - w) / 2;
		int y = baseY + i * lineH;

		// 選択中の項目の左に '>' を付ける
		if (pauseSelect == items[i].id) {
			DrawString(x - 20, y, ">", white);
		}
		DrawString(x, y, txt, white);
	}

	// 操作説明
	const char* hint = "↑↓で選択  Zで決定  ESCでポーズ解除";
	int hintW = GetDrawStringWidth(hint, -1);
	int hintX = (sw - hintW) / 2;
	int hintY = baseY + itemCount * lineH + 20;
	DrawString(hintX, hintY, hint, white);
}
