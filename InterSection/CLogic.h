#pragma once
#include "CArrow.h"
#include "CMatrix.h"
#include <vector>

#define STARN 64

class CLogic
{
public:
	CLogic();
	~CLogic();
	void AddArrow(float,float,float,float);

	unsigned int GetArraySize();
private:
	
	std::vector<CArrow>array;
};

