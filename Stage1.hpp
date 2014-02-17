// Stage1.hpp
#ifndef _MY_STAGE1_H_
#define _MY_STAGE1_H_

#include "Assist.hpp"

#define BOX_MAX 4

/*-----------------------------------------------------------------*
	���ơ����������饹
	Main.cpp�ǥ��󥹥��󥹤�����ؿ����ƤФ��
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
	int render_f; 				// ¸�ߤ��Ƥ��뤫
	int Direction;				// �ʹ�����
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

	//���̺�������ˤ�Ф��(1�ե졼����˸ƤФ��)
	virtual void Disp();

	virtual void InitBox(int i);

	// 2D
	virtual void Disp2D(int Width, int Height);

	// input
	virtual void Input(char event, int key, int x, int y);

	virtual bool onFacep(int borad_size, Vector3 speed, Vector3 pro);
	
	// 2�Ĥ�object�ΰ��ֺ�ɸ(Vector3)���Ϥ��ȡ������äƤ��뤫Ƚ�ꤹ��(��)
	virtual bool Hitp(Vector3 first, Vector3 second);
	

	// Destructor
	virtual ~Stage1();
};

#endif /* _MY_STAGE1_H_ */
