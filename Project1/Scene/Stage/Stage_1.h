#pragma once
#include "../../Scene/SceneBase.h"
class SceneManager;


class Stage_1 : public SceneBase
{
public:
	Stage_1(void);
	~Stage_1(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
};

