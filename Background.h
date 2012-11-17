#pragma once

#include "GLWindow.h"
#include "FlatCamera.h"
#include <vector>


class Background
{
public:

	static bool Init(const unsigned &w, const unsigned &h);
	static void Kill();

	static void Update(uint_fast64_t ns);
	static void Draw();
	static void Render();

	static void Add(Background *bg);
	
	static unsigned Width();
	static unsigned Height();

protected:

	static FlatCamera flatcamera;

	static void StartRender();

	virtual void BGUpdate(const float &time) = 0;
	virtual void BGRender() = 0;
	virtual void BGRestart() = 0;

private:

	static uint_fast64_t duration;
	static uint_fast32_t fadelength;
	static float fadetime;
	static bool fading;
	static unsigned fadefx;

	static std::vector<Background*> vector;
	static unsigned width, height;
	static unsigned framebuffertexture[3], framebuffer[3], depthrenderbuffer;

	static unsigned currentbackground;
	static unsigned currentbuffer;
};
