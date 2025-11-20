#pragma once
#include "../Common/Vector2.h"
#include "../Common/AsoUtility.h"
#include "../Scene/Stage/StageBase.h"
#include "../Scene/GameScene.h"
#include "Bullet.h"

class Bullet;
class GameScene;
class Player
{
public:
	static constexpr int PLAYER_WID = 96; // プレイヤーの横サイズ
	static constexpr int PLAYER_HIG = 84; // プレイヤーの縦サイズ
	static constexpr int ANIM_NUMS = 7; // 方向毎のアニメーション数
	static constexpr int CHARA_MAX = ANIM_NUMS * static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX);
	static constexpr int MOVE_SPEED = 4; // 一回の移動量
	static constexpr int ANIM_INTERVAL = 10; // アニメーションの更新間隔
	static constexpr int PLAYER_HP_MAX = 100; // プレイヤーのHPの最大値
	static constexpr int SMALL_HIT_BUFF_TIME = 60 * 5;  // 小当たり 5秒
	static constexpr int BIG_HIT_BUFF_TIME = 60 * 10; // 大当たり 10秒
	static constexpr int HEAL_EFFECT_TIME = 30; // 回復エフェクト表示時間

	//バフ種別
	enum class BuffType
	{
		None,
		SmallHit, // 小当たり用
		BigHit,   // 大当たり用
	};

	Player(GameScene* gs);
	~Player(void);
	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)

	// ゲッター・セッター関数
	Vector2 GetPlayerPos(void) { return playerPos; }
	void SetPlayerPos(Vector2 mPos) { playerPos = mPos; }
	AsoUtility::DIRECTION GetPlayerDir(void) { return static_cast<AsoUtility::DIRECTION>(playerDir); }
	int GetHp(void) { return hp; } // HPの取得
	bool GetAlive(void) { return aliveFlg; } // 生存状態の取得
	void SetDamage(int dp);

	void Heal(int amount);

	// スロット用ステータスアップ API
	void ApplySmallHitBuff(); // 小当たりバフ
	void ApplyBigHitBuff();   // 大当たりバフ

	// 現在の移動速度（バフを加味）
	float GetMoveSpeed() const;
private:
	GameScene* gInst;
	Bullet* bullet; // 弾クラスのインスタンスのポインタ

	// プレイヤー画像
	int player_img[ANIM_NUMS];

	// プレイヤー表示座標
	Vector2 playerPos;

	// プレイヤーが向いている方向
	int playerDir;

	// アニメーションカウンター
	int animCounter;

	// ヒットポイント
	int hp;

	// 生存フラグ
	bool aliveFlg;

	//バフ関連
	BuffType buffType_;
	int buffTimer_;

	int  healEffectTimer_;
	int  lastHealAmount_;
};

