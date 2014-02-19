// Stage1.cpp
#include "Stage1.hpp"
#include <time.h>

Stage1::Stage1() : board(BOARD_SIZE, BOARD_SIZE)
{
	me.vec.x = 0.0;
	me.vec.y = 0.0;
	me.vec.z = 1.0;
	me.pos.y = 2.0;
	for (int i = 0; i < ENEMY_MAX; ++i){
		enemy[i].pos.y = 10.0;
		enemy[i].pos.x = (rand() & BOARD_SIZE) - (BOARD_SIZE >> 1);
		enemy[i].pos.z = (rand() & BOARD_SIZE) - (BOARD_SIZE >> 1);
	}

	rot = 0.0;

	// box
	for(int i = 0; i < BOX_MAX; ++i){
		InitBox(i);
		color_f[i] = 0;
	}

	//ライトの位置セット
	light_0_pos[0] = 1.0;		// init 1
	light_0_pos[1] = 10.0; 		// init 10
	light_0_pos[2] = 5.0;		// init 5
	light_0_pos[3] = 1.0;

	key_on = 0;

	score = 0;
	str_score[0] = '\0';

	timer = GAME_TIME;
	str_timer[0] = '\0';

}

void Stage1::InitBox(int i)
{
	// boxの位置をランダムで配置
	pos[i].x = ((float)rand() / (float)RAND_MAX) * BOARD_SIZE - ((int)BOARD_SIZE >> 1);
	pos[i].y = 3.0;
	pos[i].z = ((float)rand() / (float)RAND_MAX) * BOARD_SIZE - ((int)BOARD_SIZE >> 1);
}

void Stage1::Disp()
{
	glLoadIdentity();


	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);

	// camera
	Vector3 campos = me.pos - me.vec * 20.0;
	campos.y += 8.0;			// 高さをつける

	gluLookAt(campos.x, campos.y, campos.z,
			  me.pos.x, me.pos.y, me.pos.z, 0.0, 1.0, 0.0);

	// move
	me.MoveCal(key_on);

	// 当たり判定、物理処理
	// 使うと入力がうまくいかない
	// me.PhyCal();
	{
		if(me.pos.y < -30.0){
			me.pos = 0.0;
			me.pos.y = 10.0;
		}

		me.speed.y -= GRAVITY;	// 毎フレーム毎に高さを減らす

		Vector3 speed = me.speed;

		// 何をしているのかわからない
		if( (me.force.x < 0 && speed.x > me.force.x) ||
			(me.force.x > 0 && speed.x < me.force.x) ){
			speed.x = me.force.x;
		}
		if( (me.force.y < 0 && speed.y > me.force.y) ||
			(me.force.y > 0 && speed.y < me.force.y) ){
			speed.y = me.force.y;
		}
		if( (me.force.z < 0 && speed.z > me.force.z) ||
			(me.force.z > 0 && speed.z < me.force.z) ){
			speed.z = me.force.z;
		}

		if(onFacep(BOARD_SIZE, speed, me.pos)){
			// 床を貫通する早さの時
			speed.y = -me.pos.y + 0.5;
			me.speed = 0.0;		// スピードを0にして床の上にとどまる
			me.onface = 1;			// 床の上に居ることのフラグ(=1の時jump可能)
		}else{
			me.onface = 0;
		}

		me.pos += speed;
	}

	//床表示
	board.render();

	
	
	// 自分を表示
	me.Render(rot);

	// box
	setDefaultMaterial();
	GLfloat color[BOX_MAX][4];
	// 色の指定
	for (int i = 0; i < BOX_MAX; ++i){
		// 最初は全て赤で表示
		color[i][0] = 1.0;
		color[i][1] = 0.0;
		color[i][2] = 0.0;
		color[i][3] = 1.0;
	}

	for (int i = 0; i < BOX_MAX; ++i){
		// color_fが1の時は自機が接触したことを表すので、アイテムboxを黒に変える
		if(color_f[i] == 1 ){
			color[i][0] = 0.0;
		}
	}

	for(int i = 0; i < BOX_MAX; ++i){
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color[i]);

		glPushMatrix();
		glTranslatef(pos[i].x, pos[i].y, pos[i].z);
		glRotatef(rot, 0.0, 1.0, 0.0);
		glutSolidCube(1.0);
		glPopMatrix();

		Vector3 hit = pos[i] - me.pos;

		// hit.y += 0.5;

		// 当たり判定は球
		if(Hitp(pos[i], me.pos)){
			me.pos.y = pos[i].y - 1.0; // 自機がboxを超えないようにする
			color_f[i] = 1;			   // 次のフレームに色を変えるためのフラグ
			if(!pro[i].renderp()){  // 進捗がまだ出ていない(初めてこのboxに自機が触れた)なら
				pro[i].Reset(pos[i]);
			}
		}
	}

	// progress

	for (int i = 0; i < BOX_MAX; ++i){
		if(pro[i].renderp()){

			pro[i].speed.y -= GRAVITY;
			Vector3 pro_speed = pro[i].speed;

			if( (pro[i].force.x < 0 && pro_speed.x > pro[i].force.x) ||
				(pro[i].force.x > 0 && pro_speed.x < pro[i].force.x) ){
				pro_speed.x = pro[i].force.x;
			}

			if( (pro[i].force.y < 0 && pro_speed.y > pro[i].force.y) ||
				(pro[i].force.y > 0 && pro_speed.y < pro[i].force.y) ){
				pro_speed.y = pro[i].force.y;
			}

			if( (pro[i].force.z < 0 && pro_speed.z > pro[i].force.z) ||
				(pro[i].force.z > 0 && pro_speed.z < pro[i].force.z) ){
				pro_speed.z = pro[i].force.z;
			}

			if(onFacep(BOARD_SIZE, pro_speed, pro[i].pos)){
				pro_speed.y = -pro[i].pos.y + 0.5;
				pro[i].speed = 0.0;
				pro[i].onface = 1;
			}else{
				pro[i].onface = 0;
			}

			pro[i].pos += pro_speed;

			switch(pro[i].Direction){
			case 0:
				pro[i].pos.x +=  ME_MOVEMENT / 3.0;
				break;
			case 1:
				pro[i].pos.z +=  ME_MOVEMENT / 3.0;
				break;
			case 2:
				pro[i].pos.x -=  ME_MOVEMENT / 3.0;
				break;
			case 3:
				pro[i].pos.z -=  ME_MOVEMENT / 3.0;
				break;
			}

			// hit_pro.y += 0.5;

			if(Hitp(pro[i].pos, me.pos)){
				InitBox(i);

				//得点加算
				if(timer >= 0){
					score++;
				}

				sprintf(str_score, "SCORE : %d", score );
				pro[i].erase();
				color_f[i] = 0;

			}else if(pro[i].pos.y < -30.0){
				InitBox(i);
				color_f[i] = 0;
				pro[i].erase();
			}

			pro[i].Render(rot);
		}
	}

	// enemy
	for (int i = 0; i < ENEMY_MAX; ++i){
		enemy[i].PhyCal(rot);
	}

	for (int i = 0; i < ENEMY_MAX; ++i){
		if(Hitp(me.pos, enemy[i].pos)){
			me.speed.y += JUMP_SPEED;
		}
	}

	rot++;						// boxの回転角度

	if(timer-- < 0){
		sprintf(str_timer, "TIME OVER");

	}else{
		sprintf(str_timer, "TIME : %d", timer);
	}
}

void Stage1::Disp2D(int Width, int Height)
{
	glColor4f(0.0, 0.0, 0.0, 1.0);

	drawString(8, Height-30, fps(), GLUT_BITMAP_TIMES_ROMAN_24);
	drawString(8, Height-50, str_score, GLUT_BITMAP_TIMES_ROMAN_24);
	drawString(8, Height-70, str_timer, GLUT_BITMAP_TIMES_ROMAN_24);
}

void Stage1::Input(char event, int key, int x, int y){
	switch(event){
	case SC_INPUT_KEY_DOWN:
		setKeyState(&key_on, key, true, keys, sizeof(keys));
		break;
	case SC_INPUT_KEY_UP:
		setKeyState(&key_on, key, false, keys, sizeof(keys));
		break;
	}
}


Stage1::~Stage1(){
	std::cout << "~Stage1()" << std::endl;
}
