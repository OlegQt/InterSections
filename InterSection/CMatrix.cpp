#include "CMatrix.h"
CMatrix::CMatrix() {}
CMatrix::~CMatrix() {}


void CMatrix::rotate(CArrow* pArrow, float angle)
{
	angle = angle * (1.00f * PI) / 180.0f;
	float x;
	float y;
	//Умножаем вектор на матрицу поворота вокруг орты Z
	//x` = x * cos(angle) - y * sin(angle);
	//y` = x * sin(angle) + y * cos(angle);
	// Поворот начала вектора
	x = pArrow->xPos * cos(angle) - pArrow->yPos * sin(angle);
	y = pArrow->xPos * sin(angle) + pArrow->yPos * cos(angle);
	pArrow->xPos = x;
	pArrow->yPos = y;
	// Поворот конца вектора
	x = pArrow->Vx * cos(angle) - pArrow->Vy * sin(angle);
	y = pArrow->Vx * sin(angle) + pArrow->Vy * cos(angle);
	pArrow->Vx = x;
	pArrow->Vy = y;

}

void  CMatrix::sum(CArrow* pArrow, float x, float y)
{
	pArrow->xPos += x;
	pArrow->yPos += y;
	pArrow->Vx += x;
	pArrow->Vy += y;
}
