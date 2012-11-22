#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <linux/input.h>
#include <bitset>

using namespace glm;


//extern PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT;
//extern PFNGLGETBUFFERPOINTERVOESPROC	glGetBufferPointervOES;
//extern PFNGLMAPBUFFEROESPROC	glMapBufferOES;
//extern PFNGLUNMAPBUFFEROESPROC	glUnmapBufferOES;


class GLWindow
{
private:

	static uint_fast32_t screen_width, screen_height;

	static uint_fast64_t time_ns;
	static uint_fast32_t dtime_ns;

	static int epoll;
	
	static int mousex, mousey, mousew;
	static int mouselx, mousely, mouselw;
	static std::bitset<KEY_MAX> keystate, keydown, keyup;
	static int fdstdin;
	static char strinbuffer[256];
	static bool newinput;

	static EGLDisplay display;
	static EGLContext context;
	static EGLSurface surface;

	static uint_fast32_t looping;
	static uint_fast32_t frame;

	static bool CompileProgram(const unsigned &sp, const char* attriblist);
	static unsigned CreateShader(const char* filepath, const GLenum &type);
	
	static bool InitInput();
	static void UpdateInput();

public:

	static bool InitShaders(unsigned *shaders, const unsigned *shadertable, const char **fragmentshaders, const unsigned &nfragmentshaders, const char **vertexshaders, const unsigned &nvertexshaders);

	static char *LoadFile(const char *filepath, unsigned *filesize);

	static uint_fast32_t ScreenWidth();
	static uint_fast32_t ScreenHeight();
	static unsigned Frame();

	static float Time();
	static uint_fast64_t TimeNS();
	static float DTime();

	static void BreakLoop();
	static bool InitGL();
	static void Loop(void (*Frame)());

	static bool Key(const unsigned &key);	// current state
	static bool KeyPressed(const unsigned &key);	// key went down in the last frame
	static bool KeyReleased(const unsigned &key);	// key went up in the last frame
	static int MouseX();
	static int MouseY();

	static char *StrInput();
};
