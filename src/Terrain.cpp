#include "Terrain.h"

#include "Camera.h"
#include "HeightMap.h"
#include "TextureRGB.h"
#include "NormalMap.h"
#include "ColorTable.h"

Terrain::Terrain(HeightMap* heightMap, Camera* camera, VoxelOctreeLOD& octreeLOD, int nbChunk, float chunkSize)
	: m_nbChunk(nbChunk), 
	m_chunkSize(chunkSize),
	m_alt(true),
	m_alt2(true),
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

			m_chunks.push_back(new VoxelOctree(octreeLOD, glm::vec3(x * m_chunkSize, 0, z * m_chunkSize), m_chunkSize));
			HeightMap* hm = m_heightMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			TextureRGB* cm = m_colorMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			NormalMap* nm = m_normalMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));

			m_chunks.back()->buildTerrain(hm, cm, NULL);
			
			m_chunks.back()->rootUpdateNeighbors();
		}
	}

	findNearest();
}

Terrain::~Terrain()
{
	for (std::vector<VoxelOctree*>::iterator it = m_chunks.begin(); it != m_chunks.end(); ++it)
		delete (*it);

	delete m_colorMap;
	delete m_normalMap;
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

	m_chunks[m_remainFrameCounter]->resetSelection();
	m_chunks[m_remainFrameCounter]->select(ref_camera);
	m_chunks[m_remainFrameCounter]->buildTriangles();

	m_remainFrameCounter++;

	if (m_remainFrameCounter >= m_chunks.size()) {
		m_remainFrameCounter = 0;
	}

	// Find nearest update
	/*if (m_findNearestCounter >= UPDATE_NEAREST_FRAME) {
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

		m_alt2 = !m_alt2;

		if (m_alt2) {

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
		else {
			m_remainFrameCounter = 0;

			if (m_curRemain >= m_remainIndexes.size())
				m_curRemain = 0;

			m_chunks[m_remainIndexes[m_curRemain]]->resetSelection();
			m_chunks[m_remainIndexes[m_curRemain]]->select(ref_camera);
			m_chunks[m_remainIndexes[m_curRemain]]->buildTriangles();

			m_curRemain++;
		}
	}*/

	/*if (m_remainFrameCounter >= REMAIN_PROCESS_FRAME) {

		m_remainFrameCounter = 0;

		if (m_curRemain >= m_remainIndexes.size())
			m_curRemain = 0;

		m_chunks[m_remainIndexes[m_curRemain]]->resetSelection();
		m_chunks[m_remainIndexes[m_curRemain]]->select(ref_camera);
		m_chunks[m_remainIndexes[m_curRemain]]->buildTriangles();

		m_curRemain++;
	}

	m_remainFrameCounter++;*/
}

int Terrain::sizeOf() const
{
	int colorMapSize = 0;
	int normalMapSize = 0;
	int octreeArraySize = 0;

	if (m_colorMap)
		colorMapSize += m_colorMap->sizeOf();

	if (m_normalMap)
		normalMapSize += m_normalMap->sizeOf();

	for (int i = 0; i < m_chunks.size(); i++)
		octreeArraySize += m_chunks[i]->sizeOf();

	return m_heightMap->sizeOf() + colorMapSize + normalMapSize + octreeArraySize;
}