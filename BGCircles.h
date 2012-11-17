#pragma once

#include "Background.h"
#include "Camera.h"


class BGCircles: public Background
{
public:

	BGCircles();
	~BGCircles();

private:

	unsigned vbo;
	
	float cc[4];

	void BGUpdate(const float &time);
	void BGRender();
	void BGRestart();

	unsigned ncircles;
	
	Camera camera;
	mat4 m, t;
};
