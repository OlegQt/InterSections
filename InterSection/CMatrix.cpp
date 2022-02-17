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
