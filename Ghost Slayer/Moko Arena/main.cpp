#pragma once /*complier stuff*/

#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif /*32 BIT setup*/

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib") /*pragma means compiler stuff*/

//Header Files
#include <Game.h>
#include <Timer.h>

//C++ Libraries
#include <stdlib.h>
#include <iostream>
#include <windows.h>            // Header File For Windows
#include <windowsx.h>

HDC			hDC  = NULL;		// Private GDI Device Context
HGLRC		hRC  = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		    // Holds The Instance Of The Application

// Declaration For WndProc
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//GLfloat fullscreenWidth  = 500.0f;
//GLfloat fullscreenHeight = 400.0f;
GLfloat fullscreenWidth  = GetSystemMetrics(SM_CXSCREEN);
GLfloat fullscreenHeight = GetSystemMetrics(SM_CYSCREEN);
Game* game_ = new Game(fullscreenWidth, fullscreenHeight);
Timer* timer = new Timer();


float FPS;
BOOL done = FALSE;
int time_now, time_prev;
bool active     = TRUE;         // Window Active Flag Set To TRUE By Default
bool fullscreen = TRUE;	        // FullScreen Flag Set To FullScreen Mode By Default

//
//          THE KILL GL WINDOW
//
GLvoid KillGLWindow()					        // Properly Kill The Window
{
	if (fullscreen) {                           // Are We In Fullscreen Mode?
		ChangeDisplaySettings(NULL,0);			// If So Switch Back To The Desktop
		ShowCursor(TRUE);						// Show Mouse Pointer
	}

	if (hRC) {									// Do We Have A Rendering Context?
		if (!wglMakeCurrent(NULL,NULL))			// Are We Able To Release The DC And RC Contexts?
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hRC))				// Are We Able To Delete The RC?
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hRC = NULL;								// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC)) {			// Are We Able To Release The DC
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC = NULL;								// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd)) {			// Are We Able To Destroy The Window?
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;								// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance)) {	// Are We Able To Unregister Class
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;							// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CREATE GL WINDOW
/////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left   = (long)0;		// Set Left Value To 0
	WindowRect.right  = (long)width;	// Set Right Value To Requested Width
	WindowRect.top    = (long)0;		// Set Top Value To 0
	WindowRect.bottom = (long)height;	// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;		// Set The Global Fullscreen Flag

	hInstance		 = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style	     = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc	 = (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra	 = 0;									// No Extra Window Data
	wc.cbWndExtra	 = 0;									// No Extra Window Data
	wc.hInstance	 = hInstance;							// Set The Instance
	wc.hIcon		 = LoadIcon(NULL, IDI_ERROR);			// Load The Default Icon
	wc.hCursor		 = LoadCursor(NULL, IDC_HELP);			// Load The Arrow Pointer
	wc.hbrBackground = CreateSolidBrush(RGB(0.0f, 0.0f, 0.0f));
	//CreatePatternBrush((HBITMAP)LoadImage(0, ("~images\Tiles\Testing.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE));
	//									// No Background Required For GL
	wc.lpszMenuName	 = NULL;								// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";							// Set The Class Name

	if (!RegisterClass(&wc)) {  							// Attempt To Register The Window Class
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (fullscreen) {							                // Attempt Fullscreen Mode?
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		dwExStyle = WS_EX_APPWINDOW;							// Window Extended Style
		dwStyle   = WS_POPUP;			                        // must handle Gsync situations: Windows Style
		//ShowCursor(FALSE);									    // Hide Mouse Pointer
	}
	else {
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(dwExStyle,						// Extended Style For The Window
                              "OpenGL",							// Class Name
                              title,							// Window Title
                              dwStyle |							// Defined Window Style
                              WS_CLIPSIBLINGS |					// Required Window Style
                              WS_CLIPCHILDREN,					// Required Window Style
                              0,
                              0,								// Window Position
                              WindowRect.right-WindowRect.left,	// Calculate Window Width
                              WindowRect.bottom-WindowRect.top,	// Calculate Window Height
                              NULL,								// No Parent Window
                              NULL,								// No Menu
                              hInstance,						// Instance
                              NULL)))							// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// Variable pfd tells windows how we want things to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size of this pixel format descriptor
		1,											// Version number
		PFD_DRAW_TO_WINDOW |						// Format must support window
		PFD_SUPPORT_OPENGL |						// Format must support OpenGL
		PFD_DOUBLEBUFFER,							// Must support double buffering
		PFD_TYPE_RGBA,								// Request an RGBA format
		bits,										// Select our color depth
		0, 0, 0, 0, 0, 0,							// Color bits ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd))) {                               // Did We Get A Device Context?
		KillGLWindow();								        // Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))) {       // Did Windows Find A Matching Pixel Format?
		KillGLWindow();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(hDC,PixelFormat,&pfd)) {            // Are We Able To Set The Pixel Format?
		KillGLWindow();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC)))	{                   // Are We Able To Get A Rendering Context?
		KillGLWindow();
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC,hRC)) {                         // Try To Activate The Rendering Context
		KillGLWindow();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWnd,SW_SHOW);						        // Show The Window
	SetForegroundWindow(hWnd);						        // Slightly Higher Priority
	SetFocus(hWnd);									        // Sets Keyboard Focus To The Window
	game_->resizeGame(width, height);			            // Set Up Our Perspective GL Screen

	if (!game_->initializeGame()) {                         // Initialize Our Newly Created GL Window
		KillGLWindow();
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
    FPS = 60.0;
    time_prev = 0;
	return TRUE;
}

//
//							THE WINDOW PROCEDURE
//
LRESULT CALLBACK WndProc(HWND	hWnd,		        // Handle For This Window
                         UINT	uMsg,		        // Message For This Window
                         WPARAM	wParam,	            // Additional Message Information
                         LPARAM	lParam)             // Additional Message Information
{
	switch (uMsg) {                                 // Check For Windows Messages
		case WM_ACTIVATE: {                         // Watch For Window Activate Message
			if (!HIWORD(wParam))					// Check Minimization State
				active = TRUE;						// Program Is Active
			else
				active = FALSE;			            // Program Is No Longer Active
			return 0;					            // Return To The Message Loop
		}

		case WM_SYSCOMMAND: {                       // Intercept System Commands
			switch (wParam)				            // Check System Calls
			{
				case SC_SCREENSAVE:		            // Screensaver Trying To Start?
				case SC_MONITORPOWER:	            // Monitor Trying To Enter Powersave?
				return 0;				            // Prevent From Happening
			}                                       // Exit
		} break;

		case WM_GETMINMAXINFO: {
            MINMAXINFO* win_ = (MINMAXINFO*)lParam;
            win_->ptMinTrackSize.x = 500.0f;
            win_->ptMinTrackSize.y = 300.0f;
            return 0;
		}

		case WM_MOUSEMOVE: {
            game_->updatingMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
		}

		case WM_CLOSE: {                            // Did We Receive A Close Message?
            PostQuitMessage(0);			            // Send A Quit Message
			return 0;
		}

		case WM_LBUTTONDBLCLK: {
            game_->mousePressed();
            return 0;
		}

		case WM_LBUTTONDOWN:
		case WM_KEYDOWN: {                          // Is A Key Being Held Down?
		    game_->buttonPressed(wParam);
            return 0;
		}

        case WM_LBUTTONUP: {
            game_->buttonReleased(wParam+1);
            return 0;
        }
		case WM_KEYUP: {                            // Has A Key Been Released?
		    game_->buttonReleased(wParam);          // If So, Mark It As FALSE
			return 0;
		}
		case WM_SIZE: {
		    GLfloat xWindow_ = LOWORD(lParam);
		    GLfloat yWindow_ = HIWORD(lParam);
			game_->resizeGame(xWindow_, yWindow_);
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//
//                      THE WINMAIN
//
int WINAPI WinMain(HINSTANCE	hInstance,			        // Instance
                   HINSTANCE	hPrevInstance,		        // Previous Instance
                   LPSTR		lpCmdLine,			        // Command Line Parameters
                   int			nCmdShow)			        // Window Show State
{
	MSG	 msg;									            // Windows Message Structure
									        // Bool Variable To Exit Loop
	//GLfloat fullscreenWidth  = GetSystemMetrics(SM_CXSCREEN);
    //GLfloat fullscreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
    fullscreen = TRUE;

	// Create Our OpenGL Window
	if (!CreateGLWindow("Moko Game", fullscreenWidth, fullscreenHeight, 256, fullscreen))
		return 0;									        // Quit If Window Was Not Created

	while (!done && !game_->currPrase()) {                                         // Loop That Runs While done=FALSE
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {     // Is There A Message Waiting?
			if (msg.message == WM_QUIT)				        // Have We Received A Quit Message?
				done = TRUE;						        // If So done=TRUE
			else {                                          // If Not, Deal With Window Messages
                TranslateMessage(&msg);				        // Translate The Message
				DispatchMessage(&msg);				        // Dispatch The Message
			}
		}
		else										        // If There Are No Messages
		{
			// Draw The Scene. Watch For ESC Key And Quit Messages From DrawGLScene()
			//if (game_->isButtonTrue(VK_ESCAPE))
           //     done = TRUE;
			//else {                                          // Not Time To Quit, Update Screen
			    //If game_->currPhrase Boolean
			    time_now = glutGet(GLUT_ELAPSED_TIME);
			    if ((time_now-time_prev) > 1000/FPS) {
                    game_->settingCurrTime();
                    game_->updatingGame();
                    game_->drawGame();
                    game_->destroyItem();
                    game_->settingPrevTime();
                    SwapBuffers(hDC);
                    time_prev = time_now;
                }

			    !game_->drawGame();
				SwapBuffers(hDC);				            // Swap Buffers (Double Buffering)
			//}

			if (game_->isButtonTrue(VK_F1)) {              // Is F1 Being Pressed?
				game_->buttonReleased(VK_F1);			        // If So Make Key FALSE
				KillGLWindow();						        // Kill Our Current Window
				fullscreen=!fullscreen;				        // Toggle Fullscreen / Windowed Mode
				if (!CreateGLWindow("Moko Game",fullscreenWidth,fullscreenHeight,256,fullscreen))
					return 0;						        // Quit If Window Was Not Created
			}
		}
	}

	// Shutdown
	KillGLWindow();
	return (msg.wParam);							        // Exit The Program
}
