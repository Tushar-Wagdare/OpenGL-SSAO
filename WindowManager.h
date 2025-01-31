#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <windows.h>


class WindowManager
{
	public:
		static int SCR_WIDTH;
		static int SCR_HEIGHT;

		HWND windowHandle;
		HINSTANCE currentInstance;
		HGLRC renderingContext;
		HDC deviceContext;
		BOOL isRunning;
		
		WindowManager();
		~WindowManager();
		bool initialize();
		void uninitialize();
		void bind();
		void swapDisplayBuffer();
		bool initializeWin32(); 
		bool initializeOpenGL();
		bool initializeImGUI();
		void printGLInfo();
		ATOM MyRegisterClass();
		BOOL InitInstance();
};

#endif //WINDOWMANAGER_H
