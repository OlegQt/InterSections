#pragma once
#include <windows.h>

// Класс CArrow представляет собой структуру,
// в которой удобно хранить вектор в двумерном пространстве
// Координаты первого отрезка xPos, yPos
// Координаты второго отрезка Vx,Vy
// Так же можно использовать данный класс для преставления других объектов,
// таких как физические модели, обладающие массой, скоростью и положением в пространстве

class CArrow
{
public:
	CArrow(float,float,float);
	CArrow(float, float, float,float);
	CArrow();
	~CArrow();

	float xPos, yPos,Diameter;
	float Vx, Vy, Vz;
	float Mass;

	typedef struct vector
	{
		float pos;
	};
	vector pos = { 0.1f };

};


