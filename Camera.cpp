#include "Camera.h"


Camera::Camera()
{
	FocalLength(50.0f);
	nearp = 0.1f;
	farp = 100.0f;
	aspect = 1.0f;
}


void Camera::LookAt(const vec3 &pos, const vec3 &t, const vec3 &up)
{
	viewm = lookAt(pos, t, up);
	mvpm = projm * viewm;
	position = pos;
}


void Camera::FocalLength(const float &fovmm)
{
	float dfov = 2.0f * atan(18.0f / fovmm);
	fov = sqrt(dfov * dfov / (1.0f + aspect * aspect));
	focallength = fovmm / 1000.0f;
}


void Camera::UpdateProjection()
{
	projm = perspective(glm::degrees(fov), aspect, nearp, farp);
	tanhalffov = tan(fov * 0.5f);
}


void Camera::AspectRatio(const unsigned &w, const unsigned &h)
{
	aspect = (float)w / h;
}
