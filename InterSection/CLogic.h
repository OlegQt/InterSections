#pragma once
#include "CQuadTree.h"

class CLogic
{
public:
	CLogic();
	~CLogic();
	CQuadTree* GetQTree();
private:
	CQuadTree* qTree;

};

