#include <DxLib.h>
#include <fstream> 
#include <string>
#include <iostream> 
#include <sstream>
#include <vector>
#include "../../Application.h"
#include "../../Common/AsoUtility.h"
#include "StageBase.h"


StageBase::StageBase(void)
{

}
StageBase::~StageBase(void)
{
}
// 初期化処理(最初の１回のみ実行)
bool StageBase::SystemInit(void)
{
	// マップチップデータを読み込む
	int err = LoadDivGraph(((Application::PATH_DATA + "map.bmp").c_str()), MAP_CHIP_ALL_NUM,
		MAP_CHIP_NUMS_X, MAP_CHIP_NUMS_Y,
		MAP_CHIP_SIZE_WID, MAP_CHIP_SIZE_HIG, imgMapChipArray);
	if (err == -1)return false;
	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void StageBase::GameInit(void)
{
	LoadGroundData(); // マップデータを読み込む 

	ChangeMap(MAP_TYPE::E_MIYPE_GROUND);

	mapDispStPos.x = mapDispStPos.y = 0;
}
// 更新処理
void StageBase::Update(void)
{
}
// 描画処理
void StageBase::Draw(void)
{
	// まず背景を黒で塗りつぶす
	DrawBox(0, 0, Application::SCREEN_SIZE_WID, Application::SCREEN_SIZE_HIG, GetColor(0, 0, 0), true);

	for (int yy = 0; yy < dispMapSize.y; yy++) {
		if (yy >= DSP_CHIP_NUM_Y)break;
		for (int xx = 0; xx < dispMapSize.x; xx++) {
			if (xx >= DSP_CHIP_NUM_X)break;
			int chip = dispMapDat[mapDispStPos.y + yy][mapDispStPos.x + xx];
			int dx = MAP_CHIP_SIZE_WID * xx;
			int dy = MAP_CHIP_SIZE_HIG * yy;
			DrawGraph(dx, dy, imgMapChipArray[chip], true);
		}
	}


}
// 解放処理(最後の１回のみ実行)
bool StageBase::Release(void)
{
	// マップチップデータの解放
	for (int ii = MAP_CHIP_ALL_NUM; ii > 0; ii--) {
		if (DeleteGraph(imgMapChipArray[ii - 1]) == -1)return false;
	}
	return true;
}
// マップの上方向に移動(画面は下にスクロール)
void StageBase::MoveMapToUpper(int mov)
{
	mapDispStPos.y -= mov;
	if (mapDispStPos.y < 0)
	{
		mapDispStPos.y = 0;
	}
}
// マップの下方向に移動(画面は上にスクロール)
void StageBase::MoveMapToDown(int mov)
{
	mapDispStPos.y += mov;
	if (mapDispStPos.y + DSP_CHIP_NUM_Y >= MAP_GROUND_NUM_Y)
		mapDispStPos.y = MAP_GROUND_NUM_Y - DSP_CHIP_NUM_Y;

}
// マップの左方向に移動(画面は右にスクロール)
void StageBase::MoveMapToLeft(int mov)
{
	mapDispStPos.x -= mov;
	if (mapDispStPos.x < 0)mapDispStPos.x = 0;
}
// マップの右方向に移動(画面は左にスクロール)
void StageBase::MoveMapToRight(int mov)
{
	mapDispStPos.x += mov;
	if (mapDispStPos.x + DSP_CHIP_NUM_X < MAP_GROUND_NUM_X)
		mapDispStPos.x = MAP_GROUND_NUM_X - DSP_CHIP_NUM_X;
}

// 外部ファイルからマップデータを読み込む処理
bool StageBase::LoadGroundData(void)
{
	// マップデータ読み込みバッファを初期化
//	for (int yy = 0; yy < MAP_GROUND_NUM_Y; yy++) {
//		for (int xx = 0; xx < MAP_GROUND_NUM_X; xx++) {
//			groundMapDat[yy][xx] = -1;
//		}
//	}
	memset((int*)&groundMapDat[0], -1, sizeof(int) * (MAP_GROUND_NUM_X * MAP_GROUND_NUM_Y));

	std::ifstream ifs = std::ifstream((Application::PATH_DATA + "Ground2.csv").c_str());
	if (!ifs)return false;

	// ファイルを1行ずつ読み込む
	std::string line; // 1行の文字情報
	std::vector<std::string> strSplit;// 1文字情報
	int chipNo = 0;
	int yy = 0;
	while (getline(ifs, line)) {
		// 1行の情報 string を ifstream の仲間に変換する
		strSplit = AsoUtility::Split(line, ',');
		for (int xx = 0; xx < strSplit.size(); xx++) {
			// string から int に変換する
			chipNo = stoi(strSplit[xx]);
			// 地上マップデータ(2次元配列)にマップチップ番号を格納する
			groundMapDat[yy][xx] = chipNo;
		}
		yy++;
	}
	return true;
}


int StageBase::GetMapChipNo(Vector2 mPos) { return dispMapDat[mPos.y][mPos.x]; }

void StageBase::ChangeMap(MAP_TYPE mtype)
{
	ClearDispMap();

	mapType = mtype;

	switch (mapType) {
	case MAP_TYPE::E_MIYPE_GROUND:
		dispMapSize.x = MAP_GROUND_NUM_X;
		dispMapSize.y = MAP_GROUND_NUM_Y;

		for (int yy = 0; yy < dispMapSize.y; yy++) {
			for (int xx = 0; xx < dispMapSize.x; xx++) {
				dispMapDat[yy][xx] = groundMapDat[yy][xx];
			}
		}
		break;
	}
}
void StageBase::ClearDispMap(void)
{
	for (int yy = 0; yy < MAP_MAX_NUM_Y; yy++) {
		for (int xx = 0; xx < MAP_MAX_NUM_X; xx++) {
			dispMapDat[yy][xx] = -1;
		}
	}
}