#include "Slide.h"

std::vector<Slide*> Slide::vector;
uint_fast32_t Slide::fadelength;
unsigned Slide::currentslide;


bool Slide::Init()
{
	fadelength = 2000000000u;
	currentslide = -1;

	return true;
}


void Slide::Kill()
{
	for(unsigned i = 0; i < vector.size(); ++i)
		delete vector[i];
}


void Slide::Next()
{
	if(currentslide < vector.size() - 1)
		++currentslide;
}


void Slide::Prev()
{
	if(currentslide > 0)
		--currentslide;
}


void Slide::Number(const unsigned &n)
{
	if(n < vector.size() - 1)
		currentslide = n;
	else
		currentslide = vector.size() - 1;
}


void Slide::Update(uint_fast64_t ns, FlatCamera *c)
{
	if(vector.size() != 0)
		vector[currentslide]->UpdateSlide(ns, c);
}


void Slide::Draw()
{
	if(vector.size() != 0)
		vector[currentslide]->DrawSlide();
}


void Slide::Add(Slide *s)
{
	vector.push_back(s);

	if(vector.size() == 1)
		currentslide = 0;
}
