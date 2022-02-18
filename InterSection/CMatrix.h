#pragma once
#include <windows.h>
#include <iostream>
#include <cmath>

#include "Element.h"

#define PI 3.14159265

class CMatrix
{
	CMatrix();
	~CMatrix();
public:

public:
	static rvector Rotate(rvector,float);
	static rvector SumVectors(rvector,rvector,int);
	static rvector ScaleVector(rvector,float);
	static rvector GetProection(rvector a, rvector b);
	static float GetAngle(rvector a, rvector b);
	static float GetVectorLength(rvector a);

};

