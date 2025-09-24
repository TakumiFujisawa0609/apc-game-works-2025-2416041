#include "SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../Manager/StageManager.h"
#include "../Scene/GameClear.h"
#include "../Scene/GameOver.h"
#include "../Manager/SoundManager.h"
#include "../Common/Fader.h"
#include <DxLib.h>
#include "../Scene/Stage/Stage_1.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;
	prevStageId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	isSceneChanging_ = false;

	preTime_ = std::chrono::system_clock::now();

	DoChangeScene(SCENE_ID::TITLE);


}

void SceneManager::Update(void)
{
	if (scene_ == nullptr)
	{
		return;
	}

	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();

	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		//scene_->Update();
	}
}

void SceneManager::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	//scene_->Draw();
	fader_->Draw();
}

void SceneManager::Destroy(void)
{
	if (scene_)
	{
		//scene_->Release();
		delete scene_;
	}

	delete fader_;
	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	waitSceneId_ = nextId;
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;
	prevStageId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;
	deltaTime_ = 1.0f / 60.0f;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	sceneId_ = sceneId;

	if (scene_ != nullptr)
	{
		//scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	//case SCENE_ID::TITLE:
		//scene_ = new TitleScene();
	//	break;
	//case SCENE_ID::SELECT:
		//scene_ = new StageManager();
	//	break;
	case SCENE_ID::STAGE01:
		scene_ = new Stage_1();
		break;
	case SCENE_ID::STAGE02:
		//scene_ = new Stage_2();
		break;
	case SCENE_ID::GAMECLEAR:
		//scene_ = new GameClear();
		break;
	case SCENE_ID::GAMEOVER:
	{
		auto* overScene = new GameOver();
		//overScene->SetPreviousStage(prevStageId_);  // ← ここで記録された前ステージを渡す
		//scene_ = overScene;
		break;
	}
	case SCENE_ID::END:
		///application_->Destroy();
		break;
	case SCENE_ID::MAX:
		// 未使用
		break;
	}

	//scene_->Init();
	ResetDeltaTime();
	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Fade(void)
{
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd())
		{
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd())
		{
			DoChangeScene(waitSceneId_);
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}
}

void SceneManager::SetPrevStageID(SCENE_ID id)
{
	prevStageId_ = id;
}
