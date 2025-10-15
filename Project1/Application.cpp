#include "Application.h"
#include <DxLib.h>
#include <time.h>
#include "Manager/SceneManager.h"
#include "Manager/InputManager.h"

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_OBJECT = PATH_DATA + "object/";
const std::string Application::PATH_WEAPON = PATH_OBJECT + "weapon/";
const std::string Application::PATH_SCENE = PATH_DATA + "Scene/";
const std::string Application::PATH_STAGE = PATH_DATA + "Stage/";
const std::string Application::PATH_UI = PATH_DATA + "UI/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_SOUND = PATH_DATA + "Sound/";
const std::string Application::PATH_MOVIE = PATH_DATA + "Movie/"; // 動画のパス（必要なら追加）
//const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
//const std::string Application::PATH_MAP_DATA = PATH_DATA + "MapData/MapData.csv";

//コンストラクタ
Application::Application(void)
{
	sceneMana = nullptr;
}

//デストラクタ
Application::~Application(void)
{

}

//初期化処理（最初の一回のみ実行）
bool Application::SystemInit(void)
{
	//システム処理
	SetWindowText("2416041 守口太一朗");
	SetGraphMode(SCREEN_SIZE_WID, SCREEN_SIZE_HIG, 32);
	//ゲームウインドウのサイズと色モードを設定
	ChangeWindowMode(true);

	if (DxLib_Init() == -1) return false;

	// 乱数の初期化
	SRand((unsigned int)time(NULL));

	InputManager::CreateInstance();
	InputManager::GetInstance().Init();

	// インスタンスの生成
	sceneMana = new SceneManager();
	if (sceneMana == nullptr)return false;

	sceneMana->SystemInit();

	return true;
}

// ゲーム起動
void Application::Run(void)
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		Update(); // 更新
		Draw(); // 描画
	}
}

// 更新
void Application::Update(void)
{
	InputManager::GetInstance().Update(); // 入力情報の更新
	sceneMana->Update();
}
// 描画
void Application::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK); // 描画する画面を裏の画面に設定する
	ClearDrawScreen(); // 描画する画面の内容を消去(クリア)する

	sceneMana->Draw();

	ScreenFlip(); // 裏画面と表画面を入れ替える
}

// 解放処理(最後の１回のみ実行)
bool Application::Release(void)
{
	InputManager::GetInstance().Destroy();
	sceneMana->Release();

	delete sceneMana;
	sceneMana = nullptr;

	if (DxLib_End() == -1)return false;
	return true;
}
