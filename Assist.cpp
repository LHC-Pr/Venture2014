// Assist.cpp
#include "Assist.hpp"

char* fps(void)
{
	static int count = 0, t1, t2 = 0;
	// count: 前回からのフレーム数 t1:現在時間 t2: 前回測定時の時間
	static char txt_fps[20] = "";

	t1 = glutGet(GLUT_ELAPSED_TIME);

	if (t1 - t2 > 10000){
		sprintf(txt_fps, "FPS: %g", (1000.0 * count) / (t1 - t2));

		t2 = t1;
		count = 0;
	}
	
	++count;

	return txt_fps;
}

void setDefaultMaterial(){

	GLfloat defoma_a[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat defoma_d[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat defoma_i[4] = { 0.0, 0.0, 0.0, 1.0 };
	
	// Key color
	glColor4f(1.0, 1.0, 1.0, 1.0);

	// Ambient Color
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defoma_a);

	// Diffuse Color
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defoma_d);

	// Specular Color
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defoma_i);

	// Emissive Color
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defoma_i);
	
	// Shininess
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	
}

void drawString(int x, int y, char *string, void *font){
	int len;

	glRasterPos2f(x, y);
	len = (int)strlen(string);

	for (int i = 0; i < len; ++i){
		glutBitmapCharacter(font, string[i]);
	}
}

Board::Board(float u, float v)
{	
	pos = 0.0;
	size.x = 0.0;
	size.y = 0.0;
	
	setRender(u, v);
}

void Board::setRender(float u, float v)
{
	const static GLfloat ground[][4] = {
		{ 0.1, 0.1, 0.1, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 }
	};
	display_list = glGenLists(1);

	glNewList(display_list, GL_COMPILE);
	
	setDefaultMaterial();
	
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	
	for (int j = -6; j < 6; ++j){
		for (int i = -6; i < 6; ++i){
			
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
			glVertex3f((GLfloat)i * (u/12.0), 0.0,
					   (GLfloat)j * (v/12.0));
			glVertex3f((GLfloat)i * (u/12.0), 0.0,
					   (GLfloat)(j + 1) * (v/12.0));
			glVertex3f((GLfloat)(i + 1) * (u/12.0), 0.0,
					   (GLfloat)(j + 1) * (v/12.0));
			glVertex3f((GLfloat)(i + 1) * (u/12.0), 0.0,
					   (GLfloat)j * (v/12.0));
		}
	}
	glEnd();

	
	glEndList();
	
	size.x = u;
	size.y = v;

	// 床の四角形の頂点位置の保存
	vertex[0].x = -u/2;
	vertex[0].y = 0.0;
	vertex[0].z = -v/2;
	
	vertex[1].x =  u/2;
	vertex[1].y = 0.0;
	vertex[1].z = -v/2;
	
	vertex[2].x =  u/2;
	vertex[2].y = 0.0;
	vertex[2].z =  v/2;
	
	vertex[3].x = -u/2;
	vertex[3].y = 0.0;
	vertex[3].z =  v/2;		
}

void Board::render()
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glCallList(display_list);
	glPopMatrix();
}

/*-----------------------------------------------------------------------------------*
	キー入力補助
	KeyBufのKeysと対応するビットを変更する。
	例：
	Keys = { a, b, c, d }　とする。
	KeyBufの中身は、２進数(実際のメモリ配置)で表すと
	00100000 00000000 00000000 00000000 (32ビット)
	abcd
	１ビットづつ順番にa,b,c,dと割り当て、そのビットが1か0かでキーの状態を保存する。
	KeyBufが上記の時は、cは押されていて、他のキーは離されている。
 *-----------------------------------------------------------------------------------*/

bool setKeyState(int *KeyBuf, int key, bool onoff, int *Keys, int size)
{
	size /= sizeof(int);

	// if too big
	if(size >= sizeof(int) * 8){
		return false;
	}

	for(int i = 0; i < size; ++i){
		if (key == Keys[i]){
			int k = 1;
			if(onoff){
				*KeyBuf |= k << i;				
			}else{
				*KeyBuf &= ~(k << i);
			}
		}
	}

	return true;
}

bool getKeyState(int *KeyBuf, int key, int *Keys, int size)
{
	if( size >= sizeof(int) * 8){
		return false;
	}

	//
	for(int i = 0; i < size; ++i){
		if(key == Keys[i]){
			int k = 1;
			return *KeyBuf & (k << i);			
		}
	}

	return true;
}


bool Hitp(Vector3 first, Vector3 second){
	Vector3 hit_pro = first - second;
	
	if(sqrt(hit_pro.x * hit_pro.x + hit_pro.y *
			hit_pro.y + hit_pro.z * hit_pro.z) < 1.0){
		return true;
	}
	return false;
}

bool onFacep(int borad_size, Vector3 speed, Vector3 pos){
	if( -(borad_size >> 1) < pos.x + speed.x &&
		pos.x + speed.x < borad_size >> 1 &&
		-(borad_size >> 1) < pos.z + speed.z &&
		pos.z + speed.z < borad_size >> 1 &&
		// 自機が床の範囲で
		pos.y - 0.5 >= 0.0 &&
		pos.y + speed.y - 0.5 < 0.0){
		// 床を貫通する早さの時
		return true;
	}
	return false;
}

bool InBoardSizep(int board_size, Vector3 pos){
	if(pos.z < (board_size >> 1) && pos.z > (-board_size >> 1) && 
	   pos.x < (board_size >> 1) && pos.x > (-board_size >> 1)){
		return true;		
	}
	
	return false;	
}
