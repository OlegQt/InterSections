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
	static void  rotate(rvector*,float);
	static void sum(rvector *, float x, float y);
	static rvector SumVectors(rvector,rvector,int);
	static rvector ScaleVector(rvector,float);

};

