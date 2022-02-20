#include "CLogic.h"
#include "CQuadTree.h"

CLogic::CLogic()
{
	this->qTree = new CQuadTree();
}

CLogic::~CLogic()
{
	delete this->qTree;
}
CQuadTree* CLogic::GetQTree()
{
	return this->qTree;
}