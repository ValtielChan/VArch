#include "Planet.h"

#include "VoxelOctree.h"
#include "HeightMap.h"
#include "Object.h"

Planet::Planet(const CubeMap &cubeMap, const ColorTable &colorTable) 
	: m_cubeMap(cubeMap),
	m_colorTable(colorTable)
{
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
	NormalMap* normalMap = heightMap->generateNormalMap();
	TextureRGB* colorMap = heightMap->generateColorMap(m_colorTable);

	VoxelOctree octree(glm::vec3(0), 1);

	octree.buildTerrain(heightMap, colorMap, NULL);
	octree.rootUpdateNeighbors();

	octree.resetSelection();
	octree.select();
	octree.buildTriangles();

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
