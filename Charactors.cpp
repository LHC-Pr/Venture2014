// Charactors.cpp
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
	glTranslated(pos.x, pos.y+0.5, pos.z);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5, 1.0, 10, 10);
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

void Me::PhyCal(int rot)
{
	if(pos.y < -30.0){
		pos = 0.0;
		pos.y = 10.0;
	}
	
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
	
	// 自分を表示
	Render(rot);
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
	glutSolidTorus(0.25, 0.25, 10, 10);
	glPopMatrix();
}

void Progress::PhyCal(int rot)
{
	if(renderp()){

		speed.y -= GRAVITY;
		Vector3 pro_speed = speed;
		
		if( (force.x < 0 && pro_speed.x > force.x) ||
			(force.x > 0 && pro_speed.x < force.x) ){
			pro_speed.x = force.x;
		}

		if( (force.y < 0 && pro_speed.y > force.y) ||
			(force.y > 0 && pro_speed.y < force.y) ){
			pro_speed.y = force.y;
		}

		if( (force.z < 0 && pro_speed.z > force.z) ||
			(force.z > 0 && pro_speed.z < force.z) ){
			pro_speed.z = force.z;
		}

		if(onFacep(BOARD_SIZE, pro_speed, pos)){
			pro_speed.y = -pos.y + 0.5;
			speed = 0.0;
			onface = 1;
		}else{
			onface = 0;
		}

		pos += pro_speed;

		switch(Direction){
		case 0:
			pos.x +=  ME_MOVEMENT / 3.0;
			break;
		case 1:
			pos.z +=  ME_MOVEMENT / 3.0;
			break;
		case 2:
			pos.x -=  ME_MOVEMENT / 3.0;
			break;
		case 3:
			pos.z -=  ME_MOVEMENT / 3.0;
			break;
		}

		Render(rot);
	}
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
	glutSolidSphere(0.5, 10, 10);
	glPopMatrix();	
}

void Enemy::PhyCal(int rot)
{
	speed.y -= GRAVITY;
	Vector3 ene_speed = speed;
	
	if( (force.x < 0 && ene_speed.x > force.x) ||
		(force.x > 0 && ene_speed.x < force.x) ){
		ene_speed.x = force.x;
	}		
	
	if( (force.y < 0 && ene_speed.y > force.y) ||
		(force.y > 0 && ene_speed.y < force.y) ){
		ene_speed.y = force.y;
	}
		
	if( (force.z < 0 && ene_speed.z > force.z) ||
		(force.z > 0 && ene_speed.z < force.z) ){
		ene_speed.z = force.z;
	}
		
	if(onFacep(BOARD_SIZE, ene_speed, pos)){			
		ene_speed.y = -pos.y + 0.5;
		speed = 0.0;
		onface = 1;
	}else{
		onface = 0;
	}

	// pos += ene_speed;
		
	switch(Direction){
	case 0:
		pos.z += ME_MOVEMENT / 2.0;
		break;
	case 1:
		pos.x += ME_MOVEMENT;
		break;
	case 2:
		pos.z -= ME_MOVEMENT;
		pos.x += ME_MOVEMENT;
		break;
	case 3:
		pos.z += ME_MOVEMENT / 2.0;
		pos.x -= ME_MOVEMENT / 2.0;
		break;
	}
	
	if(pos.y < -15){
		pos.x = (rand() % BOARD_SIZE) - (BOARD_SIZE >> 1) ;
		pos.z = (rand() % BOARD_SIZE) - (BOARD_SIZE >> 1);
		pos.y = 10.0;
		Direction = rand() % 4; // make 0~3		
	}
		
	pos += ene_speed;
		
	Render(rot);
}


