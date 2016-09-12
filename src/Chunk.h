#pragma once

#include "VoxelGrid.h"
#include "HeightMap.h"

class Chunk
{
public:
	Chunk(int width, int height, int depth);
	Chunk(int width, int height, int depth, HeightMap* heightMap);
	~Chunk();

private:

	int m_width, m_height, m_depth;

	VoxelGrid* m_voxelGrid;
	HeightMap* m_heightMap;

};

Chunk::Chunk(int width, int height, int depth)
	: m_width(width),
	m_height(height),
	m_depth(depth)
{
	m_voxelGrid = new VoxelGrid(width, height, depth);
}

Chunk::Chunk(int width, int height, int depth, HeightMap* heightMap) 
	: Chunk(width, height, depth)
{
	m_heightMap = heightMap;

	m_heightMap->transformInterval(0, m_height);

	for (int x = 0; x < width; x++) {
		for (int z = 0; z < depth; z++) {

			for (int y = 0; y < height; y++) {

				if (y < m_heightMap->get(x, z))
					m_voxelGrid->setVoxel(x, y, z, Voxel(true));
			}
		}
	}
}


Chunk::~Chunk()
{
}