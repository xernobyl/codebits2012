#pragma once

#include "Background.h"
#include "Camera.h"


class BGTunnel: public Background
{
public:

	BGTunnel();
	~BGTunnel();

private:

	void BGUpdate(const float &time);
	void BGRender();
	void BGRestart();

	mat4 m;
	float z;
	float s;
};
