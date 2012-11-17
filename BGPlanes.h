#pragma once

#include "Background.h"
#include "Camera.h"


class BGPlanes: public Background
{
public:

	BGPlanes();
	~BGPlanes();

private:

	void BGUpdate(const float &time);
	void BGRender();
	void BGRestart();

	mat4 m;
	float z;
	float s;
};
