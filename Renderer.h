#pragma once

#include "GLWindow.h"


class Renderer
{
public:

	static void Init(unsigned w, unsigned h, float ar);
	static void Kill();

private:

	static unsigned DepthTexture();
	static unsigned SceneTexture();
};
