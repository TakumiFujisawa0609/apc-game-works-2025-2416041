#pragma once
class Enemy
{
public:
	Enemy(void);
	~Enemy(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	int image;
};

