// Stage1.cpp#include "Stage1.hpp"#include <time.h>#define BOARD_SIZE 24			// 偶数にすること(bit演算使用)#define ME_MOVEMENT 0.2			// 自機の移動量#define BOX_CAMRA 2.0			// 自機のカメラの回転量#define GRAVITY 0.02#define JUMP_SPEED 0.3#define GAME_TIME 4000#define KEY_A 1#define KEY_D 2#define KEY_W 4#define KEY_S 8#define KEY_SPACE 16static int keys[] = {	'a',	'd',	'w',	's',	' '};Charactor::Charactor(){	pos = 0.0;	speed = 0.0;	force = 0.0;	vec = 0.0;}Me::Me(){	angle = 0.0;	// offset = 0.0;}void Me::Render(int rot){	// box color	setDefaultMaterial();	GLfloat color[4] = { 1.0, 1.0, 0.0, 1.0 };	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);	glPushMatrix();	glTranslated(pos.x, pos.y, pos.z);	glRotatef(rot, 0.0, 1.0, 0.0);	glutSolidCube(1.0);	glPopMatrix();}Progress::Progress(){	pos = 0.0;	speed = 0.0;	force = 0.0;	render_f = 0;}void Progress::Render(int rot){		// box color	setDefaultMaterial();	GLfloat color[4] = { 0.0, 1.0, 0.0, 1.0 };	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);	glPushMatrix();	glTranslated(pos.x, pos.y, pos.z);	glRotatef(rot, 0.0, 1.0, 0.0);	glutSolidCube(1.0);	glPopMatrix();}Stage1::Stage1() : board(BOARD_SIZE, BOARD_SIZE){	me.vec.x = 0.0;	me.vec.y = 0.0;	me.vec.z = 1.0;	me.pos.y = 2.0;	rot = 0.0;	// box	for(int i = 0; i < BOX_MAX; ++i){		InitBox(i);		color_f[i] = 0;	}	//ライトの位置セット	light_0_pos[0] = 1.0;	light_0_pos[1] = 10.0;	light_0_pos[2] = 5.0;	light_0_pos[3] = 1.0;	key_on = 0;	score = 0;	str_score[0] = '\0';	timer = GAME_TIME;	str_timer[0] = '\0';}void Stage1::InitBox(int i){	// boxの位置をランダムで配置	pos[i].x = ((float)rand() / (float)RAND_MAX) * BOARD_SIZE - ((int)BOARD_SIZE >> 1);	pos[i].y = 3.0;	pos[i].z = ((float)rand() / (float)RAND_MAX) * BOARD_SIZE - ((int)BOARD_SIZE >> 1);}void Stage1::Disp(){	glLoadIdentity();	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);	// camera	Vector3 campos = me.pos - me.vec * 20.0;	campos.y += 8.0;			// 高さをつける	gluLookAt(campos.x, campos.y, campos.z,			  me.pos.x, me.pos.y, me.pos.z, 0.0, 1.0, 0.0);	// move	me.force = 0.0;	if( key_on & KEY_W ){		me.force += me.vec * ME_MOVEMENT;	}	if( key_on & KEY_S ){		me.force -= me.vec * ME_MOVEMENT;	}	if( key_on & KEY_A ){		me.angle.y += BOX_CAMRA;		me.vec.x = sin(me.angle.y / 180 * M_PI);		me.vec.z = cos(me.angle.y / 180 * M_PI);	}	if( key_on & KEY_D ){		me.angle.y -= BOX_CAMRA;		me.vec.x = sin(me.angle.y / 180 * M_PI);		me.vec.z = cos(me.angle.y / 180 * M_PI);	}	if( key_on & KEY_SPACE && me.onface){ // jump		me.speed.y += JUMP_SPEED;	}		if(me.pos.y < -30.0){		me.pos = 0.0;		me.pos.y = 10.0;	}	// 当たり判定、物理処理	{		me.speed.y -= GRAVITY;	// 毎フレーム毎に高さを減らす				Vector3 speed = me.speed;		// 何をしているのかわからない		if( (me.force.x < 0 && speed.x > me.force.x) ||			(me.force.x > 0 && speed.x < me.force.x) ){			speed.x = me.force.x;		}		if( (me.force.y < 0 && speed.y > me.force.y) ||			(me.force.y > 0 && speed.y < me.force.y) ){			speed.y = me.force.y;		}		if( (me.force.z < 0 && speed.z > me.force.z) ||			(me.force.z > 0 && speed.z < me.force.z) ){			speed.z = me.force.z;		}		if( -(BOARD_SIZE >> 1) < me.pos.x + speed.x &&			me.pos.x + speed.x < BOARD_SIZE >> 1 &&			-(BOARD_SIZE >> 1) < me.pos.z + speed.z &&			me.pos.z + speed.z < BOARD_SIZE >> 1 &&	// 自機が床の範囲で			me.pos.y - 0.5 >= 0.0 &&			me.pos.y + speed.y - 0.5 < 0.0 ){			// 床を貫通する早さの時			speed.y = -me.pos.y + 0.5;			me.speed = 0.0;		// スピードを0にして床の上にとどまる			me.onface = 1;			// 床の上に居ることのフラグ(=1の時jump可能)		}else{			me.onface = 0;		}		me.pos += speed;	}	//床表示	board.render();	// 自分を表示	me.Render(rot);	// box	setDefaultMaterial();	GLfloat color[BOX_MAX][4];	for (int i = 0; i < BOX_MAX; ++i){		// 最初は全て赤で表示		color[i][0] = 1.0;		color[i][1] = 0.0;		color[i][2] = 0.0;		color[i][3] = 1.0;	}	for (int i = 0; i < BOX_MAX; ++i){		// color_fが1の時は自機が接触したことを表すので、アイテムboxを黒に変える		if(color_f[i] == 1 ){			color[i][0] = 0.0;		}	}	// 処理が多すぎる	for(int i = 0; i < BOX_MAX; ++i){		glMaterialfv(GL_FRONT, GL_DIFFUSE, color[i]);		glPushMatrix();		glTranslatef(pos[i].x, pos[i].y, pos[i].z);		glRotatef(rot, 0.0, 1.0, 0.0);		glutSolidCube(1.0);		glPopMatrix();		Vector3 hit = pos[i] - me.pos;		// hit.y += 0.5;		// 当たり判定は球		if(Hitp(pos[i], me.pos)){			me.pos.y = pos[i].y - 1.0; // 自機がboxを超えないようにする			color_f[i] = 1;			   // 次のフレームに色を変えるためのフラグ			if(pro[i].render_f == 0){  // 進捗がまだ出ていない(初めてこのboxに自機が触れた)なら				pro[i].render_f = 1;   // 進捗が存在するかどうかのフラグ(iはbox番号と対応しているので、ひとつのboxにつき、ひとつのflagを持つ)				pro[i].pos = pos[i];				pro[i].pos.y += 1.6;								pro[i].speed.y += JUMP_SPEED + 0.07;				pro[i].Direction = rand() % 4;				// std::cout << pro[i].Direction << std::endl;			}		}	}	// 進捗どうですか	for (int i = 0; i < BOX_MAX; ++i){		if(pro[i].render_f == 1){			pro[i].speed.y -= GRAVITY;			Vector3 pro_speed = pro[i].speed;			if( (pro[i].force.x < 0 && pro_speed.x > pro[i].force.x) ||				(pro[i].force.x > 0 && pro_speed.x < pro[i].force.x) ){				pro_speed.x = pro[i].force.x;			}						if( (pro[i].force.y < 0 && pro_speed.y > pro[i].force.y) ||				(pro[i].force.y > 0 && pro_speed.y < pro[i].force.y) ){				pro_speed.y = pro[i].force.y;			}						if( (pro[i].force.z < 0 && pro_speed.z > pro[i].force.z) ||				(pro[i].force.z > 0 && pro_speed.z < pro[i].force.z) ){				pro_speed.z = pro[i].force.z;			}			if(onFacep(BOARD_SIZE, pro_speed, pro[i].pos)){							pro_speed.y = -pro[i].pos.y + 0.5;				pro[i].speed = 0.0;				pro[i].onface = 1;			}else{				pro[i].onface = 0;			}			pro[i].pos += pro_speed;						switch(pro[i].Direction){			case 0:				pro[i].pos.x +=  ME_MOVEMENT / 3.0;				break;							case 1:				pro[i].pos.z +=  ME_MOVEMENT / 3.0;				break;							case 2:				pro[i].pos.x -=  ME_MOVEMENT / 3.0;				break;							case 3:				pro[i].pos.z -=  ME_MOVEMENT / 3.0;				break;			}			Vector3 hit_pro = pro[i].pos - me.pos;						// hit_pro.y += 0.5;						if(Hitp(pro[i].pos, me.pos)){								InitBox(i);								//得点加算				if(timer >= 0){					score++;				}								sprintf(str_score, "SCORE : %d", score );				pro[i].render_f = 0;				color_f[i] = 0;				pro[i].pos = 0;			}else if(pro[i].pos.y < -30.0){				InitBox(i);				color_f[i] = 0;				pro[i].render_f = 0;				pro[i].pos = 0;			}								pro[i].Render(rot);		}	}	rot++;						// boxの回転角度	if(timer-- < 0){		sprintf(str_timer, "TIME OVER");	}else{		sprintf(str_timer, "TIME : %d", timer);	}}void Stage1::Disp2D(int Width, int Height){	glColor4f(0.0, 0.0, 0.0, 1.0);	drawString(8, Height-30, fps(), GLUT_BITMAP_TIMES_ROMAN_24);	drawString(8, Height-50, str_score, GLUT_BITMAP_TIMES_ROMAN_24);	drawString(8, Height-70, str_timer, GLUT_BITMAP_TIMES_ROMAN_24);}void Stage1::Input(char event, int key, int x, int y){	switch(event){	case SC_INPUT_KEY_DOWN:		setKeyState(&key_on, key, true, keys, sizeof(keys));		break;	case SC_INPUT_KEY_UP:		setKeyState(&key_on, key, false, keys, sizeof(keys));		break;	}}bool Stage1::onFacep(int borad_size, Vector3 speed, Vector3 pro){	if( -(borad_size >> 1) < pro.x + speed.x &&		pro.x + speed.x < borad_size >> 1 &&		-(borad_size >> 1) < pro.z + speed.z &&		pro.z + speed.z < borad_size >> 1 &&		// 自機が床の範囲で		pro.y - 0.5 >= 0.0 &&		pro.y + speed.y - 0.5 < 0.0){		// 床を貫通する早さの時		return true;	}	return false;}bool Stage1::Hitp(Vector3 first, Vector3 second){	Vector3 hit_pro = first - second;		if(sqrt(hit_pro.x * hit_pro.x + hit_pro.y *			hit_pro.y + hit_pro.z * hit_pro.z) < 1.0){		return true;	}	return false;}Stage1::~Stage1(){	std::cout << "~Stage1()" << std::endl;}