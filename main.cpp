#include <iostream>
#include "GLWindow.h"
#include "Shaders.h"
#include "Quad.h"
#include "Resources.h"
#include "Camera.h"
#include "FlatCamera.h"
#include "Particles.h"
#include "Text.h"
#include "Model.h"
#include "Resources.h"
#include "Background.h"
#include "BGCircles.h"
#include "BGPlanes.h"
#include "BGTunnel.h"
#include "Slide.h"
#include "TextSlide.h"

#include "bcm_host.h"


Camera camera;
FlatCamera flatcamera;
//Particles *particles;

Text *text0;

bool knext = false, kprev = false;


void Render()
{
	if(GLWindow::KeyPressed(BTN_MIDDLE) || GLWindow::KeyPressed(KEY_ESC))
		GLWindow::BreakLoop();

	if(GLWindow::KeyPressed(BTN_LEFT))
		Slide::Next();
	
	if(GLWindow::KeyPressed(BTN_RIGHT))
		Slide::Prev();
		
	if(GLWindow::StrInput())
	{
		if(strcmp("quit\n", GLWindow::StrInput()) == 0)
			GLWindow::BreakLoop();

		if(strcmp("prev\n", GLWindow::StrInput()) == 0)
			Slide::Prev();

		if(strcmp("next\n", GLWindow::StrInput()) == 0)
			Slide::Next();
		
		unsigned n;

		if(sscanf(GLWindow::StrInput(), "slide %u", &n) == 1)
			Slide::Number(n);
	}

	Background::Update(GLWindow::TimeNS());
	Slide::Update(GLWindow::TimeNS(), &flatcamera);

	/*GLenum attachments[] =
	{
		GL_COLOR_EXT,
		//GL_DEPTH_EXT,
		//GL_STENCIL_EXT
	};*/

	Background::Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GLWindow::ScreenWidth(), GLWindow::ScreenHeight());
	//glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, attachments);

	Background::Draw();
	Slide::Draw();
}


int main(int argc, char *argv[])
{
	bcm_host_init();

	if(GLWindow::InitGL())
	{
		if(!Shader::InitAll())
			return 0;

		if(!Resources::Init())
			return 0;

		Quad::Init();

		glEnableVertexAttribArray(0);
		glDisable(GL_DITHER);

		flatcamera.Reset();
		flatcamera.AspectRatio(GLWindow::ScreenWidth(), GLWindow::ScreenHeight());
		flatcamera.UpdateProjection();

		camera.AspectRatio(GLWindow::ScreenWidth(), GLWindow::ScreenHeight());
		camera.UpdateProjection();

		if(!Background::Init(GLWindow::ScreenWidth() / 4, GLWindow::ScreenHeight() / 4))
		{
			std::cout << "Could not init buffers." << std::endl;
			return 0;
		}

		Background::Add(new BGTunnel());
		Background::Add(new BGCircles());
		Background::Add(new BGPlanes());
		
		Slide::Init();

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "Disclaimer:\n"
			"This is a work of fiction. Names, characters,\n"
			"places and incidents either are products of the\n"
			"author's imagination or are used fictitiously.\n"
			"Any resemblance to actual events or locales or\n"
			"persons, living or dead, is entirely coincidental.", 250, 0.03f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "OpenGL ES 2.0 Programming\non a Raspberry Pi\n(!!!111!!one!!1!eleven!1!)", 70, 0.11f, true));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "Codebits 2012\n\n@xernobyl\nTiago Chagas Farto", 43, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "take 2 @ codebits", 17, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "  github.com/xernobyl  ", 23, 0.1f));	// BUG

		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "part I", 6, 0.125f, true));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "Why computer graphics?", 22, 0.125f, true));
		
		// wireframe cube goes here
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...books? ...tutorials?", 23, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "  ...code!  ", 12, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "windows\nopengl\ndirectx\nlinux\nwindows again...", 45, 0.075f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "rarely copied code\nnever used an engine", 39, 0.1f, true));

		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "  \"click\"  ", 11, 0.1f));	// BUG

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "deferred renderer\nlight pre pass renderer\nambient occlusion\nglobal illumination\nphysically based rendering", 106, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "  POWER  ", 9, 0.2f));	// BUG
	
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "So I tried Android for a while...", 33, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "  ...and WebGL.  ", 17, 0.1f));	// BUG
		//Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "\"appealing to nonprofessional programmers, like Microsoft's Visual Basic\"", 7, 0.05f));	// BUG
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "I like control.", 15, 0.125f, true));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "libgml\nlibraspberrypi", 21, 0.01f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "Raspberry Pi", 12, 0.15f, true));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "700Mhz CPU\n250Mhz GPU\n256MB RAM", 31, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "it's cheap!", 11, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "   Linux   ", 11, 0.125f));	// BUG
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "the good part!", 14, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...perhaps your own OS?", 23, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "OpenGL ES 2.0", 13, 0.15f, true));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "march 2007", 10, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "state machine", 13, 0.1f));
		
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], ">121 functions!", 15, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "Buffers Viewport and clipping Reading pixels Vertices Texturing\nRasterization Pixel rectangles Shaders and programs Shader queries\nPer-fragment operations Special functions State and state Requests\nWhole framebuffer operations Framebuffer objects", 246, 0.04f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...and a few extensions", 23, 0.125f));

		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO],
			"GL_OES_compressed_ETC1_RGB8_texture\n"
			"GL_OES_compressed_paletted_texture\n"
			"GL_OES_texture_npot\n"
			"GL_OES_depth24\n"
			"GL_OES_vertex_half_float\n"
			"GL_OES_EGL_image\n"
			"GL_OES_EGL_image_external\n"
			"GL_EXT_discard_framebuffer\n"
			"GL_OES_rgb8_rgba8\n"
			"GL_OES_depth32\n"
			"GL_OES_mapbuffer\n"
			"GL_EXT_texture_format_BGRA8888\n"
			"GL_APPLE_rgb_422\n"
			"GL_EXT_debug_marker", 318, 0.0625f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], " asynchronous ", 14, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "context:\n\ncolor buffer\ndepth buffer\nstencil buffer", 50, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...lets get moving", 18, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "{\n"
			"\tDrawFrame();\n"
			"\teglSwapBuffers(display, surface);\n"
			"\t} while(looping);", 69, 0.05f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "end of part I", 13, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "questions so far?", 17, 0.1f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "A black screen\nis usually easy.", 31, 0.15f, true));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glClearColor(0.0f, 0.0f, 0.0f, 0.0f);\nglClear(GL_COLOR_BUFFER_BIT);", 67, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "OK. What's next?", 16, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "void DrawArrays(enum mode, int first, sizei count);\nvoid DrawElements(enum mode, sizei count, enum type, void *indices);", 120, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "POINTS, TRIANGLES, LINES, LINE_STRIP, LINE_LOOP", 47, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], " vertices ", 10, 0.2f, true));	// BUG
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], " attributes ", 12, 0.1f));		// BUG
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "you need to know where you are", 30, 0.1f));

		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glVertexAttribPointer(number, size, type, normalize, stride, offset);\nglEnableVertexAttribArray(number);", 104, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "vertex buffer objects", 21, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glGenBuffers(1, &vbo);\nglBindBuffer(GL_ARRAY_BUFFER, vbo);\nglBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);", 117, 0.075f));

		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "STREAM\nSTATIC\nDYNAMIC", 21, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], " 2-manifold ", 12, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "a tetrahedron", 13, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "each vertex is shared by 3 faces", 32, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "the solution is to index", 24, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glGenBuffers(1, &ibo);\nglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);\nglBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);", 133, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "like the previous... but with ELEMENT", 37, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "And speaking of EXTENSIONS...", 29, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "void * glMapBuffer(GLenum target, GLenum access);\nGLboolean glUnmapBuffer(GLenum target);", 89, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "And since I've mentiond 2-manifolds...", 38, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glEnable(GL_DEPTH_TEST);\nglEnable(GL_CULL_FACE);", 48, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glClear(GL_DEPTH_BUFFER_BIT);", 48, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glDisable(GL_DEPTH_TEST);\nglDisable(GL_CULL_FACE);", 50, 0.1f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], " Shaders!!1!!11!! ", 18, 0.2f, true));	// BUG
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "are essentially a set of callbacks", 34, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "one for vertices", 16, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "another for fragments", 21, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "shader = glCreateShader(GL_VERTEX_SHADER);	// or GL_FRAGMENT_SHADER\nglShaderSource(shader, 1, shaderfile, shaderfilelen);\nglCompileShader(shader);", 146, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "do it a few times", 17, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "program = glCreateProgram();\nglAttachShader(program, shader[i]);\nglBindAttribLocation(program, attrib_number, \"ATTRIB_NAME\");\nglLinkProgram(program);", 149, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...and what exactly is a shader?", 32, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "   GLSL   ", 10, 0.1f));	// BUG

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...a vertex shader...", 21, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "attribute vec4 A0;	// defaults to vec4(0.0, 0.0, 0.0, 1.0)\nuniform mat4 a_matrix;\nvarying float Im_not_doing_anything;\n\nvoid main()\n{\n\t// javascript people: that's the correct way to use parentheses\n\tgl_Position = a_matrix * A0;\n\tIm_not_doing_anything = 1337.0;\n}", 225, 0.05f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...and a fragment shader...", 27, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "precision lowp float;\nvarying float Im_not_doing_anything;\n\nvoid main()\n{\n\tgl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);	// BENFICA!!! (MAN U!!! for the britts)\n}", 157, 0.05f));

		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "You need all that to show something on screen.", 27, 0.1f));
		
		// RED CUBE
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "But there's not much more to know.", 27, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "this concludes the 2nd part", 27, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "...any questions so far?", 24, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glEnable(GL_BLEND);\nglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);", 70, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "  textures  ", 12, 0.2f, true));	// BUG
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "OGLES2 supports 2 kinds of textures", 35, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glGenTextures(1, &texture);\nglBindTexture(GL_TEXTURE_2D, texture);", 66, 0.075f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "void TexImage2D(enum target, int level, int internalformat, sizei width, sizei height, int border, enum format, enum type, void *data);\nor\nvoid CompressedTexImage2D(enum target, int level, enum internalformat, sizei width, sizei height, int border, sizei imageSize, void *data);", 278, 0.06f));
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);\nglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);", 146, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "and to use them", 15, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glActiveTexture(GL_TEXTURE0 + look_at_me);\nglBindTexture(GL_TEXTURE_2D, texture);\nglUniformi(location, look_at_me)", 116, 0.075f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "draw stuff with textures", 24, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "only one major feature is missing", 33, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "framebuffer objects", 24, 0.2f, true));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "color depth stencil", 19, 0.1f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glGenRenderbuffers(1, &depthrenderbuffer);\nglBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);\nglRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);	// DEPTH_COMPONENT24_OES or 32\n\nglBindFramebuffer(GL_FRAMEBUFFER, framebuffer);\nglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);\nglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertexture, 0);", 455, 0.05f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);	// 0 for main\nglViewport(0, 0, width, height);", 94, 0.05f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "And that's it!", 14, 0.1f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "A few performance notes...", 14, 0.1f, true));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "use the smallest possible formats\navoid overdraw\nstate changes\nreuse\n...", 71, 0.1f));

		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "OpenGL ES 2.0 Quick Reference\nopengl.org/wiki\nkhronos.org/opengles/sdk/docs/man\n\ncode.google.com/p/opengles-book-samples\n\nassimp.sourceforge.net\naras-p.info/blog/2010/09/29/glsl-optimizer\n\nraspberrypi.org", 213, 0.05f));
		
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], "Thank you.", 10, 0.15f));
		Slide::Add(new TextSlide(&Resources::font[F_FUTURA], " Questions? ", 12, 0.10f));
		
		Slide::Add(new TextSlide(&Resources::font[F_ANONYMOUSPRO], "  github.com/xernobyl  ", 23, 0.1f));	// BUG

		GLWindow::Loop(Render);
	}

	return 0;
}
