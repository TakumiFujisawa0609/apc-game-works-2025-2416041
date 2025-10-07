
#pragma once
#include "../Application.h"
#include "../StDefineData.h"

class GameOverScene
{
public:
	static constexpr int GAMEOVER_SIZE_WID = 457; // ゲームオーバー画像の横サイズ
	static constexpr int GAMEOVER_SIZE_HIG = 81; // ゲームオーバー画像の縦サイズ

	GameOverScene(void);
	~GameOverScene(void);
	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)
	E_SCENE_ID GetNextSceneID(void); // 次に遷移するシーンのIDを取得する

private:

	int gameoverImage; // ゲームオーバー画像のハンドル番号
	E_SCENE_ID nextSceneID; // 次に遷移するシーンのID
	int prevNextKey, nowNextKey;
};

