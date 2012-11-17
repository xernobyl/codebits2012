#pragma once

#include "GLWindow.h"
#include "FlatCamera.h"
#include <vector>


class Slide
{
public:

	static bool Init();
	static void Kill();
	
	static void Next();
	static void Prev();
	static void Number(const unsigned &n);

	static void Update(uint_fast64_t ns, FlatCamera *c);
	static void Draw();
	
	static void Add(Slide *s);

protected:

	virtual void UpdateSlide(uint_fast64_t ns, FlatCamera *c) = 0;
	virtual void DrawSlide() = 0;

private:

	static std::vector<Slide*> vector;
	static unsigned currentslide;
	static uint_fast32_t fadelength;
};
