#include "Planet.h"

#include "VoxelOctree.h"
#include "HeightMap.h"
#include "NormalMap.h"
#include "TextureRGB.h"
#include "Object.h"

Planet::Planet(const CubeMap &cubeMap, const ColorTable &colorTable, int octreeDepth, int nbChunk) 
	: m_cubeMap(cubeMap),
	m_colorTable(colorTable),
	m_octreeDepth(octreeDepth),
	m_nbChunk(nbChunk)
{
	m_cubeMap.transformInterval(-0.5f / (float)m_nbChunk, 0.5f / (float)m_nbChunk);

	createPlanetSide(Side::UP);
	createPlanetSide(Side::DOWN);
	createPlanetSide(Side::LEFT);
	createPlanetSide(Side::RIGHT);
	createPlanetSide(Side::FRONT);
	createPlanetSide(Side::BACK);
}

Planet::~Planet()
{
}

void Planet::addMeshesToObject(Object* object)
{
	for (Mesh* mesh : m_meshes)
		object->addComponent(mesh);
}

void Planet::createPlanetSide(Side side)
{
	std::cout << "createPlanetSide(" << side << ")" << std::endl;

	HeightMap* heightMap = m_cubeMap.getSide(side);
	//NormalMap* normalMap = heightMap->generateNormalMap();
	TextureRGB* colorMap = heightMap->generateColorMap(m_colorTable);

	VoxelOctreeLOD lod(m_octreeDepth, 4, 100, true);
	
	float step = 1.f / (float)m_nbChunk;

	int planetMemSize = 0;

	for (int x = 0; x < m_nbChunk; x++) {
		for (int z = 0; z < m_nbChunk; z++) {

			std::cout << "New Chunk" << std::endl;

			int hmStep = heightMap->width() / m_nbChunk;

			//m_chunks.push_back(new VoxelOctree(octreeLOD, glm::vec3(x * m_chunkSize, 0, z * m_chunkSize), m_chunkSize));
			HeightMap* hm = heightMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			TextureRGB* cm = colorMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));
			//NormalMap* nm = normalMap->getPart(glm::vec2(z * hmStep, x * hmStep), glm::vec2((z + 1) * hmStep, (x + 1) * hmStep));

			float xPos = -0.5f + (step / 2.f) + (x * step);
			float zPos = -0.5f + (step / 2.f) + (z * step);

			VoxelOctree octree(lod, glm::vec3(xPos, 0, zPos), 1.f / (float)m_nbChunk);

			octree.buildTerrain(hm, cm, NULL);
			octree.rootUpdateNeighbors();

			octree.resetSelection();
			octree.select();
			octree.buildTriangles();

			planetMemSize += octree.sizeOf();

			Mesh* mesh = octree.mesh();

			switch (side) {
			case UP:
				mesh->translate(glm::vec3(0, 1, 0));
				break;
			case DOWN:
				mesh->rotate(glm::vec3(180, 180, 0));
				mesh->translate(glm::vec3(0, -1, 0));
				break;
			case LEFT:
				mesh->rotate(glm::vec3(90, 0, 90)); //-90y
				mesh->translate(glm::vec3(0, 0, -1));
				break;
			case RIGHT:
				mesh->rotate(glm::vec3(-90, 0, 90));
				mesh->translate(glm::vec3(0, 0, 1));
				break;
			case BACK:
				mesh->rotate(glm::vec3(0, 180, -90));
				mesh->translate(glm::vec3(-1, 0, 0));
				break;
			case FRONT:
				mesh->rotate(glm::vec3(0, 0, 90));
				mesh->translate(glm::vec3(1, 0, 0));
				break;
			}

			//Sphere projection
			mesh->normalizePositions(glm::vec3(2));
			mesh->projectToSphere(side);

			m_meshes.push_back(mesh);
		}
	}

	std::cout << planetMemSize << std::endl;
	std::cout << "Size of int : " << sizeof(int) << std::endl;
}
