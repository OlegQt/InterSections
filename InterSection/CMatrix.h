#pragma once
#include <windows.h>
#include <iostream>
#include <cmath>
#define PI 3.14159265

class CMatrix
{
	CMatrix();
	~CMatrix();
public:
	struct rvector {
		float x, y;
	};
public:
	static void  rotate(rvector*,float);
	static void sum(rvector *, float x, float y);


};

