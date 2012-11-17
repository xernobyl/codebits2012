#include "FlatCamera.h"
#include <glm/gtx/euler_angles.hpp>
#include <iostream>


FlatCamera::FlatCamera()
{
	Reset();
	AspectRatio(1.0f);
	UpdateProjection();
}


FlatCamera::FlatCamera(const float &ar)
{
	Reset();
	AspectRatio(ar);
	UpdateProjection();
}


FlatCamera::FlatCamera(const unsigned &w, const unsigned &h)
{
	Reset();
	AspectRatio(w, h);
	UpdateProjection();
}


void FlatCamera::Reset()
{
	scale = 1.0f;
	rotation = 0.0f;
	position[0] = 0.0f;
	position[1] = 0.0f;
	nearp = 1.0f;
	farp = -1.0f;
	aspectratio = 1.0f;
	top = 0.70710678118654752440084436210485f;
	right = 0.70710678118654752440084436210485f;
}


void FlatCamera::AspectRatio(const float &ar)
{
	aspectratio = ar;
	top = sqrt(1.0f / (aspectratio + 1.0f));
	right = aspectratio * top;
}


void FlatCamera::AspectRatio(const unsigned &w, const unsigned &h)
{
	aspectratio = (float)w / h;
	top = sqrt(1.0f / (aspectratio + 1.0f));
	right = aspectratio * top;
}


void FlatCamera::UpdateProjection()
{
	lastm = projm;
	projm = ortho(-right * scale + position[0], right * scale + position[0], -top * scale + position[1], top * scale + position[1], nearp, farp);
	projm = projm * eulerAngleZ(rotation);
}
