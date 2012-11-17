#pragma once

#include "GLWindow.h"


class FlatCamera
{
public:

	FlatCamera();
	FlatCamera(const float &aspectratio);
	FlatCamera(const unsigned &w, const unsigned &h);

	void Reset();
	void UpdateProjection();

	void AspectRatio(const float &aspectratio);
	void AspectRatio(const unsigned &w, const unsigned &h);
	float AspectRatio() const{ return aspectratio; };

	mat4 *P(){ return &projm; };

	void Rotation(const float &r){ rotation = r; };
	void Position(const float &x, const float &y){ position[0] = x; position[1] = y; };

private:

	float aspectratio;
	float top, right;

	float nearp, farp;

	float position[2];
	float scale;
	float rotation;

	mat4 projm, lastm;
};
