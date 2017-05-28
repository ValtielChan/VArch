#ifndef TREE_H
#define TREE_H

#include "VoxelOctree.h"

struct TreeParameters {

	float trunkWidth;
	float trunkHeight;
	int nbBranches;

	TreeParameters(float _trunkWidth, float _trunkHeight, int _nbBranches) :
		trunkWidth(_trunkWidth), trunkHeight(_trunkHeight), nbBranches(_nbBranches) {}
};

class Tree
{
private:

	VoxelOctree* m_octree;

public:

	Tree(const TreeParameters &params, VoxelOctreeLOD &lod);
	~Tree();

	Mesh* mesh() { return m_octree->mesh(); }
	void select(Camera* camera);
};

#endif // TREE_H