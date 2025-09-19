#include "GameOver.h"
#include "GameClear.h"
#include "../Manager/InputManager.h"
#include <DxLib.h>



GameOver::GameOver(void)
{
	imgOver_, imgFramehover_, imgHomeDefault_, imgReplayDefault_ = -1;
	homeX_, retryX_ = 0;
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
	//SoundManager& snd = SoundManager::GetInstance();

	imgOver_ = LoadGraph((Application::PATH_SCENE + "Over_1.jpg").c_str());
}

void GameOver::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	SceneManager& sce = SceneManager::GetInstance();
}

void GameOver::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0, imgOver_, true);

}

void GameOver::Release(void)
{
	DeleteGraph(imgOver_);
}


void GameOver::SetPreviousStage(SceneManager::SCENE_ID prevStage)
{
	prevStage_ = prevStage;
}
