#include "Application.h"
#include <DxLib.h>

Application* Application::instance_ = nullptr;

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_OBJECT = PATH_DATA + "object/";
const std::string Application::PATH_SCENE = PATH_DATA + "Scene/";
const std::string Application::PATH_STAGE = PATH_DATA + "Stage/";
const std::string Application::PATH_UI = PATH_DATA + "UI/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_SOUND = PATH_DATA + "Sound/";
const std::string Application::PATH_MOVIE = PATH_DATA + "Movie/"; // 動画のパス（必要なら追加）
//const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
//const std::string Application::PATH_MAP_DATA = PATH_DATA + "MapData/MapData.csv";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("Slot Survivor");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	

	// シーン管理初期化
	//SceneManager::CreateInstance();

}

void Application::Run(void)
{
	//InputManager& inputManager = InputManager::GetInstance();
	//SceneManager& sceneManager = SceneManager::GetInstance();

	while (ProcessMessage() == 0)
	{
		Sleep(1);

		currentTime = GetNowCount();

		if (currentTime - lastFrameTime >= FRAME_RATE) {
			lastFrameTime = currentTime;
			frameCnt++;
		}

		//inputManager.Update();

		// Escape キー入力処理
		bool nowEscapePressed = CheckHitKey(KEY_INPUT_ESCAPE) != 0;

		// 一度押された瞬間だけ反応
		if (nowEscapePressed && !prevEscapePressed_) {
			isPauseMenuActive_ = !isPauseMenuActive_;
		}
		prevEscapePressed_ = nowEscapePressed;

		// ポーズ中は更新停止
		if (!isPauseMenuActive_) {
			//sceneManager.Update();
		}

		// 常に描画処理は行う（ポーズ中でも背景描くため）
		//sceneManager.Draw();

		// ポーズオーバーレイ描画
		if (isPauseMenuActive_) {
			DrawPauseOverlay(); // ←下で定義
		}

		CalcFrameRate();

		ScreenFlip();
	}
}

void Application::DrawPauseOverlay()
{
	// 背景（赤い半透明）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, GetColor(255, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// マウス座標
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	
}

void Application::Destroy(void)
{

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// シーン管理解放
	//SceneManager::GetInstance().Destroy();

	// 入力制御解放
	//InputManager::GetInstance().Destroy();

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

void Application::CalcFrameRate(void)
{
	int nDifTime = currentTime - updateFrameRateTime;

	if (nDifTime > 1000) {
		float fFrameCnt = (float)(frameCnt * 1000);

		frameRate = fFrameCnt / nDifTime;

		frameCnt = 0;

		updateFrameRateTime = currentTime;
	}
}

void Application::DrawFrameRate()
{
	DrawFormatString(Application::SCREEN_SIZE_X - 90, 0, GetColor(255, 30, 30), "FPS[%.2f]", frameRate);
}
