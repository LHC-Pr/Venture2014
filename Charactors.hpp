// Charactors.hpp
#ifndef _CHARACTORS_HPP_
#define _CHARACTORS_HPP_

#include "Assist.hpp"

#define BOARD_SIZE 24			// 偶数にすること(bit演算使用)

#define ME_MOVEMENT 0.2			// 自機の移動量
#define BOX_CAMRA 2.0			// 自機のカメラの回転量

#define GRAVITY 0.02
#define JUMP_SPEED 0.3

#define GAME_TIME 4000

#define KEY_A 1
#define KEY_D 2
#define KEY_W 4
#define KEY_S 8
#define KEY_SPACE 16

static int keys[] = {
	'a',
	'd',
	'w',
	's',
	' '
};

class Charactor
{
public:
	Vector3 pos;
	Vector3 speed;
	Vector3 force;

	int onface;

	Charactor();
	virtual void Render(int) = 0;
	virtual void PhyCal(int) = 0;
};

class Me : public Charactor{
public:
	Vector3 vec;
	Vector3 angle;

	Me();
	virtual void Render(int);
	virtual void PhyCal(int);
	virtual void MoveCal(int);
};

class Enemy : public Charactor
{
public:
	int Direction;
	Enemy();
	virtual void Render(int);
	virtual void PhyCal(int);
};

class Progress : public Enemy
{
public:
	bool render_f; 				// 存在しているか
	
    Progress();
	virtual void Render(int);
	void Reset(Vector3);
	bool renderp();
	void erase();
};


#endif /* _CHARACTORS_HPP_*/
