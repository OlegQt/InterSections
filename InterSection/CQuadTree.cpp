#include "CQuadTree.h"

CQuadTree::CQuadTree()
{
	this->Load = 0;
	this->border = RECT();
	for (int i = 0; i < 4; i++)
	{
		this->leaf[i] = nullptr;
		this->ar[i] = nullptr;
	}

}
CQuadTree::~CQuadTree()
{
	if (IsSubDevided())
	{
		this->leaf[0]->~CQuadTree();
		this->leaf[1]->~CQuadTree();
		this->leaf[2]->~CQuadTree();
		this->leaf[3]->~CQuadTree();
	}
	else
	{
		for (int i = 0; i < this->Load; i++)
		{
			delete(this->ar[0]);
			this->ar[0] = nullptr;
		}
	}
}

bool CQuadTree::IsSubDevided()
{
	if (this->leaf[0] != nullptr)
	{
		return true;
	}
	else return false;
}
bool CQuadTree::IsInside(rvector rV)
{
	if (rV.x > border.left && rV.x < border.right)
	{
		if (rV.y > border.top && rV.y < border.bottom)
		{
			return true;
		}
		else return false;
	}
	else return false;

}

bool CQuadTree::PutElementInLeaf(element * pE, bool check)
{
	if (!this->IsInside(pE->pos)) return false;
	else
	{
		if (check)
		{	// Check all surrounding element
			// Insert only if the current location is not occupied 
			for (int i = 0; i < this->storage.size(); i++)
			{
				rvector AB = CMatrix::SumVectors(pE->pos, storage.at(i)->pos,-1);
				if (CMatrix::GetVectorLength(AB) < pE->diameter * 1) return false;
			}
			storage.push_back(pE); // Push element to array end	
			return true;
		}
		else
		{

			this->storage.push_back(pE); // Push element to array end	
			return true;
		}
	}
}
bool CQuadTree::InsertElement(element* pE)
{
	if (this->IsSubDevided())
	{
		if (this->leaf[0]->InsertElement(pE)) return true;
		if (this->leaf[1]->InsertElement(pE)) return true;
		if (this->leaf[2]->InsertElement(pE)) return true;
		if (this->leaf[3]->InsertElement(pE))	return true;
	}
	else
	{
		if (this->storage.size() < MAXLOAD) return this->PutElementInLeaf(pE, true);
		else
		{
			if (!this->Subdivide())
			{
				return false; // If subdivide failed
			}
			else
			{
				this->TransferAlltoLeafs(true); // Transfer all elements into children leafs
				return this->InsertElement(pE); // Insert new element
			}
		}
		return false;
	}
}
bool CQuadTree::RelocateElement(element * pE)
{
	if (this->IsSubDevided())
	{
		if (this->leaf[0]->RelocateElement(pE)) return true;
		if (this->leaf[1]->RelocateElement(pE)) return true;
		if (this->leaf[2]->RelocateElement(pE)) return true;
		if (this->leaf[3]->RelocateElement(pE)) return true;
	}
	else
	{
		if (this->storage.size() < MAXLOAD) return this->PutElementInLeaf(pE, false);
		else
		{
			if (!this->Subdivide())
			{
				// Even if it coudnt be subdivide in case if mesh is too small (see subdivede function)
				// But we couldnt lose the element, so we need to encrease the storage size up to EXTRAMAXLOAD
				if(this->storage.size()<EXTRA_MAXLOAD)	return this->PutElementInLeaf(pE, false);
				else
				{
					MessageBox(NULL, L"", L"ExtraMaxLOAD limit", NULL);
					return false;
				}
			}
			else 
			{
				this->TransferAlltoLeafs(true);
				return this->RelocateElement(pE);
			}
		}
	}
}
void CQuadTree::RemoveElement(short num)
{
	/*
	if (this->Load > 1)
	{
		//Убираем пустой элемент массива (смещаем элементы влево)
		for (int i = num; i < this->Load - 1; i++)
		{
			this->ar[i] = this->ar[i + 1];
		}
		this->ar[Load - 1] = nullptr; // Delete last element
		this->Load--;
	}
	else if (this->Load == 1)
	{
		this->ar[0] = nullptr;
		this->Load--;
	}
	*/

}
bool CQuadTree::TransferAlltoLeafs(bool n)
{
	while (this->storage.size())
	{
		this->RelocateElement(this->storage.back());
		this->storage.pop_back();
	}
	return true;
}


bool CQuadTree::CheckTreeLeaf(CQuadTree* parent)
{
	// Проверяем все элементы внутри листа дерева
	// В случае, если элемент, выходит за пределы данного листа
	// производим удаление его из массива листа и затем вызываем
	// функцию добавления элемента в главное родительское дерево

	for (int i = 0; i < this->storage.size(); i++)
	{
		element* pE = this->storage.at(i);
		if (!this->IsInside(pE->pos))
		{
			this->storage.erase(storage.begin() + i);
			if (parent->RelocateElement(pE)) return true;
			else
			{
				//MessageBeep(10);
				//parent->RelocateElement(pA);
			}
		}
	}
	return true;
}
bool CQuadTree::MergeLeafs()
{
	//if (!this->IsSubDevided()) return false;
	int LOAD = 0;
	bool flag = true;

	// Chek all children leafes
	// In case all 4 children are not subdivided 
	// and there are no elements inside them
	// Merge them into one leaf 
	for (int i = 0; i < 4; i++)
	{
		LOAD += this->leaf[i]->Load;
		if (this->leaf[i]->IsSubDevided()) flag = false;
	}
	if (LOAD == 0 && flag)
	{
		for (int i = 0; i < 4; i++)
		{
			delete(this->leaf[i]);
			this->leaf[i] = nullptr;
		}
		return true;
	}
	return false;
}
bool CQuadTree::SetBorder(RECT rect)
{
	this->border = rect;
	return false;
}
bool CQuadTree::Subdivide()
{
	RECT halfBorder{ 0,0,0,0 };
	long hW = (border.right - border.left) / 2;
	long hH = (border.bottom - border.top) / 2;
	if (hW < 6) return false;
	else {
		// TOP LEFT
		halfBorder.left = border.left;
		halfBorder.top = border.top;
		halfBorder.right = border.left + hW;
		halfBorder.bottom = border.top + hH;
		this->leaf[0] = new CQuadTree();
		this->leaf[0]->SetBorder(halfBorder);
		// TOP RIGHT
		halfBorder.left = border.left + hW;
		halfBorder.top = border.top;
		halfBorder.right = border.right;
		halfBorder.bottom = border.top + hH;
		this->leaf[1] = new CQuadTree();
		this->leaf[1]->SetBorder(halfBorder);
		// BOTTOM LEFT
		halfBorder.left = border.left;
		halfBorder.top = border.top + hH;
		halfBorder.right = border.left + hW;
		halfBorder.bottom = border.bottom;
		this->leaf[2] = new CQuadTree();
		this->leaf[2]->SetBorder(halfBorder);
		// BOTTOM RIGHT
		halfBorder.left = border.left + hW;
		halfBorder.top = border.top + hH;
		halfBorder.right = border.right;
		halfBorder.bottom = border.bottom;
		this->leaf[3] = new CQuadTree();
		this->leaf[3]->SetBorder(halfBorder);
		return true;
	}
}

RECT CQuadTree::GetBorder()
{
	return this->border;
}
CQuadTree* CQuadTree::GetChild(short n)
{
	return this->leaf[n];
}

int CQuadTree::GetLoad()
{
	return this->Load;
}
CElement* CQuadTree::GetArrow(int num)
{
	return this->ar[num];
}
