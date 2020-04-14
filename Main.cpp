
//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

#include <windows.h>		// Header file for Windows
#include <iostream>
#include <chrono>
#include <vector>
#include <array>
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library

#include "Actor.h"
#include "Character.h"
#include "Background.h"
#include "Ground.h"
#include "Platform.h"
#include "HmovePlatform.h"
#include "VmovePlatform.h"
#include "DoorPlatform.h"
#include "RotatingPlatform.h"
#include "RotatingDoor.h"
#include "Collectable.h"
#include "Enemy.h"
#include "InfoPanel.h"
#include "TargettingEnemy.h"
#include "FinishPortal.h"

int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
bool SpaceKeyPressed = false;
int screenWidth=1280, screenHeight=720;
bool keys[256];
float spin=0;
float speed=0;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
GLuint loadPNG(char* name);
void init_objects();
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void spaceKey();
void update();				//called in winmain to update variables

							// This stores a handle to the texture
GLuint base;
GLvoid BuildFont(GLvoid);
GLvoid KillFont(GLvoid);
GLvoid glPrint(const char *fmt, ...);

GLuint myTexture = 0, skyTexture = 0, alphaTexture = 0, alphaTexture2 = 0, currentTex = 0;
GLfloat skyTransX = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> currentTime, previousTime;
float deltaTime;

float Xmove = 0;
bool jump;

Background bg(15000,2000,-1300,0);
//Character mychar2;
std::shared_ptr<Character> mychar = std::shared_ptr<Character>(new Character());
std::vector<std::shared_ptr<Actor>> allActors;

float xmin, xmax, ymin, ymax;				// coordinates of the "camera"

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	currentTime = std::chrono::high_resolution_clock::now();
	deltaTime = (float)((currentTime - previousTime).count())*1000 * std::chrono::high_resolution_clock::period::num /
		std::chrono::high_resolution_clock::period::den; // milliseconds
	previousTime = currentTime;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InfoPanel::setCamPos(xmin, ymin, ymax - ymin, xmax - xmin);
	bg.display(deltaTime);

	if (!InfoPanel::paused) mychar->checkAllCollision(allActors);

	int to_delete_index = -1;
	for (int i = 0; i < allActors.size(); i++) {
		if (std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(allActors[i])) {
			if (enemy->toDelete()) to_delete_index = i;
		}
		if(!InfoPanel::paused) allActors[i]->update(deltaTime);
		if(allActors[i]->isOnScreen())
			allActors[i]->display(deltaTime);
	}
	if(to_delete_index>=0) allActors.erase(allActors.begin() + to_delete_index);

	mychar->ymove(jump, deltaTime);
	mychar->display(deltaTime);

	InfoPanel::display();
	
	//PRINTING SCORE
	glLoadIdentity();                   // Reset The View
	glTranslatef(0.0f, 0.0f, -1.0f);              // Move One Unit Into The Screen
												  // Pulsing Colors Based On Text Position
	glColor3f(0.15,0.15,0.15);
	// Position The Text On The Screen
	glRasterPos2f(InfoPanel::getCam_xpos() + InfoPanel::getCam_width()-260,
				  InfoPanel::getCam_ypos() + InfoPanel::getCam_height() - 55);
	glPrint("SCORE : %i", InfoPanel::getScore());  // Print GL Text To The Screen

	if (InfoPanel::isGameFinished() && InfoPanel::paused) {
		//PRINTING SCORE for end screen
		glLoadIdentity();                   // Reset The View
		glTranslatef(0.0f, 0.0f, -1.0f);              // Move One Unit Into The Screen
													  // Pulsing Colors Based On Text Position
		glColor3f(0.24, 0.24, 0.24);
		// Position The Text On The Screen
		glRasterPos2f(InfoPanel::getCam_xpos() + InfoPanel::getCam_width() / 2 - 140,
			InfoPanel::getCam_ypos() + InfoPanel::getCam_height() / 2 - 100);
		glPrint("Your score : %i", InfoPanel::getScore());  // Print GL Text To The Screen
	}

	glFlush();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
	float WperH = (float)width / (float)height;						// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset the current viewport

	glMatrixMode(GL_PROJECTION);						// select the projection matrix stack
	glLoadIdentity();									// reset the top of the projection matrix to an identity matrix

	xmin = mychar->getX() - 300;
	if (xmin < bg.getX()) xmin = bg.getX();
	xmax = xmin + width;
	if (xmax > bg.getW() + bg.getX()) { xmax = bg.getW() + bg.getX(); xmin = xmax - width; }
	
	ymin = mychar->getY() - 400;
	if (ymin < bg.getY()) ymin = bg.getY();
	ymax = ymin + height;
	if (ymax > bg.getH() + bg.getY()) { ymax = bg.getH() + bg.getY(); ymin = ymax - height; }

	gluOrtho2D(xmin, xmax,ymin, ymax);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select the modelview matrix stack
	glLoadIdentity();									// Reset the top of the modelview matrix to an identity matrix
}
void init()
{
	previousTime = std::chrono::high_resolution_clock::now();

	glClearColor(0.0, 0.0, 0.0, 0.0);						//sets the clear colour to black
															//glClear(GL_COLOR_BUFFER_BIT) in the display function
															//will clear the buffer to this colour.
	BuildFont();                        // Build The Font


	init_objects();
	mychar->setPos(200, 400);
	
	/**********************************************************************************************/

	/**********************************************************************************************/
}

void init_objects() {
	// Initilizing all ground objects
	int const GROUND_OBJ_COUNT = 46;
	float groundObjs[GROUND_OBJ_COUNT][5] = { { 13,6,-1300,0,99 },{ 14,1,-1300,300,4 },{ 1,6,-650,0,21 },{ 12,1,-1250,350,3 },
	{ 10,1,-1200,400,3 },{ 8,1,-1150,450,3 },{ 6,1,-1100,500,3 },{ 4,1,-1050,550,3 },{ 2,1,-1000,600,1 },{ 51,1,-400,0,1 },
	{ 5,1,-400,300,2 },{ 18,1,-750,800,2 },{ 1,23,1300,0,22 },{ 1,23,1350,0,23 },{ 6,1,2200,300,2 },{ 6,1,2550,550,2 },
	{ 20,1,2950,0,5 },{ 5,1,3950,0,7 },{ 5,1,3950,50,6 },{ 4,1,4000,100,6 },{ 3,1,4050,150,6 },{ 2,1,4100,200,1 },
	{ 15,1,3750,900,2 },{ 6,1,4450,600,2 },{ 1,7,4450,600,25 },{ 1,20,4700,600,24 },{ 5,1,5150,0,9 },{ 5,1,5150,50,8 },
	{ 4,1,5150,100,8 },{ 3,1,5150,150,8 },{ 2,1,5150,200,1 },{ 28,1,5400,0,10 },{ 12,1,7300,900,2 },{ 8,1,8150,900,11 },
	{ 1,1,8600,900,12 },{ 10,1,8350,700,12 },{ 6,1,7900,700,11 },{ 14,1,8150,400,2 },{ 1,5,8800,700,26 },
	{ 1,6,8550,1100,27 },{ 6,1,10000,700,11 },{ 6,1,10700,700,12 },{ 6,1,10350,950,2 },{ 1,15,10950,700,24 }, { 20,1,10250,0,1 },{14,1,13100,900,2} };

	for (int i = 0; i < GROUND_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> ground = std::shared_ptr<Actor>(new Ground(groundObjs[i][0], groundObjs[i][1], groundObjs[i][2], groundObjs[i][3], groundObjs[i][4]));
		allActors.push_back(ground);
	}

	// Initializing all static platforms
	int const PLATF_OBJ_COUNT = 15;
	float platfObjs[PLATF_OBJ_COUNT][3] = { { 5,700,300 },{ 5,1000,550 },{ 5,700,800 },{ 5,1000,1050 },{ 5,3750,500 },
		{ 5,3450,700 },{ 3,6150,250 },{ 3,6150,650 },{ 3,6400,450 },{ 3,5750,650 },{ 3,8200,700 },{ 3,8650,900 },
		{ 5,11300,150 },{ 5,12300,350 },{ 5,12850,750 }, };

	for (int i = 0; i < PLATF_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> platform = std::shared_ptr<Actor>(new Platform(platfObjs[i][0], platfObjs[i][1], platfObjs[i][2]));
		allActors.push_back(platform);
	}

	// Initializing all horizontal moving platforms
	int const HPLATF_OBJ_COUNT = 3;
	float hPlatfObjs[HPLATF_OBJ_COUNT][4] = { { 5,4250,300,600 },{ 5,4950,800,500 },{ 5,11600,150,450 } };

	for (int i = 0; i < HPLATF_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> platform = std::shared_ptr<Actor>(new HmovePlatform(hPlatfObjs[i][0], hPlatfObjs[i][1], hPlatfObjs[i][2], hPlatfObjs[i][3]));
		allActors.push_back(platform);
	}

	// Initializing all vertical moving platforms
	int const VPLATF_OBJ_COUNT = 3;
	float vPlatfObjs[VPLATF_OBJ_COUNT][4] = { { 8,6850,100,800 },{ 4,8950,400,500 },{ 5,12600,350,400 } };

	for (int i = 0; i < VPLATF_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> platform = std::shared_ptr<Actor>(new VmovePlatform(vPlatfObjs[i][0], vPlatfObjs[i][1], vPlatfObjs[i][2], vPlatfObjs[i][3]));
		allActors.push_back(platform);
	}

	// Initializing all door platforms
	int const DPLATF_OBJ_COUNT = 1;
	float dPlatfObjs[DPLATF_OBJ_COUNT][3] = { { 4,8550,750 } };

	for (int i = 0; i < DPLATF_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> door = std::shared_ptr<Actor>(new DoorPlatform(dPlatfObjs[i][0], dPlatfObjs[i][1], dPlatfObjs[i][2]));
		allActors.push_back(door);
	}

	// Initializing all rotating platforms
	int const RPLATF_OBJ_COUNT = 1;
	float rPlatfObjs[RPLATF_OBJ_COUNT][3] = { { 13,9200,900 } };

	for (int i = 0; i < RPLATF_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> rotPlat = std::shared_ptr<Actor>(new RotatingPlatform(rPlatfObjs[i][0], rPlatfObjs[i][1], rPlatfObjs[i][2]));
		allActors.push_back(rotPlat);
	}

	// Initializing all rotating doors
	int const RDOOR_OBJ_COUNT = 2;
	float rDoorObjs[RDOOR_OBJ_COUNT][4] = { { 4,10300,700,1 },{ 4,10500,700,0 } };

	for (int i = 0; i < RDOOR_OBJ_COUNT; i++) {
		bool leftDoor = true;
		if (rDoorObjs[i][3] == 0) leftDoor = false;
		std::shared_ptr<Actor> rotPlat = std::shared_ptr<Actor>(new RotatingDoor(rDoorObjs[i][0], rDoorObjs[i][1], rDoorObjs[i][2], leftDoor));
		allActors.push_back(rotPlat);
	}

	//Initializing finish portal
	std::shared_ptr<Actor> portal = std::shared_ptr<Actor>(new FinishPortal(13250, 950));
	//std::shared_ptr<Actor> portal = std::shared_ptr<Actor>(new FinishPortal(400, 500));
	allActors.push_back(portal);

	// Initializing enemies
	int const ENE_OBJ_COUNT = 5;
	float eneObjs[ENE_OBJ_COUNT][2] = { { 500,50 },{ 3250,50 },{ 5550,50 },{ 7350,950 },{ 10700,50 } };

	for (int i = 0; i < ENE_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> enemy = std::shared_ptr<Actor>(new Enemy(eneObjs[i][0], eneObjs[i][1]));
		allActors.push_back(enemy);
	}

	// Initializing targetting enemies
	int const TENE_OBJ_COUNT = 3;
	float teneObjs[TENE_OBJ_COUNT][2] = { { 1950,450 },{ 4500,1200 },{ 10000,1100 } };

	for (int i = 0; i < TENE_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> enemy = std::shared_ptr<Actor>(new TargettingEnemy(teneObjs[i][0], teneObjs[i][1], mychar));
		allActors.push_back(enemy);
	}

	// Initializing all collectable items
	int const COLL_OBJ_COUNT = 7;
	float collObjs[COLL_OBJ_COUNT][3] = { { 50,1100,0 },{ 4600,750,1 },{ 5075,950,2 },{ 8700,1200,3 },{ 8700,825,4 },
	{ 10500,1050,5 },{ 12975,1000,6 } };

	std::shared_ptr<Actor> following = mychar;
	for (int i = 0; i < COLL_OBJ_COUNT; i++) {
		std::shared_ptr<Actor> collectable = std::shared_ptr<Actor>(new Collectable(collObjs[i][0], collObjs[i][1], collObjs[i][2], following));
		allActors.push_back(collectable);
		following = collectable;
	}

	
}

void processKeys()
{
	if (keys[VK_LEFT] && !InfoPanel::paused)
	{
		mychar->xmove(true, false, deltaTime);
		mychar->setIsWalking(true);
	}
	if (keys[VK_RIGHT] && !InfoPanel::paused)
	{
		mychar->xmove(false, true, deltaTime);
		mychar->setIsWalking(true);
	}
	if (!keys[VK_RIGHT] && !keys[VK_LEFT]) { mychar->setIsWalking(false); }

	if(keys[VK_UP] && !InfoPanel::paused)
	{
		jump = true;
	}
	else
	{
		jump = false;
	}
	if (keys[VK_SPACE])
	{
		SpaceKeyPressed = true;
	}
	else if (SpaceKeyPressed) {
		spaceKey();
		SpaceKeyPressed = false;
	}

}

void spaceKey() {
	if (!InfoPanel::hasGameStarted()) {
		InfoPanel::start();
	}
	else if (InfoPanel::isGameOver() || (InfoPanel::isGameFinished() && InfoPanel::paused)) {
		allActors.clear();
		init_objects();
		mychar->reset();
		InfoPanel::reset();
	}
	else if (InfoPanel::paused) {
		InfoPanel::paused = false;
	}
	else InfoPanel::paused = true;

}

void update()
{
	reshape(screenWidth, screenHeight);
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop


	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	
	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard
			
			display();					// Draw The Scene
			update();					// update variables
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}

	KillFont();                     // Destroy The Font
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}

/* http://nehe.gamedev.net/tutorial/bitmap_fonts/17002/ */

GLvoid BuildFont(GLvoid)                    // Build Our Bitmap Font
{
	HFONT   font;                       // Windows Font ID
	HFONT   oldfont;                    // Used For Good House Keeping

	base = glGenLists(96);                  // Storage For 96 Characters ( NEW )
	font = CreateFont(-30,                // Height Of Font ( NEW )
		0,              // Width Of Font
		0,              // Angle Of Escapement
		0,              // Orientation Angle
		FW_BOLD,            // Font Weight
		FALSE,              // Italic
		FALSE,              // Underline
		FALSE,              // Strikeout
		ANSI_CHARSET,           // Character Set Identifier
		OUT_TT_PRECIS,          // Output Precision
		CLIP_DEFAULT_PRECIS,        // Clipping Precision
		ANTIALIASED_QUALITY,        // Output Quality
		FF_DONTCARE | DEFAULT_PITCH,  // Family And Pitch
		"Courier New");         // Font Name

	oldfont = (HFONT)SelectObject(hDC, font);       // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);           // Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);             // Selects The Font We Want
	DeleteObject(font);                 // Delete The Font
}

GLvoid KillFont(GLvoid)                     // Delete The Font List
{
	glDeleteLists(base, 96);                // Delete All 96 Characters ( NEW )
}

GLvoid glPrint(const char *fmt, ...)                // Custom GL "Print" Routine
{
	char        text[256];              // Holds Our String
	va_list     ap;                 // Pointer To List Of Arguments
	if (fmt == NULL)                    // If There's No Text
		return;                     // Do Nothing
	va_start(ap, fmt);                  // Parses The String For Variables
		vsprintf(text, fmt, ap);                // And Converts Symbols To Actual Numbers
	va_end(ap);                     // Results Are Stored In Text
	glPushAttrib(GL_LIST_BIT);              // Pushes The Display List Bits     ( NEW )
	glListBase(base - 32);                  // Sets The Base Character to 32    ( NEW )
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text  ( NEW )
	glPopAttrib();                      // Pops The Display List Bits   ( NEW )
}
