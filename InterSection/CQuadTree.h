#pragma once
#include "Element.h"
#include "CMatrix.h"
#include <iostream>
#include <vector>

#define MAXLOAD 4
#define EXTRA_MAXLOAD 8

class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree();

	bool IsSubDevided();
	bool IsInside(rvector);

	bool InsertElement(element * pE); // Add entire new element(x,y) if there are no elements on this position
	bool RelocateElement(element * pE); // Its simillar to AddElement, but it`s used for elements wich leaves the leaf location
	bool PutElementInLeaf(element * pE, bool check); //Insert Element in Leaf
	bool TransferAlltoLeafs(bool); // Transfer all elements to children leafs

	bool CheckTreeLeaf(CQuadTree * parent); // Needed to send Parent Qtree as a parametr
	bool SetBorder(RECT);
	bool Subdivide();
	bool MergeLeafs();

	void RemoveElement(short);

	RECT GetBorder(); // Return RECT structure for this Qtree element
	CQuadTree * GetChild(short); // Return pointer to Qtree child (TL,TR,BL,BR)
	int GetLoad(); // Return ammount of particles inside this Qtree node
	element* GetElement(int num); //Return pointer to particle Array
	
private:
	CQuadTree* leaf[4];
	std::vector<element*> storage;
	RECT border;
	//element* ar[4];
	
	unsigned short Load;
};

