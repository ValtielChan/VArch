#pragma once

#include "VoxelGrid.h"

class VoxelOctree
{
public:

	VoxelOctree();
	~VoxelOctree();
	void setNeighbor(Side side, VoxelOctree * neighbor);

	bool haveChilds();

private:

	VoxelOctree* m_cells[8];
	VoxelOctree* m_neighbors[6];

};

VoxelOctree::VoxelOctree()
{
	for (int i = 0; i < 8; i++)
		m_cells[i] = nullptr;
	
	for (int i = 0; i < 6; i++)
		m_neighbors[i] = nullptr;
}

VoxelOctree::~VoxelOctree()
{

}

void VoxelOctree::setNeighbor(Side side, VoxelOctree* neighbor)
{
	m_neighbors[side] = neighbor;
}

bool VoxelOctree::haveChilds() {

	return m_cells[0] ? true : false;
}