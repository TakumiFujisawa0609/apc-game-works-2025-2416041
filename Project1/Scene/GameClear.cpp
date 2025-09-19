#include "GameClear.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>

GameClear::GameClear(void)
{
	imgClear_, imgFramehover_, imgHomeDefault_,
		imgNextDefault_, imgReplayDefault_ = -1;

}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
	imgClear_ = LoadGraph((Application::PATH_SCENE + "Clear_1.png").c_str());
	
}

void GameClear::Update(void)
{

	InputManager& ins = InputManager::GetInstance();
	SceneManager& sce = SceneManager::GetInstance();

}

void GameClear::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0, imgClear_, true);

}

void GameClear::Release(void)
{
	DeleteGraph(imgClear_);
	
}