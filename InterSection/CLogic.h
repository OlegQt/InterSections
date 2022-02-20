#pragma once
#include "CQuadTree.h"



enum MouseBehaviour
{
	CURRENTMOUSEPOS,
	LBUTTONDOWN,
	LBUTTONUP,
	RBUTTONDOWN,
	RBUTTONUP
};

class CLogic
{
public:
	CLogic();
	~CLogic();
	CQuadTree* GetQTree();
	element* GetTemporaryElement();

	void SetMousPos(float x, float y, int pos);
	void PutInQTree();

public:
	int MouseBehaveour;
private:
	CQuadTree* qTree;
	element* TemporaryEl;
	rvector mouseDownPos;
	rvector mousCurrentPos;


};

