#pragma once
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"
#include "../Common/AsoUtility.h"
class GameScene;
class Bullet
{
public:
	static constexpr int BULLET_SIZE_WID = 12; // 弾の横サイズ
	static constexpr int BULLET_SIZE_HIG = 12; // 弾の縦サイズ
	static constexpr int ANIM_NUMS = 2; // 弾のアニメーション数
	static constexpr float MOVE_SPEED = 5.0f; // 移動速度
	static constexpr int ANIM_INTERVAL = 5; // 弾のアニメーションの更新間隔
	static constexpr int ALIVE_TIME = 30; // 弾の生存時間
	static constexpr int BLAST_SIZE_WID = 32; // 爆発のサイズ
	static constexpr int BLAST_SIZE_HIG = 32;
	static constexpr int BLAST_ANIM_XNUM = 4; // 爆発画像ファイル内の横方向のパターン数
	static constexpr int BLAST_ANIM_YNUM = 4; // 爆発画像ファイル内の縦方向のパターン数
	static constexpr int BLAST_ANIM_MAX = BLAST_ANIM_XNUM * BLAST_ANIM_YNUM;
	// 爆発アニメーションのパターン数
	static constexpr int BLAST_ANIM_INTERVAL = 2; // 爆発アニメーションの更新間隔

	// 弾の状態
	enum class STATUS {
		E_STAT_NONE,
		E_STAT_MOVE, // 弾の移動
		E_STAT_BLAST, // 爆発
		E_STAT_END, // 弾の動作終了
		E_STAT_MAX,
	};

	Bullet(GameScene* gs);
	~Bullet(void);
	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)
	// 弾を生成する
	void Create(Vector2F pos, AsoUtility::DIRECTION dir);
	// 弾の生成が可能かどうか
	bool IsEnableCreate(void);
	// 弾が発射状態かどうかを取得
	// bool : true = 発射中 / false = 日発射状態
	bool IsShotState(void) { return bNowStat == STATUS::E_STAT_MOVE; }
	// 弾の座標を取得
	Vector2F GetBulletPos(void) { return bPos; }
	// 爆発表示開始
	void BlastOn(Vector2F pos);
private:
	GameScene* gInst; // ゲームシーンクラスのインスタンスのポインタ

	int img[ANIM_NUMS]; // 弾の画像のハンドル番号テーブル
	int blastImage[BLAST_ANIM_MAX]; // 爆発画像のハンドル番号テーブル

	STATUS bNowStat; // 弾の状態

	Vector2F bPos; // 弾の座標
	Vector2F bVec; // 弾の移動方向のベクトル(単位ベクトル)

	int bDir; // 弾の移動方向
	int aliveCounter; // 弾の生存時間のカウンター
	int animCounter; // 弾のアニメーションカウンター
	int blastAnimCounter; // 爆発のアニメーションカウンター

	Vector2F blastPos; // 爆発の座標

	// 弾の状態毎の更新処理
	void UpdateMove(void);
	void UpdateBlast(void);
	void UpdateEnd(void);
	// 弾の状態毎の描画処理
	void DrawMove(void);
	void DrawBlast(void);
	void DrawEnd(void);
	// 状態を遷移させる
	void ChangeStatus(STATUS stat);
};


