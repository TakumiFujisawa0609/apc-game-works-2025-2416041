#pragma once
#include <string>
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include "../../Common/AsoUtility.h"
class GameScene;
class Enemy
{
public:
	enum class ENEMY_TYPE
	{
		E_TYPE_NORMAL,
		E_TYPE_Skeleton,
		E_TYPE_FIRE,
		E_TYPE_LIZARD_SMALL,
		E_TYPE_LIZARD_BIG,
		E_TYPE_DRAGON,
		E_TYPE_BOSS,

		E_TYPE_MAX,
	};

	static constexpr int ANIM_NUMS = 4; // 方向毎のアニメーション数
	static constexpr int CHARA_MAX = ANIM_NUMS * static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX);
	static constexpr int ANIM_INTERVAL = 1; // アニメーションの更新間隔
	Enemy(void);
	~Enemy(void);
	virtual bool SystemInit(GameScene* gs); // 初期化処理(最初の１回のみ実行)
	virtual void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	virtual void Update(void); // 更新処理
	virtual void Draw(void); // 描画処理
	virtual bool Release(void); // 解放処理(最後の１回のみ実行)

	// 敵キャラ個別のパラメータ設定処理(純粋仮想関数)
	virtual void SetEnemyParam(void) = 0;

	// 敵の座標を取得
	virtual Vector2F GetEnemyPos(void) { return pos; }

	Vector2F GetEnemyPosF() const {
		return { static_cast<float>(pos.x), static_cast<float>(pos.y) };
	}

	// 敵の画像サイズを取得
	virtual Vector2 GetEnemySize(void) { return size; }

	// 敵の生存状態を取得する
	virtual bool GetAlive(void) { return aliveFlg; }

	// 敵の生存状態を設定する
	virtual void SetAlive(bool bflg) { aliveFlg = bflg; }

	void SetDamege(int dp);

protected:
	GameScene* gInst; // ゲームシーンのインスタンスのポインタ

	// 敵画像のハンドル番号テーブル
	int img[ANIM_NUMS];
	// 敵表示座標
	Vector2F pos;
	// 敵が向いている方向
	int dir;
	// アニメーションカウンター
	int animCounter;
	// ヒットポイント
	int hp;
	// ヒットポイント最大値
	int hpMax;
	// 生存フラグ
	bool aliveFlg;
public:
	// 敵の画像ファイル名
	std::string imgFName;
	// 敵の画像サイズ
	Vector2 size;
	// 敵の移動速度
	float speed;

	void SetMoveDirection(Vector2F dir);
};

