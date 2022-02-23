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

	std::vector<rLine>* getGeometryPointer();
	void clearAdditionalGeometry();


	void RbuttonDown(float, float);
	void RbuttonUp(float, float);
	void RMouseMoove(float, float);


	void PutInQTree();
	void CheckCollusions(element*, element*);
	void MooveElement(element*);


	int MouseStatus;
public:

private:
	CQuadTree* qTree;
	element* TemporaryEl;
	rvector mouseDownPos;
	rvector mousCurrentPos;


	std::vector <rLine> VectorArray; // Array with additional geometry for rendering
};

