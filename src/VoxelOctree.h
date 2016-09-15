#pragma once

#include "VoxelGrid.h"

class VoxelOctree
{
public:
	VoxelOctree();
	~VoxelOctree();

private:

	VoxelOctree* m_cells[8];

};

VoxelOctree::VoxelOctree()
{
	for (int i = 0; i < 8; i++)
		m_cells[i] = nullptr;
	
}

VoxelOctree::~VoxelOctree()
{
}