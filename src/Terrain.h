#pragma once

#include "VoxelOctree.h"

#define PROCESS_PER_FRAME 1

class Terrain
{

public:

	Terrain(HeightMap* heightMap, Camera* camera, int nbChunk = 1, float chunkSize = 1.f);
	~Terrain();

	void addMeshesToObject(Object* object);

	void findNearest();

	void process();

private:

	Camera* ref_camera;

	std::vector<VoxelOctree*> m_chunks;
	HeightMap* m_heightMap;

	int m_nbChunk;
	float m_chunkSize; // Correspond to voxelSize of Octrees

	VoxelOctree* m_curChunk;

	int m_frameCounter;
};

Terrain::Terrain(HeightMap* heightMap, Camera* camera, int nbChunk, float chunkSize)
	: m_nbChunk(nbChunk), 
	m_chunkSize(chunkSize),
	m_frameCounter(0),
	m_heightMap(heightMap),
	ref_camera(camera)
{
	for (int x = 0; x < nbChunk; x++) {
		for (int z = 0; z < nbChunk; z++) {

			int hmStep = m_heightMap->width() / m_nbChunk;

			m_chunks.push_back(new VoxelOctree(glm::vec3(x * m_chunkSize, 0, z * m_chunkSize), m_chunkSize));
			HeightMap* hm = m_heightMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			
			m_chunks.back()->buildTerrain(hm);
			
			m_chunks.back()->rootUpdateNeighbors();
		}
	}
}

Terrain::~Terrain()
{

}

void Terrain::addMeshesToObject(Object* object)
{
	for (VoxelOctree* chunk : m_chunks)
		object->addComponent(chunk->mesh());
}

void Terrain::findNearest()
{
	glm::vec3 pos = ref_camera->transform.position();
	//glm::vec3 pos(2.f, 0 ,3.6f);

	float xIndex = pos.x / m_nbChunk;
	float zIndex = pos.z / m_nbChunk;

	xIndex = xIndex - floorf(xIndex) > ceilf(xIndex) - xIndex ? ceilf(xIndex) : floorf(xIndex);
	zIndex = zIndex - floorf(zIndex) > ceilf(zIndex) - zIndex ? ceilf(zIndex) : floorf(zIndex);

	xIndex = std::max(0, std::min((int)xIndex, m_nbChunk - 1));
	zIndex = std::max(0, std::min((int)zIndex, m_nbChunk - 1));

	m_curChunk = m_chunks[(int)xIndex * m_nbChunk + (int)zIndex];

	std::cout << "(" << xIndex << "," << zIndex << ")" << std::endl;
}

void Terrain::process()
{
	int i = 0;

	findNearest();

	m_curChunk->resetSelection();
	m_curChunk->select(ref_camera);
	m_curChunk->buildTriangles();

	/*while (i < PROCESS_PER_FRAME) {

		m_chunks[m_frameCounter]->resetSelection();
		m_chunks[m_frameCounter]->select(ref_camera);
		m_chunks[m_frameCounter]->buildTriangles();

		i++;
		m_frameCounter++;

		if (m_frameCounter >= m_chunks.size())
			m_frameCounter = 0;
	}*/
}

