#include "CLogic.h"
#include "CQuadTree.h"

CLogic::CLogic()
{
	this->qTree = new CQuadTree();
	this->TemporaryEl = new element{};
	TemporaryEl->diameter = 2.0f;
}
CLogic::~CLogic()
{
	delete this->qTree;
	delete TemporaryEl;
	TemporaryEl = nullptr;
}


CQuadTree* CLogic::GetQTree()
{
	return this->qTree;
}

element* CLogic::GetTemporaryElement()
{
	return this->TemporaryEl;
}

void CLogic::PutInQTree()
{
	element* pE = new element;
	pE = this->TemporaryEl;
	pE->diameter = 10;
	pE->pos = this->mousCurrentPos;
	this->qTree->InsertElement(pE);
	this->TemporaryEl = new element{0.0f,0.0f,0.0f,0.0f,0.0f,5.0f};
}

void CLogic::SetMousPos(float x,float y,int pos)
{
	if (pos == RBUTTONDOWN)
	{
		this->mouseDownPos = { x,y };
		this->MouseBehaveour = RBUTTONDOWN;
		this->TemporaryEl->diameter = 2;
	}
	else if(pos == CURRENTMOUSEPOS)
	{
		this->mousCurrentPos = { x,y };
		this->TemporaryEl->pos = this->mousCurrentPos;
	}
	else if (pos == RBUTTONUP)
	{
		this->mousCurrentPos = { x,y };
		this->MouseBehaveour = RBUTTONUP;
		this->TemporaryEl->pos = this->mouseDownPos;
		this->PutInQTree();
	}

}
