#include "Charactors.hpp"

Charactor::Charactor(){
	pos = 0.0;
	speed = 0.0;
	force = 0.0;
}

Me::Me()
{
	angle = 0.0;
	vec = 0.0;
}

void Me::Render(int rot){
	// box color
	setDefaultMaterial();
	GLfloat color[4] = { 1.0, 1.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);
	glRotatef(rot, 0.0, 1.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

void Me::MoveCal(int key_on)
{
	force = 0.0;

	if( key_on & KEY_W ){
		force += vec * ME_MOVEMENT;
	}

	if( key_on & KEY_S ){
		force -= vec * ME_MOVEMENT;
	}

	if( key_on & KEY_A ){
		angle.y += BOX_CAMRA;
		vec.x = sin(angle.y / 180 * M_PI);
		vec.z = cos(angle.y / 180 * M_PI);
	}
	if( key_on & KEY_D ){
		angle.y -= BOX_CAMRA;
		vec.x = sin(angle.y / 180 * M_PI);
		vec.z = cos(angle.y / 180 * M_PI);
	}

	if( key_on & KEY_SPACE && onface){ // jump
		speed.y += JUMP_SPEED;
	}
}

void Me::PhyCal()
{		
	speed.y -= GRAVITY;	// 毎フレーム毎に高さを減らす
		
	Vector3 calspeed;
	calspeed = speed;

	// 何をしているのかわからない
	if( (force.x < 0 && calspeed.x > force.x) ||
		(force.x > 0 && calspeed.x < force.x) ){
		calspeed.x = force.x;
	}
	if( (force.y < 0 && calspeed.y > force.y) ||
		(force.y > 0 && calspeed.y < force.y) ){
		calspeed.y = force.y;
	}
	if( (force.z < 0 && calspeed.z > force.z) ||
		(force.z > 0 && calspeed.z < force.z) ){
		calspeed.z = force.z;
	}
	
	if( -(BOARD_SIZE >> 1) < pos.x + calspeed.x &&
		pos.x + calspeed.x < BOARD_SIZE >> 1 &&
		-(BOARD_SIZE >> 1) < pos.z + calspeed.z &&
		pos.z + calspeed.z < BOARD_SIZE >> 1 &&	// 自機が床の範囲で
		pos.y - 0.5 >= 0.0 &&
		pos.y + calspeed.y - 0.5 < 0.0 ){
		// 床を貫通する早さの時
		calspeed.y = -pos.y + 0.5;
		calspeed = 0.0;		// スピードを0にして床の上にとどまる
		onface = 1;			// 床の上に居ることのフラグ(=1の時jump可能)
	}else{
		onface = 0;
	}
	
	pos += calspeed;
}

Progress::Progress()
{
	pos = 0.0;
	speed = 0.0;
	force = 0.0;
	render_f = false;
}

void Progress::Render(int rot){
	
	// box color
	setDefaultMaterial();
	GLfloat color[4] = { 0.0, 1.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);
	glRotatef(rot, 0.0, 1.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

void Progress::Reset(Vector3 box_pos)
{
	render_f = true;   // 進捗が存在するかどうかのフラグ(iはbox番号と対応しているので、ひとつのboxにつき、ひとつのflagを持つ)
	pos = 0.0;
	speed = 0.0;
	force = 0.0;
	
	pos = box_pos;
	pos.y += 1.0;
	onface = 0;
	speed.y += JUMP_SPEED + 0.10;
	Direction = rand() % 4;
	// std::cout << pro[i].Direction << std::endl;
}

void Progress::erase()
{
	render_f = false;
	pos = -2.0;
}

bool Progress::renderp()
{
	return render_f;
}

Enemy::Enemy()
{
	Direction = 0;
}

void Enemy::Render(int rot)
{
	
	setDefaultMaterial();
	GLfloat color[4] = { 0.0, 0.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);
	glRotatef(rot, 0.0, 1.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();	
}

void Enemy::PhyCal()
{

}


