#pragma once
#include <windows.h>
#include <iostream>
#include <cmath>
#include "CArrow.h"
#define PI 3.14159265

class CMatrix
{
	CMatrix();
	~CMatrix();
public:
	static void  rotate(CArrow *,float);
	static void sum(CArrow *, float x, float y);
};

