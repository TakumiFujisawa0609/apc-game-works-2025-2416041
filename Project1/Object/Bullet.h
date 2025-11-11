#pragma once
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"
#include "../Common/AsoUtility.h"
#include "../Object/Player.h"

class GameScene;
class player;

class Bullet
{
public:
	static constexpr int BULLET_SIZE_WID = 60; // 弾の横サイズ
	static constexpr int BULLET_SIZE_HIG = 40; // 弾の縦サイズ
	//static constexpr int ANIM_NUMS = 2; // 弾のアニメーション数
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

	enum class BulletType {
		NORMAL,
		ORBIT,		// 回転
		RAIN,		// 雨
		DEFFUSION,	// 放射
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
	void CreateOrbit(Vector2F center, float rad, float speed, int time);

	void CreateRain(Vector2F center, float rad, float speed, int time);

	void CreateAngle(Vector2F pos, float angleRad, int life = ALIVE_TIME);

	// 円運動用
	bool isOrbit = false;         // 円運動中かどうか

	bool isRain = false;
	Vector2F centerPos;           // 回る中心座標（プレイヤーの座標）
	float radius = 50.0f;         // 回る半径
	float angle = 0.0f;           // 現在の角度
	float angularSpeed = 0.1f;    // 回転速度（ラジアン/frame）
	int orbitTime = 120;           // 回る時間（フレーム）

	BulletType bulletType = BulletType::NORMAL;

	Vector2F bPos; // 弾の座標
	Vector2F bVec; // 弾の移動方向のベクトル(単位ベクトル)
	
private:
	GameScene* gInst; // ゲームシーンクラスのインスタンスのポインタ
	Player* player; // プレイヤークラスのインスタンスのポインタ

	int img; // 弾の画像のハンドル番号テーブル
	int blastImage[BLAST_ANIM_MAX]; // 爆発画像のハンドル番号テーブル

	STATUS bNowStat; // 弾の状態

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


