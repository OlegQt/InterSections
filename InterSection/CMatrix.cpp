#include "CMatrix.h"
CMatrix::CMatrix() {}
CMatrix::~CMatrix() {}


void CMatrix::rotate(rvector* pArrow, float angle)
{
	angle = angle * (1.00f * PI) / 180.0f;
	//”множаем вектор на матрицу поворота вокруг орты Z
	//x` = x * cos(angle) - y * sin(angle);
	//y` = x * sin(angle) + y * cos(angle);


}

void  CMatrix::sum(rvector* pArrow, float x, float y)
{
	pArrow->x += x;
	pArrow->y += y;
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

