#pragma once

#include "../../StDefineData.h"
#include "../../Common/Geometry.h"]
#include "../../Application.h"

class StageBase
{
public:
	static constexpr int MAP_CHIP_SIZE_WID = 32; // マップチップ
	static constexpr int MAP_CHIP_SIZE_HIG = 32; // マップチップの縦サイズ
	static constexpr int MAP_CHIP_NUMS_X = 9; // マップチップ画像内の素材の横数
	static constexpr int MAP_CHIP_NUMS_Y = 9; // マップチップ画像内の素材の縦数
	static constexpr int MAP_CHIP_ALL_NUM = (MAP_CHIP_NUMS_X * MAP_CHIP_NUMS_Y);

	// マップチップ画像内の素材の総数
	static constexpr int MAP_GROUND_NUM_X = 30; // 地上のマップサイズ横
	static constexpr int MAP_GROUND_NUM_Y = 30; // 地上のマップサイズ縦

	static constexpr int DSP_CHIP_NUM_X = Application::SCREEN_SIZE_WID / MAP_CHIP_SIZE_WID;
	static constexpr int DSP_CHIP_NUM_Y = Application::SCREEN_SIZE_HIG / MAP_CHIP_SIZE_HIG + 1;

	static constexpr int MAP_SIZE_WID = MAP_CHIP_SIZE_WID * MAP_GROUND_NUM_X; // マップの横サイズ
	static constexpr int MAP_SIZE_HIG = MAP_CHIP_SIZE_HIG * MAP_GROUND_NUM_Y; // マップの縦サイズ

	static constexpr int MAP_MAX_NUM_X = 50;
	static constexpr int MAP_MAX_NUM_Y = 50;

	static constexpr int  MAP_CHIP_STAIR_NO = 15;

	bool LoadGroundData(void); // 外部ファイルからマップデータを読み込む

	// ゲッター・セッター関数
	void MoveMapToUpper(int mov); // マップの上方向に移動(画面は下にスクロール)
	void MoveMapToDown(int mov); // マップの下方向に移動(画面は上にスクロール)
	void MoveMapToLeft(int mov); // マップの左方向に移動(画面は右にスクロール)
	void MoveMapToRight(int mov); // マップの右方向に移動(画面は左にスクロール)



	enum  MAP_TYPE {
		E_MTYPE_NON = -1,
		E_MIYPE_GROUND,

		E_MTYPE_MAX,
	};

	StageBase(void); // コンストラクタ
	~StageBase(void); // デストラクタ

	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)



	Vector2 GetMapDispStPos(void) { return mapDispStPos; }
	int GetMapChipNo(Vector2 mPos);
	Vector2 GetDispMapSize(void) { return dispMapSize; }
	StageBase::MAP_TYPE GetMapType(void) { return mapType; }
	bool IsStair(Vector2 mapPos) {
		if (GetMapChipNo(mapPos) == MAP_CHIP_STAIR_NO)return true;
		return false;
	}

	void ChangeMap(MAP_TYPE mtype);

private:

	int imgMapChipArray[MAP_CHIP_ALL_NUM]; // マップチップの画像ハンドル番号テーブル

	int dispMapDat[MAP_MAX_NUM_Y][MAP_MAX_NUM_X];

	Vector2 dispMapSize;

	MAP_TYPE mapType;

	// 地上マップデータ
	int groundMapDat[MAP_GROUND_NUM_Y][MAP_GROUND_NUM_X];

	Position2 mapDispStPos; // マップ表示開始座標 

	void ClearDispMap(void);

};