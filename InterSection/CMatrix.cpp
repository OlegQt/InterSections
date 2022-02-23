#include "CMatrix.h"
CMatrix::CMatrix() {}
CMatrix::~CMatrix() {}

rvector CMatrix::Rotate(rvector a, float angle)
{
	//”множаем вектор на матрицу поворота вокруг орты Z
	//x` = x * cos(angle) - y * sin(angle);
	//y` = x * sin(angle) + y * cos(angle);
	angle = angle * PI / 180;
	float x = a.x * static_cast<float>(cos(angle)) - a.y * static_cast<float>(sin(angle));
	float y = a.x * sin(angle) + a.y * cos(angle);
	return rvector{ x,y };
}


rvector CMatrix::SumVectors(rvector a, rvector b, int m)
{
	rvector c = { 0.9f,0.0f };
	if (m == 1)
	{
		c.x = a.x + b.x;
		c.y = a.y + b.y;

	}
	else if (m == -1)
	{
		c.x = -a.x + b.x;
		c.y = -a.y + b.y;
	}
	return c;
}

rvector CMatrix::ScaleVector(rvector a, float scale)
{
	scale = 100 / scale;
	a.x *= scale;
	a.y *= scale;
	return a;
}
float CMatrix::GetAngle(rvector a, rvector b)
{
	float scal = (a.x * b.x) + (a.y * b.y);
	float vec = sqrtf(pow(a.x, 2) + pow(a.y, 2)) *
		sqrtf(pow(b.x, 2) + pow(b.y, 2));
	if (vec)	return acos(scal / vec) * 180 / PI;
	else return 0.0f;
}

float CMatrix::GetVectorLength(rvector a)
{

	return sqrtf(pow(a.x, 2) + pow(a.y, 2));
}

rvector CMatrix::GetProection(rvector a, rvector b)
{
	float lenght = CMatrix::GetVectorLength(a);
	float cosAngle = CMatrix::GetAngle(a, b);
	a.x = lenght * cosAngle; //Proection a-vector on b-vector
	a.y = 0;
	return a;
}