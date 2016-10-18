#pragma once

#include "VoxelOctree.h"

#define PROCESS_PER_FRAME 1
#define REMAIN_PROCESS_FRAME 10
#define UPDATE_NEAREST_FRAME 60

class Terrain
{

public:

	Terrain(HeightMap* heightMap, Camera* camera, int nbChunk = 1, float chunkSize = 1.f);
	~Terrain();

	TextureRGB* colorMap() { return m_colorMap; }

	void addMeshesToObject(Object* object);
	void findNearest();
	void process();

private:

	Camera* ref_camera;

	std::vector<VoxelOctree*> m_chunks;
	HeightMap* m_heightMap;
	TextureRGB* m_colorMap;
	NormalMap* m_normalMap;

	int m_nbChunk;
	float m_chunkSize; // Correspond to voxelSize of Octrees

	VoxelOctree* m_curChunk;
	std::vector<int> m_aroundIndexes;
	std::vector<int> m_remainIndexes;

	// Counters
	int m_curRemain;

	// Timers
	bool m_alt;
	int m_nearestFrameCounter;
	int m_remainFrameCounter;
	int m_findNearestCounter;
};

Terrain::Terrain(HeightMap* heightMap, Camera* camera, int nbChunk, float chunkSize)
	: m_nbChunk(nbChunk), 
	m_chunkSize(chunkSize),
	m_alt(true),
	m_nearestFrameCounter(0),
	m_remainFrameCounter(0),
	m_findNearestCounter(0),
	m_curRemain(0),
	m_heightMap(heightMap),
	ref_camera(camera)
{
	ColorTable colorTable = ColorTable::Nature(128);
	m_colorMap = heightMap->generateColorMap(colorTable);
	m_normalMap = heightMap->generateNormalMap();

	for (int x = 0; x < nbChunk; x++) {
		for (int z = 0; z < nbChunk; z++) {

			int hmStep = m_heightMap->width() / m_nbChunk;

			m_chunks.push_back(new VoxelOctree(glm::vec3(x * m_chunkSize, 0, z * m_chunkSize), m_chunkSize));
			HeightMap* hm = m_heightMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			TextureRGB* cm = m_colorMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			NormalMap* nm = m_normalMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));

			m_chunks.back()->buildTerrain(hm, cm, nm);
			
			m_chunks.back()->rootUpdateNeighbors();
		}
	}

	findNearest();
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

	m_aroundIndexes.clear();

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {

			if (xIndex + i >= 0 && xIndex + i < m_nbChunk && zIndex + j >= 0 && zIndex + j < m_nbChunk
				&& (i != 0 || j != 0)) {

				m_aroundIndexes.push_back(((int)xIndex + i) * m_nbChunk + ((int)zIndex + j));
			}
		}
	}

	m_remainIndexes.clear();

	for (int x = 0; x < m_nbChunk; x++) {
		for (int z = 0; z < m_nbChunk; z++) {

			if ((x < xIndex - 1 || x > xIndex + 1) || (z < zIndex - 1 || z > zIndex + 1)) {
				m_remainIndexes.push_back(x * m_nbChunk + z);
			}
		}
	}
}

void Terrain::process()
{
	int i = 0;

	// Find nearest update
	if (m_findNearestCounter >= UPDATE_NEAREST_FRAME) {
		findNearest();
		m_findNearestCounter = 0;
	}
	
	m_findNearestCounter++;

	// Alternative process between nearest & current
	m_alt = !m_alt;

	if (m_alt) {

		m_curChunk->resetSelection();
		m_curChunk->select(ref_camera);
		m_curChunk->buildTriangles();

	}
	else {

		while (i < PROCESS_PER_FRAME) {

			m_chunks[m_aroundIndexes[m_nearestFrameCounter]]->resetSelection();
			m_chunks[m_aroundIndexes[m_nearestFrameCounter]]->select(ref_camera);
			m_chunks[m_aroundIndexes[m_nearestFrameCounter]]->buildTriangles();

			i++;
			m_nearestFrameCounter++;

			if (m_nearestFrameCounter >= m_aroundIndexes.size())
				m_nearestFrameCounter = 0;
		}
	}

	if (m_remainFrameCounter >= REMAIN_PROCESS_FRAME) {

		m_remainFrameCounter = 0;

		if (m_curRemain >= m_remainIndexes.size())
			m_curRemain = 0;

		m_chunks[m_remainIndexes[m_curRemain]]->resetSelection();
		m_chunks[m_remainIndexes[m_curRemain]]->select(ref_camera);
		m_chunks[m_remainIndexes[m_curRemain]]->buildTriangles();

		m_curRemain++;
	}

	m_remainFrameCounter++;
}

