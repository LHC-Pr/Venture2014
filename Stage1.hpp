// Stage1.hpp
#ifndef _MY_STAGE1_H_
#define _MY_STAGE1_H_

#include "Assist.hpp"
#include "Charactors.hpp"


#define BOX_MAX 4
#define ENEMY_MAX 5

/*-----------------------------------------------------------------*
	ステージ１　クラス
	Main.cppでインスタンスが作られ関数が呼ばれる
 ------------------------------------------------------------------*/
class Stage1 : public StageClass{
private:

	GLfloat light_0_pos[4];

	// own equipment
	Me me;
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

	Enemy enemy[ENEMY_MAX];
public:

	Stage1();

	//画面再描画時によばれる(1フレーム毎に呼ばれる)
	virtual void Disp();

	virtual void InitBox(int i);

	// 2D
	virtual void Disp2D(int Width, int Height);

	// input
	virtual void Input(char event, int key, int x, int y);

	// Destructor
	virtual ~Stage1();
};

#endif /* _MY_STAGE1_H_ */
