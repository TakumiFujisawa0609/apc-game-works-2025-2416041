#pragma once

#include <vector>
#include<list>
#include "../Common/Vector2.h"
#include "../StDefineData.h"

class StageBase;
class Player;
class Enemy;
class Bullet;

class GameScene
{

public:
	//スクロールを発生させる範囲
	static constexpr int SCROLL_AREA_WID = 150;
	static constexpr int SCROLL_AREA_HIG = 100;

	//エンカウント(値が大きいと出現する間隔が長くなる）
	static constexpr int ENCOUNT = 180;

	GameScene(void);
	~GameScene(void);

	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)

	StageBase* GetLpStage(void) { return stage; }
	Player* GetLpPlayer(void) { return player; }

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

private:
	StageBase* stage; // Stageクラスのインスタンスのポインタ
	Player* player; // Playerクラスのインスタンスのポインタ
	Bullet* bullet; // Bulletクラスのインスタンスのポインタ
	// EnemyBaseクラスのインスタンスのポインタの可変長テーブル

	// 複数の敵を動的に管理する
	std::vector<Enemy*> enemys;
	// 敵の足元のY座標の小さい順にソートする為の可変長テーブル
	std::list<Enemy*> enemysSortTbl;
	// 敵の発生頻度用のカウンター
	int enCounter;

	int prevShotKey, nowShotKey; // キーの入力状態

	E_SCENE_ID nextSceneID; // 次に遷移するシーンのID
	void MapScrollProc(void);
	void EraseEnemys(void); // 敵データテーブルを空にする
	Vector2 WorldPos2MapPos(Vector2 wpos); // ワールド座標をマップ座標に変換する
	bool IsCollisionStage(Vector2 worldPos); // ワールド座標の指す地形は衝突対象か
	void CollisionCheck(void);
	bool CollisionCheckRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2);
};


