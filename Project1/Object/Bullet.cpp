#include <DxLib.h>
#include "Bullet.h"
#include "../Scene/GameScene.h"
#include "../Scene/Stage/StageBase.h"
#include "Player.h"
#include "../Application.h"
#include "../Object/Enemy/Enemy.h"
/*
 *----------------------------------------------------
 * コンストラクタ
 * Input:
 * GameScene* gs : ゲームシーンクラスのインスタンスのポインタ
 * Output:
 * 無し
 *----------------------------------------------------
 */
Bullet::Bullet(GameScene* gs)
{
	gInst = gs;
}
Bullet::~Bullet(void)
{
}
// 初期化処理(最初の１回のみ実行)
bool Bullet::SystemInit(void)
{
	img = LoadGraph((Application::PATH_WEAPON + "sword.png").c_str());
	if (img == -1) return false; // 読み込み失敗チェック

	int err = LoadDivGraph(((Application::PATH_OBJECT+"weapon/Blast.png").c_str()), BLAST_ANIM_MAX, BLAST_ANIM_XNUM, BLAST_ANIM_YNUM,
		BLAST_SIZE_WID, BLAST_SIZE_HIG, blastImage);
	if (err == -1)return false;
	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void Bullet::GameInit(void)
{
	bNowStat = STATUS::E_STAT_NONE;
	bPos.x = bPos.y = 0.0f;
	bVec.x = bVec.y = 0.0f;
	bDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);
	aliveCounter = animCounter = blastAnimCounter = 0;
}
// 更新処理
void Bullet::Update(void)
{
	switch (bNowStat) {
	case Bullet::STATUS::E_STAT_MOVE:
		// 弾の移動
		UpdateMove();
		break;
	case Bullet::STATUS::E_STAT_BLAST:
		// 弾の爆発アニメーション
		UpdateBlast();
		break;
	case Bullet::STATUS::E_STAT_END:
		UpdateEnd();
		break;
	}
}
// 弾の状態毎の更新処理
void Bullet::UpdateMove(void)
{
	if (bulletType == BulletType::ORBIT && isOrbit)
	{
		// プレイヤー座標を中心に追従
		Vector2 pPos = gInst->GetLpPlayer()->GetPlayerPos();
		centerPos.x = static_cast<float>(pPos.x);
		centerPos.y = static_cast<float>(pPos.y);

		// 角度を更新
		angle += angularSpeed;
		bPos.x = centerPos.x + cosf(angle) * radius;
		bPos.y = centerPos.y + sinf(angle) * radius;

		orbitTime--;
		if (orbitTime <= 0)
		{
			isOrbit = false;
			BlastOn(bPos); // 生存時間終了で爆発
		}
	}
	else if (bulletType == BulletType::RAIN && isOrbit)
	{
		// プレイヤー座標を中心に追従
		Vector2 pPos = gInst->GetLpPlayer()->GetPlayerPos();
		centerPos.x = static_cast<float>(pPos.x);
		centerPos.y = static_cast<float>(pPos.y);

		// 角度を更新
		angle += angularSpeed;
		bPos.x = centerPos.x + cosf(angle) * radius;
		bPos.y = centerPos.y + sinf(angle) * radius;

		orbitTime--;
		if (orbitTime <= 0)
		{
			isOrbit = false;
			BlastOn(bPos); // 生存時間終了で爆発
		}
	}
	else
	{
		// 通常弾の移動
		bPos.x += bVec.x * 10.0f; // 速度
		bPos.y += bVec.y * 10.0f;

		aliveCounter--;
		if (aliveCounter <= 0) { BlastOn(bPos); }
	}

	animCounter++;
}
void Bullet::UpdateBlast(void)
{
	blastAnimCounter++;
	if (blastAnimCounter >= BLAST_ANIM_INTERVAL * BLAST_ANIM_MAX) {
		// 状態を終了に移行
		ChangeStatus(Bullet::STATUS::E_STAT_END);
	}
}
void Bullet::UpdateEnd(void)
{
}
// 描画処理
void Bullet::Draw(void)
{
	switch (bNowStat) {
	case Bullet::STATUS::E_STAT_MOVE:
		// 弾の移動
		DrawMove();
		break;
	case Bullet::STATUS::E_STAT_BLAST:
		// 弾の爆発アニメーション
		DrawBlast();
		break;
	case Bullet::STATUS::E_STAT_END:
		DrawEnd();
		break;
	}
}
// 弾の状態毎の描画処理
void Bullet::DrawMove(void)
{
	Vector2 mapSPos = gInst->GetLpStage()->GetMapDispStPos();
	// 座標をint型に変換する
	Vector2 pos;
	pos = AsoUtility::Round(bPos);
	DrawGraph(pos.x - BULLET_SIZE_WID / 2 - mapSPos.x * StageBase::MAP_CHIP_SIZE_WID,
		pos.y - BULLET_SIZE_HIG / 2 - mapSPos.y * StageBase::MAP_CHIP_SIZE_HIG,
		img, true);
}
void Bullet::DrawBlast(void)
{
	int animNo = (blastAnimCounter / BLAST_ANIM_INTERVAL) % BLAST_ANIM_MAX;
	Vector2 mapSPos = gInst->GetLpStage()->GetMapDispStPos();
	// 座標をint型に変換する
	Vector2 pos;
	pos = AsoUtility::Round(blastPos);
	DrawGraph(pos.x - BLAST_SIZE_WID / 2 - mapSPos.x * StageBase::MAP_CHIP_SIZE_WID,
		pos.y - BLAST_SIZE_HIG / 2 - mapSPos.y * StageBase::MAP_CHIP_SIZE_HIG,
		blastImage[animNo], true);
}
void Bullet::DrawEnd(void)
{
}
// 解放処理(最後の１回のみ実行)
bool Bullet::Release(void)
{
	DeleteGraph(img);

	for (int ii = BLAST_ANIM_MAX - 1; ii >= 0; ii--) {
		if (DeleteGraph(blastImage[ii]) == -1)return false;
	}
	return true;
}
/*
 *----------------------------------------------------
 * 弾を生成する
 * Input:
 * Vector2F pos : 弾の座標
 * AsoUtility::DIRECTION dir : 弾の方向
 * Output:
 * 無し
 *----------------------------------------------------
 */
void Bullet::Create(Vector2F pos, AsoUtility::DIRECTION dir)
{
	aliveCounter = ALIVE_TIME; // 生存時間設定
	bPos = pos;
	// 移動の方向から単位ベクトルを設定
	switch (dir) {
	case AsoUtility::DIRECTION::E_DIR_UP:
		bVec = { 0.0f, -1.0f };
		break;
	case AsoUtility::DIRECTION::E_DIR_DOWN:
		bVec = { 0.0f, 1.0f };
		break;
	case AsoUtility::DIRECTION::E_DIR_LEFT:
		bVec = { -1.0f, 0.0f };
		break;
	case AsoUtility::DIRECTION::E_DIR_RIGHT:
		bVec = { 1.0f, 0.0f };
		break;
	}

	

	// 状態を遷移させる
	ChangeStatus(STATUS::E_STAT_MOVE);
}
// 弾の生成が可能かどうか
bool Bullet::IsEnableCreate(void)
{
	return bNowStat == Bullet::STATUS::E_STAT_NONE || bNowStat == Bullet::STATUS::E_STAT_END;
}
/*
 *----------------------------------------------------
 * 指定された状態に状態を遷移させる
 * Input:
 * STATUS stat : 遷移させる弾の状態
 * Output:
 * 無し
 *----------------------------------------------------
 */
void Bullet::ChangeStatus(STATUS stat)
{
	bNowStat = stat;
	// 状態遷移時に必要な初期化処理
	switch (bNowStat) {
	case Bullet::STATUS::E_STAT_MOVE:
		// 弾の移動
		animCounter = 0;
		break;
	case Bullet::STATUS::E_STAT_BLAST:
		// 弾の爆発アニメーション
		blastAnimCounter = 0;
		break;
	case Bullet::STATUS::E_STAT_END:
		break;
	}
}
// 爆発表示開始
void Bullet::BlastOn(Vector2F pos)
{
	blastPos = pos;
	ChangeStatus(Bullet::STATUS::E_STAT_BLAST);
}

void Bullet::CreateOrbit(Vector2F center, float rad, float speed, int time)
{
	centerPos = center;
	radius = rad;
	angularSpeed = speed;
	orbitTime = time;
	angle = 0.0f;
	isOrbit = true;
	bulletType = BulletType::ORBIT;

	bPos.x = center.x + cosf(angle) * radius;
	bPos.y = center.y + sinf(angle) * radius;

	ChangeStatus(STATUS::E_STAT_MOVE);

}

void Bullet::CreateRain(Vector2F center, float rad, float speed, int time)
{
	centerPos = center;
	radius = rad;
	angularSpeed = speed;
	orbitTime = time;
	angle = 0.0f;
	isOrbit = true;
	bulletType = BulletType::RAIN;

	bPos.x = center.x + cosf(-angle) * radius;
	bPos.y = center.y + sinf(angle) * radius;

	ChangeStatus(STATUS::E_STAT_MOVE);

}
