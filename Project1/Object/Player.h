#pragma once
class Player
{
public:
	Player(void);
	~Player(void);

	void Init(void) ;
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	int image;
};

