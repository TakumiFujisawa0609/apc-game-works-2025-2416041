#pragma once
#include "SceneBase.h"
#include "../StDefineData.h"
class TitleScene
{
public:
	static constexpr int TITLE_SIZE_WID = 800;
	static constexpr int TITLE_SIZE_HIG = 600;

	TitleScene(void);
	~TitleScene(void);

	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void);

private:
	int bgImage;

	E_SCENE_ID nextSceneID;

	int prevSpaceKey;
	int nowSpaceKey;


};


