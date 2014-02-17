// 参考URL:http://moto-net.github.io/programming/2012/11/12/OpenGLGame/

//Main.cpp

#include "Assist.hpp"
#include "Stage1.hpp"

#define FIRST_STAGECLASS Stage1()
#define TIMER_WAIT 20

//global
int WindowWidth = 400;
int WindowHeight = 400;
const char WindowTitle[] = "OpenGLGame";
StageClass *Stage;

void Init();
void Disp();
void Display();
void Reshape(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void Close();
void Timer(int);

/*----------------------------------------------------------------
  Entry point , initialization
  *--------------------------------------------------------------*/

//Entry point
int main(int argc, char *argv[])
{
	//GLUT initialization
	glutInit(&argc, argv);

	srand((unsigned int)time(NULL));
        
	glutInitWindowSize(WindowWidth, WindowHeight);
		
	//RGBA、z buffer、Double buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
		
	glutCreateWindow(WindowTitle);
        
	//Registration of the callback function
	glutDisplayFunc(Display);        //Redrawn when
	glutCloseFunc(Close);        //When the window is closed
	glutReshapeFunc(Reshape);        //when Window resizing
	glutKeyboardFunc(Keyboard);        //Key input
	glutKeyboardUpFunc(KeyboardUp);        //Key is released
	glutTimerFunc(TIMER_WAIT, Timer, 0);
	
	//initialization
	Init();

	//main loop
	glutMainLoop();        
}

//Initializing process
void Init(void)
{
	// Designation of Color to clear the back buffer
	glClearColor(0.5, 0.5, 1.0, 1.0);
        
	// Depth test ON
	glEnable(GL_DEPTH_TEST);
        
	// light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
        
	// Single-sided display
		
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	
	Stage = new Stage1();
	// seg

	// NextStage = NULL;
}

/*--------------------------------------------------------------------
  Instead of drawing screen
  *-----------------------------------------------------------------*/

void Disp()
{
	//　Clear back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	
	Stage->Disp();

	// 2Dへ以降
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, 0, WindowHeight);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	
	Stage->Disp2D(WindowWidth, WindowHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

// Instead of drawing screen
void Display(){
	Disp();

	glutSwapBuffers();

	// if( NextStage ) 
}

void Timer(int value)
{
	glutTimerFunc(TIMER_WAIT, Timer, 0);
	glutSwapBuffers();
	Disp();
}

// change window size
void Reshape(int x, int y)
{
	// Save window size
	WindowWidth = x;
	WindowHeight = y;
    
	// Viewport settings
	glViewport(0, 0, WindowWidth, WindowHeight);
	
    
	//　Setting the projection matrix
	//Manipulate PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)WindowWidth/(double)WindowHeight,
				   0.1, 1000.0);        // setting
}

/*-----------------------------------------------------------------------------------*
  Input relationship
  *-----------------------------------------------------------------------------------*/

// Keyboard handling
void Keyboard(unsigned char key, int x, int y)
{
	Stage->Input(SC_INPUT_KEY_DOWN, (int)key, x, y);
	
	switch ( key ){
	case '\033':        //Esc
		Close();
		exit(0);
		break;
	}
}

//Key is released
void KeyboardUp(unsigned char key, int x, int y)
{
	Stage->Input(SC_INPUT_KEY_UP, (int)key, x, y);
}

/*-----------------------------------------------------------------------------------*
  other
  *-----------------------------------------------------------------------------------*/

// End processing
void Close(void)
{
	std::cout << "finish!" << std::endl;

	delete Stage;
}
