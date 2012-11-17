#pragma once

#include "GLWindow.h"


class Camera
{
public:

	Camera();

	void LookAt(const vec3 &pos, const vec3 &t, const vec3 &up);

	void UpdateProjection();

	void FocalLength(const float &fovmm);
	void FieldOfView(const float &fovy){ fov = fovy; };
	float FieldOfViewH() const{ return fov * aspect; };
	float FieldOfViewV() const{ return fov; };

	void AspectRatio(const float &ar){ aspect = ar; };
	void AspectRatio(const unsigned &w, const unsigned &h);
	float AspectRatio() const{ return aspect; };

	void NearP(const float &n){ nearp = n; };
	void FarP(const float &f){ farp = f; };

	float NearP() const{ return nearp; };
	float FarP() const{ return farp; };

	mat4 *MVP(){ return &mvpm; };
	mat4 *MV(){ return &viewm; };
	mat4 *P(){ return &projm; };

	float TanHalfFOV() const{ return tanhalffov; };

	vec3 Position() const{ return position; };

private:

	vec3 position;

	mat4 viewm, projm, mvpm, lastm;

	float fov;				// vertical fov
	float tanhalffov;
	float aspect;			// aspect ratio
	float nearp, farp;

	float focallength;
};
