#include "CLogic.h"
CLogic::CLogic()
{
}
CLogic::~CLogic()
{
	this->array.clear();
}

void CLogic::AddArrow(float xA, float yA, float xB, float yB)
{
	this->array.push_back(CArrow(xA, yA, xB, yB));
}
unsigned int CLogic::GetArraySize()
{
	return this->array.size();
}
