
// GameScene class
#include <DxLib.h>
#include "SceneBase.h"
#include "../Application.h"
#include "../Scene/Stage/Stage_1.h"
#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Enemy.h"
#include "../Object/Enemynormal.h"
//#include "EnemyDragon.h"
//#include "EnemyFly.h"
//#include "EnemyFire.h"
//#include "EnemyLizardSmall.h"
//#include "EnemyLizardBig.h"
//#include "EnemyBoss.h"

GameScene::GameScene(void)
{
	stage = nullptr;
	player = nullptr;
	bullet = nullptr;
}

GameScene::~GameScene(void)
{
}

// 初期化処理(最初の１回のみ実行)
bool GameScene::SystemInit(void)
{
	stage = new StageBase();
	if (stage == nullptr)return false;
	player = new Player(this);
	if (player == nullptr)return false;
	bullet = new Bullet(this);
	if (bullet == nullptr)return false;

	if (stage->SystemInit() == -1)return false;
	player->SystemInit();
	bullet->SystemInit();
	return true;
}

// ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
{
	stage->GameInit();
	player->GameInit();
	bullet->GameInit();
	prevShotKey = nowShotKey = 0;
	enCounter = 0;
	nextSceneID = E_SCENE_GAME;
}

// 更新処理
void GameScene::Update(void)
{
	Vector2 oldPos = player->GetPlayerPos(); // 移動前のプレイヤーの位置
	stage->Update();
	player->Update();
	// 弾の発射処理
	if (bullet->IsEnableCreate()) {
		// 発射可能な状態
		prevShotKey = nowShotKey;
		nowShotKey = CheckHitKey(KEY_INPUT_SPACE);
		// ダウントリガーで判定
		if (prevShotKey == 0 && nowShotKey == 1) {
			// 弾の発射(生成する)
			Vector2 vec = player->GetPlayerPos();
			Vector2F vecf(static_cast<float>(vec.x), static_cast<float>(vec.y));
			bullet->Create(vecf, player->GetPlayerDir());
		}
	}
	bullet->Update();

	// 敵の更新
	size_t size = enemys.size(); // 敵のテーブルの要素数を取得
	for (int ii = 0; ii < size; ii++) {
		enemys[ii]->Update();
	}

	// エンカウンター
	if (stage->GetMapType() == StageBase::MAP_TYPE::E_MIYPE_GROUND)enCounter++;
	if (enCounter > ENCOUNT) {

		// 敵の生成
		Enemy* e = nullptr;

		// ランダムに種別を決める
		int rr = GetRand(static_cast<int>(Enemy::ENEMY_TYPE::E_TYPE_MAX) - 1);
		Enemy::ENEMY_TYPE rType = static_cast<Enemy::ENEMY_TYPE>(rr);
		// 種別に対応した派生クラスのインスタンスを生成
		switch (rType) {
		case Enemy::ENEMY_TYPE::E_TYPE_NORMAL:
		e = new Enemynormal();
			break;
		//case EnemyBase::ENEMY_TYPE::E_TYPE_FLY:
		//	e = new EnemyFly();
		//	break;
		//case EnemyBase::ENEMY_TYPE::E_TYPE_FIRE:
		//	e = new EnemyFire();
		//	break;
		//case EnemyBase::ENEMY_TYPE::E_TYPE_LIZARD_SMALL:
		//	e = new EnemyLizardSmall();
		//	break;
		//case EnemyBase::ENEMY_TYPE::E_TYPE_LIZARD_BIG:
		//	e = new EnemyLizardBig();
		//	break;
		//case EnemyBase::ENEMY_TYPE::E_TYPE_DRAGON:
		//	e = new EnemyDragon();
		//	break;
		//case EnemyBase::ENEMY_TYPE::E_TYPE_BOSS:
		//	e = new EnemyBoss();
		//	break;
		}

		if (e != nullptr) {
			e->SystemInit(this);
			e->GameInit();
			// 可変長配列に要素を追加する
			enemys.push_back(e);
			enCounter = 0; // エンカウンターをリセット
		}
	}
	Vector2 playerPos = player->GetPlayerPos(); // 移動後のプレイヤーの位置
	AsoUtility::DIRECTION pdir = player->GetPlayerDir(); // 移動方向を取得

	if (pdir == AsoUtility::DIRECTION::E_DIR_DOWN) {
		// 下移動の場合のみプレイヤーのY座標を画像の下端に設定する
		playerPos.y += (Player::PLAYER_HIG / 2);
	}

	// 移動先が衝突対象地形かを調べる
	if (IsCollisionStage(playerPos)) {
		// 衝突対象地形なので移動不可なので、元の位置にプレイヤーを戻す。
	//	player->SetPlayerPos(oldPos);
	//	if (pdir == AsoUtility::DIRECTION::E_DIR_DOWN) {
	//		Vector2 mp = WorldPos2MapPos(playerPos);
	//		oldPos.y = Stage::MAP_CHIP_SIZE_HIG * mp.y - (Player::PLAYER_HIG / 2) - 1;
	//	}
		player->SetPlayerPos(oldPos);
	}
	else {
		if (pdir == AsoUtility::DIRECTION::E_DIR_LEFT || pdir == AsoUtility::DIRECTION::E_DIR_RIGHT) {
			// 念の為に、足元の座標が移動不可能地形にかかっていないか調べる
			Vector2 wPos = playerPos;
			wPos.y += (Player::PLAYER_HIG / 2);
			if (IsCollisionStage(wPos)) {
				// 足元が衝突対象地形にかかっているので、移動不可として元の位置にプレイヤーを戻す。
				player->SetPlayerPos(oldPos);
			}
		}
	}

	if (pdir == AsoUtility::DIRECTION::E_DIR_DOWN) {
		// 下移動の場合のみプレイヤーのY座標を変更しているので、元に戻す
		playerPos.y -= (Player::PLAYER_HIG / 2);
	}
	MapScrollProc();

	// 衝突判定
	CollisionCheck();
	if (player->GetAlive()) {
		// 死亡した敵データを消去する
		for (int ii = (int)size; ii > 0; ii--) {
			if (!enemys[ii - 1]->GetAlive()) {
				enemys[ii - 1]->Release();
				delete enemys[ii - 1];
				//std::vector<Enemy*>::iterator itr;
				std::vector<Enemy*>::iterator itr;
				itr = enemys.begin() + (ii - 1);
				enemys.erase(itr);
			}
		}
	}
	else {
		// プレイヤーのＨＰが 0 になったらゲームオーバー
		EraseEnemys();
		nextSceneID = E_SCENE_GAMEOVER;
	}
	

}

// 描画処理
void GameScene::Draw(void)
{
	stage->Draw();
	player->Draw();
	// 敵の描画
	size_t size = enemys.size();
	for (int ii = 0; ii < size; ii++) {
		enemys[ii]->Draw();
	}
	bullet->Draw();
	DrawBox(0, 0, Application::SCREEN_SIZE_WID, 20, GetColor(0, 0, 0), true);
	int php = player->GetHp();
	DrawFormatString(32, 0, GetColor(0xff, 0xff, 0xff), "プレイヤーＨＰ：%3d", php);
	//-----------------------------------------------------------------------
	// デバッグ用
	Vector2 pPos = player->GetPlayerPos();
	Vector2 mPos = WorldPos2MapPos(pPos);
	DrawFormatString(0, 32, GetColor(0xff, 0xff, 0xff), "プレイヤー座標：(%d, %d)→(%d, %d)",
		pPos.x, pPos.y, mPos.x, mPos.y);
	int num = (int)enemys.size();
	DrawFormatString(0, 48, GetColor(0xff, 0xff, 0xff), "敵の数：%d", num);
	//-----------------------------------------------------------------------
	// 画面のスクロール範囲(デバッグ用)
	DrawBox(SCROLL_AREA_WID, SCROLL_AREA_HIG,
		Application::SCREEN_SIZE_WID - SCROLL_AREA_WID, Application::SCREEN_SIZE_HIG - SCROLL_AREA_HIG,
		GetColor(0, 0, 255), false);
}

// 解放処理(最後の１回のみ実行)
bool GameScene::Release(void)
{
	// 敵の解放
	EraseEnemys();
	// インスタンスの解放
	bullet->Release();
	delete bullet;
	bullet = nullptr;
	player->Release();
	delete player;
	player = nullptr;
	stage->Release();
	delete stage;
	stage = nullptr;
	return true;
}

void GameScene::MapScrollProc(void)
{
	Vector2 mapStPos = stage->GetMapDispStPos();
	Vector2 playerPos = player->GetPlayerPos();
	// 現在のマップの端のマップ座標ではなく、ワールド座標を求める
	int leftEdge = mapStPos.x * StageBase::MAP_CHIP_SIZE_WID;
	int rightEdge = mapStPos.x * StageBase::MAP_CHIP_SIZE_WID + Application::SCREEN_SIZE_WID - 1;
	int upperEdge = mapStPos.y * StageBase::MAP_CHIP_SIZE_HIG;
	int lowerEdge = mapStPos.y * StageBase::MAP_CHIP_SIZE_HIG + Application::SCREEN_SIZE_HIG - 1;
	// 画面右端のマップチップ座標内に達しているか調べる

	if (playerPos.x >= rightEdge - SCROLL_AREA_WID) {
		// 表示画面の右端に達しているので、プレイヤー位置が横中央にくる表示開始位置を求める
		int dx = playerPos.x - Application::SCREEN_SIZE_WID / 2;
		dx /= StageBase::MAP_CHIP_SIZE_WID;
		dx -= mapStPos.x;
		if (dx < 0)dx *= -1;
		stage->MoveMapToRight(dx);
	}

	// 画面左端のマップチップ座標内に達しているか調べる
	if (playerPos.x <= leftEdge + SCROLL_AREA_WID) {
		// 表示画面の左端に達しているので、プレイヤー位置が横中央にくる表示開始位置を求める
		int dx = playerPos.x - Application::SCREEN_SIZE_WID / 2;
		dx /= StageBase::MAP_CHIP_SIZE_WID;
		dx -= mapStPos.x;
		if (dx < 0)dx *= -1;
		stage->MoveMapToLeft(dx);
	}

	// 画面上端のマップチップ座標内に達しているか調べる
	if (playerPos.y <= upperEdge + SCROLL_AREA_HIG) {
		// 表示画面の上端に達しているので、プレイヤー位置が縦中央にくる表示開始位置を求める
		int dy = playerPos.y - Application::SCREEN_SIZE_HIG / 2;
		dy /= StageBase::MAP_CHIP_SIZE_HIG;
		dy -= mapStPos.y;
		if (dy < 0)dy *= -1;
		stage->MoveMapToUpper(dy);
	}

	// 画面下端のマップチップ座標内に達しているか調べる
	if (playerPos.y >= lowerEdge - SCROLL_AREA_HIG) {
		// 表示画面の下端に達しているので、プレイヤー位置が横中央にくる表示開始位置を求める
		int dy = playerPos.y - Application::SCREEN_SIZE_HIG / 2;
		dy /= StageBase::MAP_CHIP_SIZE_HIG;
		dy -= mapStPos.y;
		if (dy < 0)dy *= -1;
		stage->MoveMapToDown(dy);
	}
}

/*
 *----------------------------------------------------
 * 敵データテーブルを空にする
 * Input:
 * 無し
 * Output:
 * 無し
 *----------------------------------------------------
 */
void GameScene::EraseEnemys(void)
{
	size_t size = enemys.size();
	for (int ii = (int)size; ii > 0; ii--) {
		enemys[ii - 1]->Release();
		// インスタンスの解放
		delete enemys[ii - 1];
	}
	enemys.clear(); // 可変長配列を空にする(サイズを0にする)
}

/*
 *----------------------------------------------------
 * ワールド座標をマップ座標に変換する
 * Input:
 * Vector2 wpos : ワールド座標
 * Output:
 * Vector2 : マップ座標
 *----------------------------------------------------
 */
Vector2 GameScene::WorldPos2MapPos(Vector2 wpos)
{
	Vector2 mpos;
	mpos.x = mpos.y = 0;
	mpos.x = wpos.x / StageBase::MAP_CHIP_SIZE_WID;
	mpos.y = wpos.y / StageBase::MAP_CHIP_SIZE_HIG;
	return mpos;
}

/*
 *----------------------------------------------------
 * ワールド座標の指す地形は衝突対象か調べる(衝突判定)
 * Input:
 * Vector2 worldPos : ワールド座標
 * Output:
 * bool : true = 衝突対象 / false = 衝突対象ではない(通行可能)
 *----------------------------------------------------
 */
bool GameScene::IsCollisionStage(Vector2 worldPos)
{
	// チップ対応の衝突対象情報テーブル
	bool chipCollisionInfoTbl[StageBase::MAP_CHIP_ALL_NUM] = {
	false, false, true, true, true, false, false, false, false,
	false, true, true, true, false, false, false, true, true,
	true, true, true, false, false, false, false, false, false,
	true, true, true, false, false, false, false, false, false,
	true, true, true, false, false, false, false, false, false,
	false, false, false, false, false, false, true, true, true,
	false, false, false, false, false, false, true, true, true,
	false, false, false, false, false, false, true, true, true,
	false, false, false, false, false, false, true, true, true,
	};
	if (worldPos.x >= StageBase::MAP_SIZE_WID || worldPos.y >= StageBase::MAP_SIZE_HIG) {
		// マップ範囲外の座標
		return true;
	}
	Vector2 mpos = WorldPos2MapPos(worldPos);
	int chipNo = stage->GetMapChipNo(mpos);
	bool rb = chipCollisionInfoTbl[chipNo];
	return rb;
}

/*
 *----------------------------------------------------
 * 衝突判定
 * Input:
 * 無し
 * Output:
 * 無し
 *----------------------------------------------------
 */
void GameScene::CollisionCheck(void)
{
	// 弾の情報
	Vector2F pos = bullet->GetBulletPos();
	Vector2 bPos = AsoUtility::Round(pos);
	Vector2 bSize = { Bullet::BULLET_SIZE_WID, Bullet::BULLET_SIZE_HIG };

	// プレイヤーの情報
	Vector2 pPos = player->GetPlayerPos();
	Vector2 pSize = { Player::PLAYER_WID, Player::PLAYER_HIG };

	// 敵の数だけチェックを行う
	size_t size = enemys.size();
	for (int ii = 0; ii < size; ii++) {
		pos = enemys[ii]->GetEnemyPos();
		if (!enemys[ii]->GetAlive())continue;
		Vector2 ePos = AsoUtility::Round(pos);
		Vector2 eSize = enemys[ii]->GetEnemySize();

		// 敵とプレイヤーの弾の衝突判定
		if (bullet->IsShotState()) {
			// 弾を発射している
			if (CollisionCheckRectCenter(bPos, bSize, ePos, eSize)) {
				//enemys[ii]->SetAlive(false);
				enemys[ii]->SetDamege(4);
				bullet->BlastOn(bullet->GetBulletPos());
			}
		}

		if (!enemys[ii]->GetAlive())continue;
		// 敵とプレイヤーの衝突判定
		if (CollisionCheckRectCenter(pPos, pSize, ePos, eSize)) {
			player->SetDamage(1); // プレイヤーにダメージを与える
		}

		if (!player->GetAlive()) {
			break;
		}
	}
}

/*
 *----------------------------------------------------
 * 中心座標から衝突判定を行う
 * Input:
 * Vector2 centerPos1 : 判定対象１の中心座標
 * Vector2 size1 : 判定対象１の縦横のサイズ
 * Vector2 centerPos2 : 判定対象２の中心座標
 * Vector2 size2 : 判定対象２の縦横のサイズ
 * Output:
 * bool : true = 衝突している / false = 衝突していない
 *----------------------------------------------------
 */
bool GameScene::CollisionCheckRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2)
{
	// １つ目の矩形の座標を求める
	Vector2 stPos1 = centerPos1;
	Vector2 edPos1 = centerPos1;
	stPos1.x -= (size1.x / 2);
	stPos1.y -= (size1.y / 2);
	edPos1.x += (size1.x / 2);
	edPos1.y += (size1.y / 2);
	// ２つ目の矩形の座標を求める
	Vector2 stPos2 = centerPos2;
	Vector2 edPos2 = centerPos2;
	stPos2.x -= (size2.x / 2);
	stPos2.y -= (size2.y / 2);
	edPos2.x += (size2.x / 2);
	edPos2.y += (size2.y / 2);
	// 矩形同士の衝突判定
	// 矩形１の左より矩形２の右が大きい かつ
	// 矩形１の右が矩形２の左より大きい かつ
	// 矩形１の上より矩形２の下が大きい かつ
	// 矩形１の下が矩形２の上より大きい
	if (stPos1.x < edPos2.x &&
		edPos1.x > stPos2.x &&
		stPos1.y < edPos2.y &&
		edPos1.y > stPos2.y) {
		return true;
	}
	return false;
}