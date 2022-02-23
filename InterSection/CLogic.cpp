#include "CLogic.h"
#include "CQuadTree.h"

CLogic::CLogic()
{
	this->qTree = new CQuadTree();
	this->TemporaryEl = new element{};
	//TemporaryEl->diameter = 4.0f;
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

std::vector<rLine>* CLogic::getGeometryPointer()
{
	return &this->VectorArray;
}
void CLogic::clearAdditionalGeometry()
{
	this->VectorArray.clear();
}

void CLogic::PutInQTree()
{
	element* pE = new element;
	pE = this->TemporaryEl;

	this->qTree->InsertElement(pE);
	this->TemporaryEl = new element{ 0.0f,0.0f,0.0f,0.0f,0.0f,1.0f };
}
void CLogic::RbuttonDown(float x, float y)
{
	this->MouseStatus = RBUTTONDOWN;
	this->mouseDownPos = { x,y };
	this->TemporaryEl->pos = mouseDownPos;
	this->TemporaryEl->radius = 20;
	this->TemporaryEl->mass = 6;
}
void CLogic::RMouseMoove(float x, float y)
{
	if (this->MouseStatus == RBUTTONDOWN)
	{

		// Draw line between mousedown position and current mouseposition
		mousCurrentPos = { x,y };
		rLine line = { TemporaryEl->pos,this->mousCurrentPos ,NEWELEMENT };
		this->VectorArray.push_back(line);

		// Draw reverce line
		rvector V = CMatrix::SumVectors(TemporaryEl->pos, mousCurrentPos, -1);
		V.x = -V.x;
		V.y = -V.y;
		V = CMatrix::SumVectors(TemporaryEl->pos, V, 1);

		line = { TemporaryEl->pos,V,BOND };
		this->VectorArray.push_back(line);
	}

}
void CLogic::RbuttonUp(float x, float y)
{
	if (this->MouseStatus == RBUTTONDOWN)
	{

		this->mousCurrentPos = { x,y };

		// Calculate particle speed accordin to difference
		// between mouseDown pos and mouseUp position
		rvector V = CMatrix::SumVectors(TemporaryEl->pos, mousCurrentPos, -1);

		V.x = -V.x / 600.0f;
		V.y = -V.y / 600.0f;

		TemporaryEl->V = V;
		this->PutInQTree();
		MouseStatus = CURRENTMOUSEPOS;
		this->clearAdditionalGeometry();
	}
}


void CLogic::CheckCollusions(element* Aparticle, element* Bparticle)
{
	rvector AB{ 0,0 };
	rvector V{ 0,0 };
	rvector Offset{ 0,0 };
	float betta = 0;
	rLine line;

	AB = CMatrix::SumVectors(Aparticle->pos, Bparticle->pos, -1);
	if (CMatrix::GetVectorLength(AB) <= Aparticle->radius + Bparticle->radius + 0.0f)
	{

		betta = CMatrix::GetAngle(AB, { 1.0f,0.0f }); // Calculate angle between radius vector from A-point ro B-point and OX axis
		rvector Va = Aparticle->V; // Take the real velocity  vector in real OXY
		rvector Vb = Bparticle->V; // Take the real velocity vector in real OXY
		Va = CMatrix::Rotate(Va, betta); // Rotate velocity (x component is the proection the real velocity on the AB-vector)
		Vb = CMatrix::Rotate(Vb, betta); // Rotate velocity (x component is the proection the real velocity on the AB-vector)

		float DeltaMass = Bparticle->mass - Aparticle->mass;
		float SummMass = Bparticle->mass + Aparticle->mass;
		float Vax = (2 * Bparticle->mass * Vb.x - Va.x * DeltaMass) / SummMass; // Calculate Vx for A-particle after collusion
		float Vbx = (2 * Aparticle->mass * Va.x + Vb.x * DeltaMass) / SummMass; // Calculate Vx for B-particle after collusion
		Va.x = Vax;
		Vb.x = Vbx;

		Aparticle->V = CMatrix::Rotate(Va, -betta); // Rotate back to take new velocities after collusion
		Bparticle->V = CMatrix::Rotate(Vb, -betta); // Rotate back to take new velocities after collusion

		// If both particles stuck in each others
		// Calculate the overlay distance and moove each particl with its new speed on the half of this distancce

		float delta = CMatrix::GetVectorLength(AB) - (Aparticle->radius + Bparticle->radius); // Calculate the overlay
		float time = (1.0f * abs(delta)) / CMatrix::GetVectorLength(Aparticle->V); //Calculate time to moove from stuck
		

		Aparticle->pos.x = Aparticle->pos.x + (time * Aparticle->V.x);
		Aparticle->pos.y = Aparticle->pos.y + (time * Aparticle->V.y);

		time = (1.0f * abs(delta)) / CMatrix::GetVectorLength(Bparticle->V); //Calculate time to moove from stuck

		Bparticle->pos.x = Bparticle->pos.x + (time * Bparticle->V.x);
		Bparticle->pos.y = Bparticle->pos.y + (time * Bparticle->V.y);

		AB = CMatrix::SumVectors(Aparticle->pos, Bparticle->pos, -1);
		delta = CMatrix::GetVectorLength(AB) - (Aparticle->radius + Bparticle->radius); // Calculate the overlay
		if (delta<=0)
		{
			Aparticle->V = { 0.0f,0.0f };
			Bparticle->V = { 0.0f,0.0f };
		}
		//Aparticle->V = { 0.0f,0.0f };
		//Bparticle->V = { 0.0f,0.0f };
	}




	line = { Bparticle->pos,Aparticle->pos ,BOND };
	this->VectorArray.push_back(line);


	V = Aparticle->V;
	Offset = Aparticle->pos;
	V = CMatrix::ScaleVector(V, 2.1f);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset ,V,VELOCITY };
	this->VectorArray.push_back(line);

	V = Bparticle->V;
	Offset = Bparticle->pos;
	V = CMatrix::ScaleVector(V, 2.1f);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset ,V,VELOCITY };
	this->VectorArray.push_back(line);

}
void CLogic::MooveElement(element* pE)
{

	pE->pos.x += pE->V.x;
	pE->pos.y += pE->V.y;
}