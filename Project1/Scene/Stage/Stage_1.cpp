#include <DxLib.h>
#include "Stage_1.h"
#include "../TitleScene.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Application.h"

Stage_1::Stage_1(void)
{
}

Stage_1::~Stage_1(void)
{
}

void Stage_1::Init(void)
{
	LoadGraph((
		Application::PATH_SCENE + "Stage_1.png").c_str());
}

void Stage_1::Update(void)
{
}

void Stage_1::Draw(void)
{
	DrawGraph(0, 0, LoadGraph((Application::PATH_SCENE + "Stage_1.png").c_str()), true);
}

void Stage_1::Release(void)
{
}
