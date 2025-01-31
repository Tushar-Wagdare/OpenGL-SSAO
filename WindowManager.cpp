#include "WindowManager.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <gl/GL.h>

#include "Logger.h"
#include "Timer.h"
#include "Resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WindowManager::SCR_WIDTH  = 1920;
int WindowManager::SCR_HEIGHT = 1080;
clock_t Timer::start = 0.0;

WindowManager::WindowManager() {
    Timer::startMyTimer();
    currentInstance = GetModuleHandle(NULL);
    windowHandle = NULL;
    renderingContext = NULL;
    deviceContext = NULL;
    isRunning = FALSE;
};

WindowManager::~WindowManager() {};

bool WindowManager:: initialize() 
{
    if (!currentInstance) 
    {
        LOG_ERROR("Current Instance NULL");
        return false;
    }
    else 
    {
        LOG_INFO("Current Instance Valid");
    }


    TIMER_INIT("Win32");
    if (!initializeWin32())  
    {
        LOG_ERROR("Failed to initialize Win32 Window.\n");
        TIMER_END();
        return false;
    }
    else 
    { 
        TIMER_END(); 
        LOG_INFO("initializeWin32 Initialized in %.6f seconds", TIMER_GET("Win32")); 
    }


    TIMER_INIT("OpenGL");
    if (!initializeOpenGL()) 
    {
        LOG_ERROR("Failed to initialize OpenGL.\n");
        TIMER_END();
        return false;
    }
    else 
    { 
        TIMER_END(); 
        LOG_INFO("OpenGL Initialized in %.6f seconds", TIMER_GET("OpenGL")); 
    }
   
    /*
    if (!initializeImGUI()) 
    {
        LOG_ERROR("Failed to initialize ImGUI.\n");
        return false;
    */

    return true;
}

ATOM WindowManager::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc; 
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DASHAVTAR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = MAKEINTRESOURCEW(NULL);
    wcex.lpszClassName = TEXT("DASHAVTAR");
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_DASHAVTAR));

    return RegisterClassExW(&wcex);
}

BOOL WindowManager::InitInstance()
{
    windowHandle = CreateWindowW(TEXT("DASHAVTAR"), TEXT("OpenGL : SSAO"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, currentInstance, nullptr);

    if (!windowHandle)
    {
        printf("Window handle found to be NULL after createWindow");
        return FALSE;
    }

    ShowWindow(windowHandle, SW_SHOW);
    UpdateWindow(windowHandle);

    return TRUE;
}

void WindowManager::uninitialize() {

   LOG_INFO("Window handle found to be NULL after createWindow");
};

void WindowManager::bind() {};

void WindowManager::swapDisplayBuffer() {
    SwapBuffers(deviceContext);
};

bool WindowManager::initializeWin32() {
    MONITORINFO mi = { sizeof(MONITORINFO) };
    DWORD dwStyle = 0;
    WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
    // Initialize global strings
    MyRegisterClass();

    // Perform application initialization:
    if (!InitInstance())
    {
        printf("InitInstance() failed");
        return false;
    }

   /* dwStyle = GetWindowLong(windowHandle, GWL_STYLE);
    if (dwStyle & WS_OVERLAPPEDWINDOW)
    {
        if (GetWindowPlacement(windowHandle, &wpPrev) && GetMonitorInfo(MonitorFromWindow(windowHandle, MONITORINFOF_PRIMARY), &mi))
        {
            SetWindowLong(windowHandle, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(windowHandle, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }
    ShowCursor(FALSE);*/

    return true;

};

bool WindowManager::initializeOpenGL() { 
    //variable declaration
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;

    //code
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cStencilBits = 8;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;

    // initialization of PIXELFORMATDESCRIPOTOR STRUCTURE COMPLETE
    // get dc
    deviceContext = GetDC(windowHandle);

    // choose pixel format
    iPixelFormatIndex = ChoosePixelFormat(deviceContext, &pfd);
    if (iPixelFormatIndex == 0)
    {
        LOG_ERROR("ChoosePixelFormat Failed");
        return false;
    }

    // set the choosen pixel format 
    if (SetPixelFormat(deviceContext, iPixelFormatIndex, &pfd) == FALSE)
    {
        LOG_ERROR("SetPixelFormat Failed");
        return false;
    }

    // Create a temporary context to get the wglCreateContextAttribsARB function
    HGLRC tempContext = wglCreateContext(deviceContext);
    if (tempContext == NULL)
    {
        LOG_ERROR("Temporary wglCreateContext Failed");
        return false;
    }

    if (wglMakeCurrent(deviceContext, tempContext) == FALSE)
    {
        LOG_ERROR("Temporary wglMakeCurrent Failed");
        return false;
    }

    // Get the modern context creation function
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
    (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (wglCreateContextAttribsARB == NULL)
    {
        LOG_ERROR("wglCreateContextAttribsARB not found");
        return false;
    }

    //// Specify new pixel format with multisampling
    //const int pixelAttribs[] = {
    //    //WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    //    //WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    //    //WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    //    //WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    //    //WGL_COLOR_BITS_ARB, 32,
    //    //WGL_DEPTH_BITS_ARB, 24,
    //    //WGL_STENCIL_BITS_ARB, 8,
    //    //WGL_SAMPLE_BUFFERS_ARB, 1,     // Enable multisampling
    //    WGL_SAMPLES_ARB, 8,            // 4x multisampling
    //    0  // Terminator
    //};

    //int pixelFormat;
    //UINT numFormats;
    //wglChoosePixelFormatARB(deviceContext, pixelAttribs, NULL, 1, &pixelFormat, &numFormats);

    //if (!numFormats) {
    //    LOG_ERROR("Couldn't find appropriate pixel format");
    //    return false;
    //}

    //// Get the full PDF for this pixel format
    //PIXELFORMATDESCRIPTOR finalPfd;
    //DescribePixelFormat(deviceContext, pixelFormat, sizeof(finalPfd), &finalPfd);

    //// Set the pixel format
    //if (!SetPixelFormat(deviceContext, pixelFormat, &finalPfd)) {
    //    LOG_ERROR("Couldn't set pixel format");
    //    return false;
    //}

    // Specify that we want an OpenGL 4.6 core profile context
    const int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0 // Terminator
    };

    // Create modern OpenGL context
    renderingContext = wglCreateContextAttribsARB(deviceContext, NULL, contextAttribs);

    // Delete temporary context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);

    if (renderingContext == NULL)
    {
        LOG_ERROR("wglCreateContextAttribsARB Failed");
        return false;
    }

    // Make the modern context current
    if (wglMakeCurrent(deviceContext, renderingContext) == FALSE)
    {
        LOG_ERROR("Modern wglMakeCurrent Failed");
        return false;
    }
    //glew initialization 

    if (glewInit() != GLEW_OK)
    {
        LOG_ERROR("glewInit Failed");
        return false;
    }

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_MULTISAMPLE);
    //printGLInfo();
   
    return true; 
};

void WindowManager::printGLInfo(void)
{
    //local variable declarations
    GLint numExtensions = 0;

    //code
    LOG_INFO("OpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
    LOG_INFO("OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
    LOG_INFO("OpenGL Version  : %s\n", glGetString(GL_VERSION));
    LOG_INFO("GLSL Version    : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); //GLSL - graphic library shading language


    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    LOG_INFO( "Number of Supported extensions : %d \n", numExtensions);

    for (int i = 0; i < numExtensions; i++)
    {
        LOG_INFO("%s \n", glGetStringi(GL_EXTENSIONS, i));
    }



}

bool WindowManager::initializeImGUI() { return true; };



