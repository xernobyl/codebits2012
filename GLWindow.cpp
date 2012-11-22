#include "GLWindow.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define ASSERT(X, Y) if(!(X)){ std::cout << Y; return false; }
#define _INITEXT_(TYPE, NAME) NAME = (TYPE)eglGetProcAddress(#NAME); if(!NAME){ std::cout << "Could not init " << #NAME << std::endl; return false; }


uint_fast32_t GLWindow::screen_width, GLWindow::screen_height;

int GLWindow::epoll;

int GLWindow::mousex, GLWindow::mousey, GLWindow::mousew;
int GLWindow::mouselx, GLWindow::mousely, GLWindow::mouselw;
std::bitset<KEY_MAX> GLWindow::keystate, GLWindow::keydown, GLWindow::keyup;
int GLWindow::fdstdin;
char GLWindow::strinbuffer[256];
bool GLWindow::newinput;

EGLDisplay GLWindow::display;
EGLContext GLWindow::context;
EGLSurface GLWindow::surface;

uint_fast32_t GLWindow::looping;
uint_fast32_t GLWindow::frame;

uint_fast64_t GLWindow::time_ns;
uint_fast32_t GLWindow::dtime_ns;


//PFNGLDISCARDFRAMEBUFFEREXTPROC	glDiscardFramebufferEXT;
//PFNGLGETBUFFERPOINTERVOESPROC	glGetBufferPointervOES;
//PFNGLMAPBUFFEROESPROC	glMapBufferOES;
//PFNGLUNMAPBUFFEROESPROC	glUnmapBufferOES;


uint32_t GLWindow::ScreenWidth()
{
	return screen_width;
}


uint32_t GLWindow::ScreenHeight()
{
	return screen_height;
}


unsigned GLWindow::Frame()
{
	return frame;
}


float GLWindow::Time()
{
	return 1e-9f * time_ns;
}


uint_fast64_t GLWindow::TimeNS()
{
	return time_ns;
}

float GLWindow::DTime()
{
	return 1e-9f * dtime_ns;
}


void GLWindow::BreakLoop()
{
	looping = 0;
}


bool GLWindow::InitGL()
{
	static const EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_NONE
	};

	static const EGLint context_attributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLint num_config;
	EGLConfig config;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	ASSERT(display != EGL_NO_DISPLAY, "Could not get display.");
	EGLBoolean result = eglInitialize(display, 0, 0);
	ASSERT(result != EGL_FALSE, "Could not initialize EGL.");

	result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
	ASSERT(result != EGL_FALSE, "Could not find a valid configuration.");
	result = eglBindAPI(EGL_OPENGL_ES_API);
	ASSERT(result != EGL_FALSE, "eglBindAPI error.");

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
	ASSERT(context != EGL_NO_CONTEXT, "Could not create context.");

	DISPMANX_MODEINFO_T mode_info;
	
	DISPMANX_DISPLAY_HANDLE_T dispman_display = vc_dispmanx_display_open(0);	// 0 = LCD
	int success = vc_dispmanx_display_get_info(dispman_display, &mode_info);
	ASSERT(success >= 0, "Could not get the display size.");

	screen_width = mode_info.width;
	screen_height = mode_info.height;

	VC_RECT_T dst_rect;
	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = screen_width;
	dst_rect.height = screen_height;

	VC_RECT_T src_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = screen_width << 16;
	src_rect.height = screen_height << 16;

	static VC_DISPMANX_ALPHA_T alpha =
	{
		DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS,
		255,
		0
	};

	DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start(0);
	DISPMANX_ELEMENT_HANDLE_T dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, &alpha, (DISPMANX_CLAMP_T *)NULL, (DISPMANX_TRANSFORM_T)0);

	static EGL_DISPMANX_WINDOW_T nativewindow;
	nativewindow.element = dispman_element;
	nativewindow.width = screen_width;
	nativewindow.height = screen_height;
	vc_dispmanx_update_submit_sync(dispman_update);

	surface = eglCreateWindowSurface(display, config, &nativewindow, 0);
	ASSERT(surface != EGL_NO_SURFACE, "Could not create a window surface.");
	result = eglMakeCurrent(display, surface, surface, context);
	ASSERT(result != EGL_FALSE, "Could not make current.");

	//std::cout << glGetString(GL_EXTENSIONS) << std::endl;

	//_INITEXT_(PFNGLDISCARDFRAMEBUFFEREXTPROC, glDiscardFramebufferEXT);
	//_INITEXT_(PFNGLGETBUFFERPOINTERVOESPROC, glGetBufferPointervOES);
	//_INITEXT_(PFNGLMAPBUFFEROESPROC, glMapBufferOES);
	//_INITEXT_(PFNGLUNMAPBUFFEROESPROC, glUnmapBufferOES);

	InitInput();

	return true;
}


void GLWindow::Loop(void (*Frame)())
{
	timespec currenttime, lasttime, firsttime;
	clock_gettime(CLOCK_MONOTONIC, &firsttime);
	currenttime = firsttime;

	frame = 0;
	looping = 1;

	do
	{
		lasttime = currenttime;
		clock_gettime(CLOCK_MONOTONIC, &currenttime);

		dtime_ns = 1000000000u * (currenttime.tv_sec - lasttime.tv_sec) + (currenttime.tv_nsec - lasttime.tv_nsec);		// can't be bigger than 4.294967295 seconds!
		time_ns = 1000000000ull * (currenttime.tv_sec - firsttime.tv_sec) + (currenttime.tv_nsec - firsttime.tv_nsec);	// can't be bigger than 584.54204609062639791999391588714 years!

		UpdateInput();

		Frame();
		eglSwapBuffers(display, surface);
		++frame;
	} while(looping);

	std::cout << frame << " frames drawn, average: " << 1e9f * frame / time_ns << std::endl;
}


char *GLWindow::LoadFile(const char *filepath, unsigned *filesize)
{
	FILE *file = fopen(filepath, "rb");

	if(!file)
	{
		std::cout << "Could not open " << filepath << std::endl;
		return 0;
	}

	fseek(file, 0, SEEK_END);
	unsigned size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *buffer = new char[size];
	fread(buffer, size, 1, file);

	fclose(file);

	if(filesize)
		*filesize = size;

	if(size == 0)
	{
		delete [] buffer;
		return 0;
	}

	return buffer;
}


bool GLWindow::CompileProgram(const unsigned &sp, const char* attriblist)
{
	static const char defattriblist[] =
	{
		'A', '0', '\0', 0,
		'A', '1', '\0', 1,
		'A', '2', '\0', 2,
		'A', '3', '\0', 3,
		0
	};

	int result;

	if(!attriblist)
		attriblist = defattriblist;

	while(*attriblist)
	{
		const char *attrib = attriblist;
		attriblist += strlen(attrib) + 1;
		unsigned nattrib = (unsigned)*attriblist;

		glBindAttribLocation(sp, nattrib, attrib);

		++attriblist;
	}

	glLinkProgram(sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &result);

	if(result != GL_TRUE)
	{
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &result);
		char* tmp = new char[result];
		glGetProgramInfoLog(sp, result, 0, tmp);

		std::cout << "Could not link program:" << std::endl << tmp << std::endl;

		delete tmp;

		return false;
	}

	return true;
}


unsigned GLWindow::CreateShader(const char* filepath, const GLenum &type)
{
	int result, shaderfilelen;

	char* shaderfile = (char*)LoadFile(filepath, (unsigned*)&shaderfilelen);
	if(!shaderfile)
		return 0;

	unsigned shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&shaderfile, &shaderfilelen);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	delete [] shaderfile;

	if(result != GL_TRUE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		char* tmp = new char[result];
		glGetShaderInfoLog(shader, result, 0, tmp);

		std::cout << "Could not compile shader:" << std::endl << tmp << std::endl;

		return 0;
	}
	else
		return shader;
}


bool GLWindow::InitShaders(unsigned *shaders, const unsigned *shadertable, const char **fragmentshaders, const unsigned &nfragmentshaders, const char **vertexshaders, const unsigned &nvertexshaders)
{
	unsigned *fs = new unsigned[nfragmentshaders];
	unsigned *vs = new unsigned[nvertexshaders];

	char tmp[32];

	for(unsigned i = 0; i < nfragmentshaders; ++i)
	{
		sprintf(tmp, "glsl/%s.fs", fragmentshaders[i]);

		if(!(fs[i] = CreateShader(tmp, GL_FRAGMENT_SHADER)))
			return false;
	}

	for(unsigned i = 0; i < nvertexshaders; ++i)
	{
		sprintf(tmp, "glsl/%s.vs", vertexshaders[i]);

		if(!(vs[i] = CreateShader(tmp, GL_VERTEX_SHADER)))
			return false;
	}

	unsigned nshaders;

	while((nshaders = *shadertable++))
	{
		*shaders = glCreateProgram();

		for(unsigned i = 0; i < nshaders; ++i)
		{
			unsigned t = *shadertable++;
			unsigned s = t & 0x3fff;

			if(t & 0x4000)
				s = vs[s];
			else
				s = fs[s];

			glAttachShader(*shaders, s);
		}

		if(!CompileProgram(*shaders, 0))
			return false;

		++shaders;
	}

	for(unsigned i=0; i<nfragmentshaders; ++i)
		glDeleteShader(fs[i]);

	for(unsigned i=0; i<nvertexshaders; ++i)
		glDeleteShader(vs[i]);

	delete [] fs;
	delete [] vs;
	
	return true;
}


bool GLWindow::InitInput()
{
	char inputpath[18] = "/dev/input/event\0";

	mousex = 0;
	mousey = 0;

	epoll = epoll_create1(0);

	epoll_event event;
	event.events = EPOLLIN | EPOLLET;

	for(unsigned i = 0; i < 10; ++i)
	{
		inputpath[16] = '0' + i;
		event.data.fd = open(inputpath, O_RDONLY | O_NONBLOCK);

		if(event.data.fd <= 0)
			continue;
		else
			epoll_ctl(epoll, EPOLL_CTL_ADD, event.data.fd, &event);
	}

	event.data.fd = fdstdin = open("/dev/stdin", O_RDONLY | O_NONBLOCK);
	epoll_ctl(epoll, EPOLL_CTL_ADD, event.data.fd, &event);

	return true;
}


bool GLWindow::Key(const unsigned &key)
{
	return keystate[key];
}


bool GLWindow::KeyPressed(const unsigned &key)
{
	return keydown[key];
}


bool GLWindow::KeyReleased(const unsigned &key)
{
	return keyup[key];
}


int GLWindow::MouseX()
{
	return mousex;
}


int GLWindow::MouseY()
{
	return mousey;
}


void GLWindow::UpdateInput()
{
	enum
	{
		MAX_EVENTS = 4,
	};

	static epoll_event events[MAX_EVENTS];
	static input_event ie;

	keydown.reset();
	keyup.reset();

	mouselx = mousex;
	mousely = mousey;
	mouselw = mousew;

	int n = epoll_wait(epoll, events, MAX_EVENTS, 0);
	
	newinput = false;

	for(int i = 0; i < n; ++i)	// for each message
	{
		if(events[i].data.fd == fdstdin)
		{
			newinput = true;
			read(events[i].data.fd, strinbuffer, 256);
		}
		else
		{
			while(read(events[i].data.fd, &ie, sizeof(input_event)) == sizeof(input_event))
			{
				switch(ie.type)
				{
				case EV_KEY:
					keystate[ie.code] = ie.value;
					
					if(ie.value)
						keydown[ie.code] = true;
					else
						keyup[ie.code] = true;

					break;

				case EV_REL:
					switch(ie.code)
					{
					case REL_X:
						mousex += ie.value;
						break;
					case REL_Y:
						mousey -= ie.value;
						break;
					case REL_WHEEL:
						mousew += ie.value;
					};
					break;

				/*case EV_ABS:
					switch(ie.code)
					{
					case ABS_X:
						mousex = ie.value;
						break;
					case ABS_Y:
						mousey = ie.value;
						break;
					case ABS_WHEEL:
						mousew = ie.value;
					};
					break;*/

				/*case EV_MSC:
					switch(ie.code)
					{
					case MSC_SCAN:
						break;
					};
					break;*/

				//case EV_SYN:
				//case EV_LED:
				//case EV_SND:
				//case EV_REP:
				//case EV_FF:
				//case EV_PWR:
				//case EV_FF_STATUS:
				//case EV_MAX:
				}

				//printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n", ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
			}
		}
	}
}


char *GLWindow::StrInput()
{
	if(newinput)
		return strinbuffer;
	else
		return 0;
}