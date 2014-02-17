// Stage1.hpp
#ifndef _MY_STAGE1_H_
#define _MY_STAGE1_H_

#include "Assist.hpp"

#define BOX_MAX 4

/*-----------------------------------------------------------------*
	ステージ１　クラス
	Main.cppでインスタンスが作られ関数が呼ばれる
 ------------------------------------------------------------------*/

class Charactor
{
public:
	Vector3 pos;
	Vector3 speed;
	Vector3 force;

	int onface;

	Charactor();
	virtual void Render(int) = 0;
};

class Me : public Charactor{
public:
	Vector3 angle;

	Me();
	virtual void Render(int);
};

class Progress : public Charactor
{
public:
	int render_f; 				// 存在しているか
	int Direction;				// 進行方向
	int test;
	
    Progress();
	virtual void Render(int);
	void Reset(int);
};

class Stage1 : public StageClass{
private:
	GLfloat light_0_pos[4];

	// own equipment
	Me me;
	Vector3 me_vec;
	float rot;

	// box
	Vector3 pos[BOX_MAX];
	int color_f[BOX_MAX];

	// board
	Board board;

	// key
	int key_on;

	int score;
	char str_score[20];
	
	// timer
	int timer;
	char str_timer[20];

	Progress pro[BOX_MAX];
	Vector3 pro_vec[BOX_MAX];


public:

	Stage1();

	//画面再描画時によばれる(1フレーム毎に呼ばれる)
	virtual void Disp();

	virtual void InitBox(int i);

	// 2D
	virtual void Disp2D(int Width, int Height);

	// input
	virtual void Input(char event, int key, int x, int y);

	virtual bool onFacep(int borad_size, Vector3 speed, Vector3 pro);
	
	// 2つのobjectの位置座標(Vector3)を渡すと、あたっているか判定する(球)
	virtual bool Hitp(Vector3 first, Vector3 second);
	

	// Destructor
	virtual ~Stage1();
};

#endif /* _MY_STAGE1_H_ */
