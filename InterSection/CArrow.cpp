#include "CArrow.h"

CArrow::CArrow(float x, float y, float d)
{
	this->xPos = x;
	this->yPos = y;
	
	this->Vx = 0.0f;
	this->Vy = 0.0f;
	this->Vz = 0;

	this->Diameter = d;
}
CArrow::CArrow()
{}
CArrow::CArrow(float xA, float yA, float xB, float yB)
{
	this->xPos = xA;
	this->yPos = yA;

	this->Vx = xB;
	this->Vy = yB;
}
CArrow::~CArrow()
{
}

