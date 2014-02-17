// Assist.hpp
#ifndef _MY_ASSIST_H_
#define _MY_ASSIST_H_

#include <GL/freeglut.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>

#define SC_INPUT_KEY_DOWN 'n'
#define SC_INPUT_KEY_UP 'f'

struct Vector2;
struct Vector3;
	
struct Vector2
{
	float x, y;
	
	operator float* (){
		return (float*)&x;
	}
	operator const float*() const{
		return (const float*)&x;
	}
};

struct Vector3
{
	float x, y, z;
	operator float *(){ 		// *(Vector3)
		return (float*)&x;
	}
	
	operator const float *() const{
		return (const float*)&x;
	}

	Vector3 &operator +=(const Vector3 &in){
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;
		return *this;
	}

	Vector3 &operator -=(const Vector3 &in){
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;
		return *this;
	}

	Vector3 operator + (const Vector3 &in){
		Vector3 v = *this;
		v += in;
		return v;
	}

	Vector3 operator - (const Vector3 &in){
		Vector3 v = *this;
		v -= in;
		return v;
	}

	Vector3 &operator = (const float &in){
		x = in;
		y = in;
		z = in;
		return *this;
	}

	Vector3 operator * (const float &in){
		Vector3 out;
		out.x = this->x * in;
		out.y = this->y * in;
		out.z = this->z * in;
		return out;
	}
};

class StageClass
{
public:
	// StageClass();

	// called on the screen redraw time
	virtual void Disp() = 0;

	virtual void Disp2D(int WindowWidth, int WindowHeight) = 0;

	virtual void Input(char event, int key, int x, int y)
	{
		switch(event){
		case SC_INPUT_KEY_UP:
			break;
		case SC_INPUT_KEY_DOWN:
			break;
		}
	}
	
	virtual ~StageClass(){};
};


// 床

class Board{
private:
	GLuint display_list;
	void setRender(float u, float v); // make disp;ay list
public:
	Vector2 size;
	Vector3 pos;
	Vector3 vertex[4];
	Vector3 tmp;

	Board(float u, float v);
	void render();
};

// これを実行した次のDisp終了時に、ステージが変更される。
// Definition Main
// void changeStage(StageClass *next);

// char* "FPS: --.-" 
char* fps();

// set default material (color white)
void setDefaultMaterial();

// draw char (GLUT_BITMAP_TIMES_ROMAN_24)
void drawString(int x, int y, char *string, void *font);



/*-----------------------------------------------------------------------------------*
	キー入力補助
	KeyBuf: キーの入力状態をもつ変数。Keys配列の番号と対応するビットがそのキーの状態を表す。
	key: 状態を変更したいキー、または調べたいキーを指定する。
	onoff: 状態変更。true=押された false=離された
	Keys: 状態を知りたいキーの配列。
	size: Keysのサイズ。
 *-----------------------------------------------------------------------------------*/

bool setKeyState(int *KeyBuf, int key, bool onoff, int *Keys, int size);

bool getKeyState(int *KeyBuf, int key, int *Keys, int size);


#endif /* _MY_ASSIST_H_ */
