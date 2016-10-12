#pragma once

#include <ctime>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "enum.h"
#include "HeightMap.h"
#include "Camera.h"

#define DEPTH 7
#define THRESHOLD 20
#define PERVISSIVE_FRUSTUM false

//#define BENCHMARK

struct Voxel {

	bool value;
	glm::vec2 texCoord; // If color value set into texture
	glm::vec3 color; // Vertex Color

	Voxel(bool e = false, glm::vec3 c = glm::vec3(1), glm::vec2 tc = glm::vec2(0))
		: value(e), color(c), texCoord(tc) {}

};

enum OctreePosition {
	UP_LEFT_BACK,
	UP_RIGHT_BACK,
	UP_LEFT_FRONT,
	UP_RIGHT_FRONT,
	DOWN_LEFT_BACK,
	DOWN_RIGHT_BACK,
	DOWN_LEFT_FRONT,
	DOWN_RIGHT_FRONT,
	NOWHERE
};

class VoxelOctree
{
public:

	/// <summary>
	/// Can be selected
	/// </summary>
	bool exist;

	/// <summary>
	/// Has been selected to be draw or select childs
	/// </summary>
	bool selected;

	/// <summary>
	/// Width parcour bool
	/// </summary>
	bool visited;

	bool deeper;

	/// <summary>
	/// Root Octree constructor
	/// </summary>
	VoxelOctree(glm::vec3 worldCenter = glm::vec3(0), float voxelSize = 1.f);

	/// <summary>
	/// Child Octree construtor
	/// </summary>
	VoxelOctree(VoxelOctree* parent, Mesh* mesh, glm::vec3 worldCenter, float voxelSize, int depth, OctreePosition relPos);

	~VoxelOctree();

	void setNeighbor(Side side, VoxelOctree * neighbor);

	VoxelOctree* parent() { return m_parent; }
	VoxelOctree* getCell(OctreePosition position);
	std::vector<VoxelOctree*> getCells();
	int depth() { return m_depth; }
	Mesh* mesh() { return m_mesh; }
	OctreePosition relativePosition() { return m_relativePosition; }
	glm::vec3 rootWorldPosition();

	/// <summary>
	/// Does the node have childs
	/// </summary>
	bool haveChilds();

	/// <summary>
	/// Is the neighbor selected
	/// </summary>
	bool checkNeighbor(Side side);

	bool isInFrustum(Camera* camera, int subLevel);

	/// <summary>
	/// Create 8 childs
	/// </summary>
	void subdivide();

	/// <summary>
	/// build Octree according to user defined datas
	/// </summary>
	void build();
	 
	/// <summary>
	/// build with heightmap
	/// </summary>
	void buildTerrain(HeightMap* hm, TextureRGB* colorMap = nullptr, NormalMap* normalMap = nullptr);

	/// <summary>
	/// select the cells according to certains conditions
	/// </summary>
	void select(Camera* camera);

	/// <summary>
	/// Recursively set selected attribute at false
	/// </summary>
	void resetSelection();

	/// <summary>
	/// Add triangles to geometry
	/// </summary>
	void buildTriangles();

	/// <summary>
	/// Add triangles to geometry
	/// </summary>
	void updateNeighbors();

	/// <summary>
	/// Propagate neighbor update to childs
	/// </summary>
	void propagateNeighbors(Side side);

	/// <summary>
	/// Recursively update neighborhood, must be called after build()
	/// </summary>
	void rootUpdateNeighbors();

	bool debugIsPath(std::vector<OctreePosition> path);
	void countExistingVoxel(int &count);
	int sizeOf();

protected:

	VoxelOctree* m_parent;
	VoxelOctree* m_cells[8];
	VoxelOctree* m_neighbors[6];

	std::vector<int> m_vertices;

	Mesh * m_mesh;
	std::vector<Vertex>* ref_vertices; // fast access
	std::vector<GLuint>* ref_indices; // fast access

	OctreePosition m_relativePosition;
	glm::vec3 m_worldCenter;
	float m_voxelSize;
	int m_depth;

	void addVertices();
};

VoxelOctree::VoxelOctree(glm::vec3 worldCenter, float voxelSize)
	: m_voxelSize(voxelSize), 
	m_worldCenter(worldCenter),
	m_depth(0)
{
	m_parent = nullptr;

	selected = false;
	visited = false;
	exist = false;
	deeper = false;

	m_mesh = new Mesh();
	ref_vertices = m_mesh->getVertices();
	ref_indices = m_mesh->getIndices();

	for (int i = 0; i < 8; i++)
		m_cells[i] = nullptr;
	
	for (int i = 0; i < 6; i++)
		m_neighbors[i] = nullptr;

	m_relativePosition = NOWHERE;

	addVertices();

}

VoxelOctree::VoxelOctree(VoxelOctree* parent, Mesh* mesh, glm::vec3 worldCenter, float voxelSize, int depth, OctreePosition relPos)

{
	m_voxelSize = voxelSize;
	m_worldCenter = worldCenter;

	m_parent = parent;

	selected = false;
	visited = false;
	exist = false;
	deeper = false;

	m_mesh = mesh;
	ref_vertices = mesh->getVertices();
	ref_indices = mesh->getIndices();

	for (int i = 0; i < 8; i++)
		m_cells[i] = nullptr;

	for (int i = 0; i < 6; i++)
		m_neighbors[i] = nullptr;

	m_depth = depth;
	m_relativePosition = relPos;

	addVertices();
}

VoxelOctree::~VoxelOctree()
{

}

VoxelOctree* VoxelOctree::getCell(OctreePosition position)
{
	// TODO : Exception if null
	return m_cells[position];
}

inline std::vector<VoxelOctree*> VoxelOctree::getCells()
{
	std::vector<VoxelOctree*> cells;

	if (haveChilds()) {
		for (int i = 0; i < 8; i++)
			cells.push_back(m_cells[i]);
	}

	return cells;
}

glm::vec3 VoxelOctree::rootWorldPosition()
{
	glm::vec3 pos;

	if (m_parent)
		pos = m_parent->rootWorldPosition();
	else
		pos = m_worldCenter;

	return pos;
}

void VoxelOctree::setNeighbor(Side side, VoxelOctree* neighbor)
{
	m_neighbors[side] = neighbor;
}

bool VoxelOctree::haveChilds() {

	return m_cells[0] ? true : false;
}

inline bool VoxelOctree::checkNeighbor(Side side)
{
	return m_neighbors[side] && m_neighbors[side]->selected && !m_neighbors[side]->deeper;
}

inline bool VoxelOctree::isInFrustum(Camera * camera, int subLevel)
{
	float halfSize = m_voxelSize / 2.f;
	float subSize = m_voxelSize / (float)subLevel;

	bool isInFrustum = false;
	
	for (int i = 0; i < subLevel + 1; i++) {
		for (int j = 0; j < subLevel + 1; j++) {

			glm::vec3 down(m_worldCenter.x - halfSize + i * subSize, m_worldCenter.y - halfSize, m_worldCenter.z - halfSize + j * subSize);
			glm::vec3 up(m_worldCenter.x - halfSize + i * subSize, m_worldCenter.y + halfSize, m_worldCenter.z - halfSize + j * subSize);
			glm::vec3 left(m_worldCenter.x - halfSize, m_worldCenter.y - halfSize + i * subSize, m_worldCenter.z - halfSize + j * subSize);
			glm::vec3 right(m_worldCenter.x + halfSize, m_worldCenter.y - halfSize + i * subSize, m_worldCenter.z - halfSize + j * subSize);
			glm::vec3 back(m_worldCenter.x - halfSize + i * subSize, m_worldCenter.y - halfSize + j * subSize, m_worldCenter.z - halfSize);
			glm::vec3 front(m_worldCenter.x - halfSize + i * subSize, m_worldCenter.y - halfSize + j * subSize, m_worldCenter.z + halfSize);

			if (camera->isInFrustum(down, PERVISSIVE_FRUSTUM)) {
				isInFrustum = true; 
				goto abort;
			}
			else if (camera->isInFrustum(up, PERVISSIVE_FRUSTUM)) {
				isInFrustum = true; 
				goto abort;
			}
			else if (camera->isInFrustum(left, PERVISSIVE_FRUSTUM)) {
				isInFrustum = true; 
				goto abort;
			}
			else if (camera->isInFrustum(right, PERVISSIVE_FRUSTUM)) {
				isInFrustum = true; 
				goto abort;
			}
			else if (camera->isInFrustum(back, PERVISSIVE_FRUSTUM)) {
				isInFrustum = true; 
				goto abort;
			}
			else if (camera->isInFrustum(front, PERVISSIVE_FRUSTUM)) {
				isInFrustum = true; 
				goto abort;
			}
		}
	}

abort:

	return isInFrustum;
}

void VoxelOctree::subdivide() {

	float quartSize = m_voxelSize / 4.f;
	float halfSize = m_voxelSize / 2.f;

	m_cells[UP_LEFT_BACK] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(-quartSize, quartSize, -quartSize), halfSize, m_depth + 1, UP_LEFT_BACK);
	m_cells[UP_RIGHT_BACK] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(-quartSize, quartSize, quartSize), halfSize, m_depth + 1, UP_RIGHT_BACK);
	m_cells[UP_LEFT_FRONT] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(quartSize, quartSize, -quartSize), halfSize, m_depth + 1, UP_LEFT_FRONT);
	m_cells[UP_RIGHT_FRONT] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(quartSize, quartSize, quartSize), halfSize, m_depth + 1, UP_RIGHT_FRONT);
	m_cells[DOWN_LEFT_BACK] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(-quartSize, -quartSize, -quartSize), halfSize, m_depth + 1, DOWN_LEFT_BACK);
	m_cells[DOWN_RIGHT_BACK] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(-quartSize, -quartSize, quartSize), halfSize, m_depth + 1, DOWN_RIGHT_BACK);
	m_cells[DOWN_LEFT_FRONT] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(quartSize, -quartSize, -quartSize), halfSize, m_depth + 1, DOWN_LEFT_FRONT);
	m_cells[DOWN_RIGHT_FRONT] = new VoxelOctree(this, m_mesh, m_worldCenter + glm::vec3(quartSize, -quartSize, quartSize), halfSize, m_depth + 1, DOWN_RIGHT_FRONT);

	// UP_LEFT_BACK NEIGHBORS
	m_cells[UP_LEFT_BACK]->setNeighbor(Side::UP, m_neighbors[Side::UP]);
	m_cells[UP_LEFT_BACK]->setNeighbor(Side::DOWN, m_cells[DOWN_LEFT_BACK]);
	m_cells[UP_LEFT_BACK]->setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]);
	m_cells[UP_LEFT_BACK]->setNeighbor(Side::RIGHT, m_cells[UP_RIGHT_BACK]);
	m_cells[UP_LEFT_BACK]->setNeighbor(Side::FRONT, m_cells[UP_LEFT_FRONT]);
	m_cells[UP_LEFT_BACK]->setNeighbor(Side::BACK, m_neighbors[Side::BACK]);

	// UP_RIGHT_BACK NEIGHBORS
	m_cells[UP_RIGHT_BACK]->setNeighbor(Side::UP, m_neighbors[Side::UP]);
	m_cells[UP_RIGHT_BACK]->setNeighbor(Side::DOWN, m_cells[DOWN_RIGHT_BACK]);
	m_cells[UP_RIGHT_BACK]->setNeighbor(Side::LEFT, m_cells[UP_LEFT_BACK]);
	m_cells[UP_RIGHT_BACK]->setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]);
	m_cells[UP_RIGHT_BACK]->setNeighbor(Side::FRONT, m_cells[UP_RIGHT_FRONT]);
	m_cells[UP_RIGHT_BACK]->setNeighbor(Side::BACK, m_neighbors[Side::BACK]);

	// UP_LEFT_FRONT NEIGHBORS
	m_cells[UP_LEFT_FRONT]->setNeighbor(Side::UP, m_neighbors[Side::UP]);
	m_cells[UP_LEFT_FRONT]->setNeighbor(Side::DOWN, m_cells[DOWN_LEFT_FRONT]);
	m_cells[UP_LEFT_FRONT]->setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]);
	m_cells[UP_LEFT_FRONT]->setNeighbor(Side::RIGHT, m_cells[UP_RIGHT_FRONT]);
	m_cells[UP_LEFT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]);
	m_cells[UP_LEFT_FRONT]->setNeighbor(Side::BACK, m_cells[UP_LEFT_BACK]);

	// UP_RIGHT_FRONT NEIGHBORS
	m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::UP, m_neighbors[Side::UP]);
	m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::DOWN, m_cells[DOWN_RIGHT_FRONT]);
	m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::LEFT, m_cells[UP_LEFT_FRONT]);
	m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]);
	m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]);
	m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::BACK, m_cells[UP_RIGHT_BACK]);

	// DOWN_LEFT_BACK NEIGHBORS
	m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::UP, m_cells[UP_LEFT_BACK]);
	m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]);
	m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]);
	m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::RIGHT, m_cells[DOWN_RIGHT_BACK]);
	m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::FRONT, m_cells[DOWN_LEFT_FRONT]);
	m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::BACK, m_neighbors[Side::BACK]);

	// DOWN_RIGHT_BACK NEIGHBORS
	m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::UP, m_cells[UP_RIGHT_BACK]);
	m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]);
	m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::LEFT, m_cells[DOWN_LEFT_BACK]);
	m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]);
	m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::FRONT, m_cells[DOWN_RIGHT_FRONT]);
	m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::BACK, m_neighbors[Side::BACK]);

	// DOWN_LEFT_FRONT NEIGHBORS
	m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::UP, m_cells[UP_LEFT_FRONT]);
	m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]);
	m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]);
	m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::RIGHT, m_cells[DOWN_RIGHT_FRONT]);
	m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]);
	m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::BACK, m_cells[DOWN_LEFT_BACK]);

	// DOWN_RIGHT_FRONT NEIGHBORS
	m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::UP, m_cells[UP_RIGHT_FRONT]);
	m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]);
	m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::LEFT, m_cells[DOWN_LEFT_FRONT]);
	m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]);
	m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]);
	m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::BACK, m_cells[DOWN_RIGHT_BACK]);


}

inline void VoxelOctree::build()
{

	if (m_relativePosition != OctreePosition::DOWN_RIGHT_FRONT)
		exist = true;

	if (m_depth < DEPTH && exist) {

		subdivide();

		for (int i = 0; i < 8; i++) {
			m_cells[i]->build();
		}
	}
}

void VoxelOctree::buildTerrain(HeightMap * hm, TextureRGB* colorMap, NormalMap* normalMap)
{
	// TODO : Non Square heightmap exception

#ifdef BENCHMARK
	clock_t t = clock();
#endif

	glm::vec3 rootVoxelSize = glm::vec3(m_voxelSize * pow(2, m_depth));
	glm::vec3 rootWorldCenter = rootWorldPosition();

	glm::vec3 nCenter = ((m_worldCenter - rootWorldCenter) + (rootVoxelSize / 2.f)) / rootVoxelSize;
	float nvSize = m_voxelSize / (rootVoxelSize.x);

	glm::vec2 bMin(nCenter.z - nvSize / 2, nCenter.x - nvSize / 2);
	glm::vec2 bMax(nCenter.z + nvSize / 2, nCenter.x + nvSize / 2);
	bMin *= hm->width();
	bMax *= hm->width();

	bool middle = false;
	bool above = false;
	bool below = false;

	for (int x = (int)bMin.x; x < (int)bMax.x; x++) {
		for (int y = (int)bMin.y; y < (int)bMax.y; y++) {

			float height = hm->get(x, y);
			float halfSize = m_voxelSize / 2;

			if (m_worldCenter.y + halfSize >= height && m_worldCenter.y - halfSize <= height) {
				middle = true;
				goto abort;	
			}
			else if (hm->get(x, y) >= m_worldCenter.y + halfSize) {
				below = true;
			}
			else if (hm->get(x, y) <= m_worldCenter.y - halfSize) {
				above = true;
			}
		}
	}

abort:

	if (colorMap) {

		glm::vec3 color = colorMap->get(bMin.x + nvSize / 2, bMin.y + nvSize / 2);
		for (int i : m_vertices) {
			ref_vertices->at(i).color = color;
		}
	}

	if (middle && m_depth < DEPTH) {
		exist = true;
		subdivide();

		for (int i = 0; i < 8; i++)
			m_cells[i]->buildTerrain(hm, colorMap, normalMap);
	}
	else if (below || middle) {
		exist = true;
	}

#ifdef BENCHMARK
	t = clock() - t;

	if (!m_parent) // Only root print the whole bench result
		std::cout << "[BENCH] VoxelOctree::build() : " << ((float)t) << " ms" << std::endl;
#endif
}

inline void VoxelOctree::select(Camera* camera)
{

#ifdef BENCHMARK
	clock_t t = clock();
#endif

	glm::vec3 pov = camera->transform.position();

	if (!m_parent) {  // if root
		ref_indices->clear();

		selected = true;
	}
	
	if (exist) {
		
		// Test distance between center & pov
		float x = m_worldCenter.x - pov.x;
		float y = m_worldCenter.z - pov.z;
		float dist = sqrt(x*x + y*y);

		if (isInFrustum(camera, pow(2, DEPTH - m_depth)) && dist < THRESHOLD / (m_depth + 1)) {
		
		//if(true) {
			if (haveChilds()) {

				deeper = true;

				for (int i = 0; i < 8; i++) {

					if (m_cells[i]->exist)
						m_cells[i]->selected = true;

					m_cells[i]->select(camera);
				}		
			}
		}
	}

#ifdef BENCHMARK
	t = clock() - t;

	if (!m_parent) // Only root print the whole bench result
		std::cout << "[BENCH] VoxelOctree::select() | depth " << DEPTH << " : " << ((float)t) << " ms" << std::endl;
#endif
}

inline void VoxelOctree::resetSelection()
{

#ifdef BENCHMARK
	clock_t t = clock();
#endif

	selected = false;
	visited = false;
	deeper = false;

	if (haveChilds()) {
		for (int i = 0; i < 8; i++) {
			m_cells[i]->resetSelection();
		}
	}

#ifdef BENCHMARK
	t = clock() - t;

	if (!m_parent) // Only root print the whole bench result
		std::cout << "[BENCH] VoxelOctree::resetSelection() | depth " << DEPTH << " : " << ((float)t) << " ms" << std::endl;
#endif
}

inline void VoxelOctree::buildTriangles()
{

#ifdef BENCHMARK
	clock_t t = clock();
#endif

	if (selected) {

		std::vector<OctreePosition> path;
		path.push_back(UP_RIGHT_BACK); path.push_back(DOWN_RIGHT_BACK); path.push_back(DOWN_LEFT_BACK);
		
		/*if (debugIsPath(path))
			goto exception;*/

		if (deeper) {

			for (int i = 0; i < 8; i++)
				m_cells[i]->buildTriangles();
		}
		else {

			if (!checkNeighbor(Side::UP)) {

				ref_indices->push_back(m_vertices[0]); ref_indices->push_back(m_vertices[1]); ref_indices->push_back(m_vertices[2]);
				ref_indices->push_back(m_vertices[3]); ref_indices->push_back(m_vertices[0]); ref_indices->push_back(m_vertices[2]);
			}

			if (!checkNeighbor(Side::DOWN)) {

				ref_indices->push_back(m_vertices[4]); ref_indices->push_back(m_vertices[5]); ref_indices->push_back(m_vertices[6]);
				ref_indices->push_back(m_vertices[7]); ref_indices->push_back(m_vertices[4]); ref_indices->push_back(m_vertices[6]);
			}

			if (!checkNeighbor(Side::LEFT)) {

				ref_indices->push_back(m_vertices[8]); ref_indices->push_back(m_vertices[9]); ref_indices->push_back(m_vertices[10]);
				ref_indices->push_back(m_vertices[11]); ref_indices->push_back(m_vertices[8]); ref_indices->push_back(m_vertices[10]);
			}

			if (!checkNeighbor(Side::RIGHT)) {

				ref_indices->push_back(m_vertices[12]); ref_indices->push_back(m_vertices[13]); ref_indices->push_back(m_vertices[14]);
				ref_indices->push_back(m_vertices[15]); ref_indices->push_back(m_vertices[12]); ref_indices->push_back(m_vertices[14]);
			}

			if (!checkNeighbor(Side::BACK)) {

				ref_indices->push_back(m_vertices[16]); ref_indices->push_back(m_vertices[17]); ref_indices->push_back(m_vertices[18]);
				ref_indices->push_back(m_vertices[19]); ref_indices->push_back(m_vertices[16]); ref_indices->push_back(m_vertices[18]);
			}

			if (!checkNeighbor(Side::FRONT)) {

				ref_indices->push_back(m_vertices[20]); ref_indices->push_back(m_vertices[21]); ref_indices->push_back(m_vertices[22]);
				ref_indices->push_back(m_vertices[23]); ref_indices->push_back(m_vertices[20]); ref_indices->push_back(m_vertices[22]);
			}
		}
	}

	if (!m_parent)
		m_mesh->updateGL();

#ifdef BENCHMARK
	t = clock() - t;

	if (!m_parent) // Only root print the whole bench result
		std::cout << "[BENCH] VoxelOctree::buildTriangles() | depth " << DEPTH << " : " << ((float)t) << " ms" << std::endl;
#endif
}

void VoxelOctree::updateNeighbors()
{

	if (m_neighbors[Side::UP] && m_neighbors[Side::UP]->haveChilds() && m_neighbors[Side::UP]->depth() == m_depth - 1) {

		switch (m_relativePosition) {

			case OctreePosition::UP_LEFT_BACK:
				setNeighbor(Side::UP, m_neighbors[Side::UP]->getCell(DOWN_LEFT_BACK));
				m_neighbors[Side::UP]->setNeighbor(Side::DOWN, this);
				break;
			case OctreePosition::UP_LEFT_FRONT:
				setNeighbor(Side::UP, m_neighbors[Side::UP]->getCell(DOWN_LEFT_FRONT));
				m_neighbors[Side::UP]->setNeighbor(Side::DOWN, this);
				break;
			case OctreePosition::UP_RIGHT_BACK:
				setNeighbor(Side::UP, m_neighbors[Side::UP]->getCell(DOWN_RIGHT_BACK));
				m_neighbors[Side::UP]->setNeighbor(Side::DOWN, this);
				break;
			case OctreePosition::UP_RIGHT_FRONT:
				setNeighbor(Side::UP, m_neighbors[Side::UP]->getCell(DOWN_RIGHT_FRONT));
				m_neighbors[Side::UP]->setNeighbor(Side::DOWN, this);
				break;
		}

		propagateNeighbors(Side::UP);

	}
	else if (m_neighbors[Side::UP] && m_neighbors[Side::UP]->depth() == m_depth) {
		propagateNeighbors(Side::UP);
	}

	if (m_neighbors[Side::DOWN] && m_neighbors[Side::DOWN]->haveChilds() && m_neighbors[Side::DOWN]->depth() == m_depth - 1) {

		switch (m_relativePosition) {

		case OctreePosition::DOWN_LEFT_BACK:
			setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]->getCell(UP_LEFT_BACK));
			m_neighbors[Side::DOWN]->setNeighbor(Side::UP, this);
			break;
		case OctreePosition::DOWN_LEFT_FRONT:
			setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]->getCell(UP_LEFT_FRONT));
			m_neighbors[Side::DOWN]->setNeighbor(Side::UP, this);
			break;
		case OctreePosition::DOWN_RIGHT_BACK:
			setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]->getCell(UP_RIGHT_BACK));
			m_neighbors[Side::DOWN]->setNeighbor(Side::UP, this);
			break;
		case OctreePosition::DOWN_RIGHT_FRONT:
			setNeighbor(Side::DOWN, m_neighbors[Side::DOWN]->getCell(UP_RIGHT_FRONT));
			m_neighbors[Side::DOWN]->setNeighbor(Side::UP, this);
			break;
		}

		propagateNeighbors(Side::DOWN);

	}
	else if (m_neighbors[Side::DOWN] && m_neighbors[Side::DOWN]->depth() == m_depth) {
		propagateNeighbors(Side::DOWN);
	}

	if (m_neighbors[Side::LEFT] && m_neighbors[Side::LEFT]->haveChilds() && m_neighbors[Side::LEFT]->depth() == m_depth - 1) {

		switch (m_relativePosition) {

		case OctreePosition::DOWN_LEFT_BACK:
			setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]->getCell(DOWN_RIGHT_BACK));
			m_neighbors[Side::LEFT]->setNeighbor(Side::RIGHT, this);
			break;
		case OctreePosition::DOWN_LEFT_FRONT:
			setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]->getCell(DOWN_RIGHT_FRONT));
			m_neighbors[Side::LEFT]->setNeighbor(Side::RIGHT, this);
			break;
		case OctreePosition::UP_LEFT_BACK:
			setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]->getCell(UP_RIGHT_BACK));
			m_neighbors[Side::LEFT]->setNeighbor(Side::RIGHT, this);
			break;
		case OctreePosition::UP_LEFT_FRONT:
			setNeighbor(Side::LEFT, m_neighbors[Side::LEFT]->getCell(UP_RIGHT_FRONT));
			m_neighbors[Side::LEFT]->setNeighbor(Side::RIGHT, this);
			break;
		}

		propagateNeighbors(Side::LEFT);

	}
	else if (m_neighbors[Side::LEFT] && m_neighbors[Side::LEFT]->depth() == m_depth) {
		propagateNeighbors(Side::LEFT);
	}

	if (m_neighbors[Side::RIGHT] && m_neighbors[Side::RIGHT]->haveChilds() && m_neighbors[Side::RIGHT]->depth() == m_depth - 1) {

		switch (m_relativePosition) {

		case OctreePosition::DOWN_RIGHT_BACK:
			setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]->getCell(DOWN_LEFT_BACK));
			m_neighbors[Side::RIGHT]->setNeighbor(Side::LEFT, this);
			break;
		case OctreePosition::DOWN_RIGHT_FRONT:
			setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]->getCell(DOWN_LEFT_FRONT));
			m_neighbors[Side::RIGHT]->setNeighbor(Side::LEFT, this);
			break;
		case OctreePosition::UP_RIGHT_BACK:
			setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]->getCell(UP_LEFT_BACK));
			m_neighbors[Side::RIGHT]->setNeighbor(Side::LEFT, this);
			break;
		case OctreePosition::UP_RIGHT_FRONT:
			setNeighbor(Side::RIGHT, m_neighbors[Side::RIGHT]->getCell(UP_LEFT_FRONT));
			m_neighbors[Side::RIGHT]->setNeighbor(Side::LEFT, this);
			break;
		}

		propagateNeighbors(Side::RIGHT);

	}
	else if (m_neighbors[Side::RIGHT] && m_neighbors[Side::RIGHT]->depth() == m_depth) {
		propagateNeighbors(Side::RIGHT);
	}

	if (m_neighbors[Side::FRONT] && m_neighbors[Side::FRONT]->haveChilds() && m_neighbors[Side::FRONT]->depth() == m_depth - 1) {

		switch (m_relativePosition) {

		case OctreePosition::DOWN_LEFT_FRONT:
			setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]->getCell(DOWN_LEFT_BACK));
			m_neighbors[Side::FRONT]->setNeighbor(Side::BACK, this);
			break;
		case OctreePosition::DOWN_RIGHT_FRONT:
			setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]->getCell(DOWN_RIGHT_BACK));
			m_neighbors[Side::FRONT]->setNeighbor(Side::BACK, this);
			break;
		case OctreePosition::UP_LEFT_FRONT:
			setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]->getCell(UP_LEFT_BACK));
			m_neighbors[Side::FRONT]->setNeighbor(Side::BACK, this);
			break;
		case OctreePosition::UP_RIGHT_FRONT:
			setNeighbor(Side::FRONT, m_neighbors[Side::FRONT]->getCell(UP_RIGHT_BACK));
			m_neighbors[Side::FRONT]->setNeighbor(Side::BACK, this);
			break;
		}

		propagateNeighbors(Side::FRONT);

	}
	else if (m_neighbors[Side::FRONT] && m_neighbors[Side::FRONT]->depth() == m_depth) {
		propagateNeighbors(Side::FRONT);
	}

	if (m_neighbors[Side::BACK] && m_neighbors[Side::BACK]->haveChilds() && m_neighbors[Side::BACK]->depth() == m_depth - 1) {

		switch (m_relativePosition) {

		case OctreePosition::DOWN_LEFT_BACK:
			setNeighbor(Side::BACK, m_neighbors[Side::BACK]->getCell(DOWN_LEFT_FRONT));
			m_neighbors[Side::BACK]->setNeighbor(Side::FRONT, this);
			break;
		case OctreePosition::DOWN_RIGHT_BACK:
			setNeighbor(Side::BACK, m_neighbors[Side::BACK]->getCell(DOWN_RIGHT_FRONT));
			m_neighbors[Side::BACK]->setNeighbor(Side::FRONT, this);
			break;
		case OctreePosition::UP_LEFT_BACK:
			setNeighbor(Side::BACK, m_neighbors[Side::BACK]->getCell(UP_LEFT_FRONT));
			m_neighbors[Side::BACK]->setNeighbor(Side::FRONT, this);
			break;
		case OctreePosition::UP_RIGHT_BACK:
			setNeighbor(Side::BACK, m_neighbors[Side::BACK]->getCell(UP_RIGHT_FRONT));
			m_neighbors[Side::BACK]->setNeighbor(Side::FRONT, this);
			break;
		}

		propagateNeighbors(Side::BACK);

	}
	else if (m_neighbors[Side::BACK] && m_neighbors[Side::BACK]->depth() == m_depth) {
		propagateNeighbors(Side::BACK);
	}
}

inline void VoxelOctree::propagateNeighbors(Side side)
{
	if (haveChilds()) {

		switch (side) {
		case UP:
			m_cells[UP_LEFT_FRONT]->setNeighbor(Side::UP, m_neighbors[UP]);
			m_cells[UP_LEFT_BACK]->setNeighbor(Side::UP, m_neighbors[UP]);
			m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::UP, m_neighbors[UP]);
			m_cells[UP_RIGHT_BACK]->setNeighbor(Side::UP, m_neighbors[UP]);

			m_cells[UP_LEFT_FRONT]->propagateNeighbors(Side::UP);
			m_cells[UP_LEFT_BACK]->propagateNeighbors(Side::UP);
			m_cells[UP_RIGHT_FRONT]->propagateNeighbors(Side::UP);
			m_cells[UP_RIGHT_BACK]->propagateNeighbors(Side::UP);
			break;
		case DOWN:
			m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::DOWN, m_neighbors[DOWN]);
			m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::DOWN, m_neighbors[DOWN]);
			m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::DOWN, m_neighbors[DOWN]);
			m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::DOWN, m_neighbors[DOWN]);

			m_cells[DOWN_LEFT_FRONT]->propagateNeighbors(Side::DOWN);
			m_cells[DOWN_LEFT_BACK]->propagateNeighbors(Side::DOWN);
			m_cells[DOWN_RIGHT_FRONT]->propagateNeighbors(Side::DOWN);
			m_cells[DOWN_RIGHT_BACK]->propagateNeighbors(Side::DOWN);
			break;
		case LEFT:
			m_cells[UP_LEFT_FRONT]->setNeighbor(Side::LEFT, m_neighbors[LEFT]);
			m_cells[UP_LEFT_BACK]->setNeighbor(Side::LEFT, m_neighbors[LEFT]);
			m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::LEFT, m_neighbors[LEFT]);
			m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::LEFT, m_neighbors[LEFT]);

			m_cells[UP_LEFT_FRONT]->propagateNeighbors(Side::LEFT);
			m_cells[UP_LEFT_BACK]->propagateNeighbors(Side::LEFT);
			m_cells[DOWN_LEFT_FRONT]->propagateNeighbors(Side::LEFT);
			m_cells[DOWN_LEFT_BACK]->propagateNeighbors(Side::LEFT);
			break;
		case RIGHT:
			m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::RIGHT, m_neighbors[RIGHT]);
			m_cells[UP_RIGHT_BACK]->setNeighbor(Side::RIGHT, m_neighbors[RIGHT]);
			m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::RIGHT, m_neighbors[RIGHT]);
			m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::RIGHT, m_neighbors[RIGHT]);

			m_cells[UP_RIGHT_FRONT]->propagateNeighbors(Side::RIGHT);
			m_cells[UP_RIGHT_BACK]->propagateNeighbors(Side::RIGHT);
			m_cells[DOWN_RIGHT_FRONT]->propagateNeighbors(Side::RIGHT);
			m_cells[DOWN_RIGHT_BACK]->propagateNeighbors(Side::RIGHT);
			break;
		case FRONT:
			m_cells[UP_LEFT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[FRONT]);
			m_cells[UP_RIGHT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[FRONT]);
			m_cells[DOWN_LEFT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[FRONT]);
			m_cells[DOWN_RIGHT_FRONT]->setNeighbor(Side::FRONT, m_neighbors[FRONT]);

			m_cells[UP_LEFT_FRONT]->propagateNeighbors(Side::FRONT);
			m_cells[UP_RIGHT_FRONT]->propagateNeighbors(Side::FRONT);
			m_cells[DOWN_LEFT_FRONT]->propagateNeighbors(Side::FRONT);
			m_cells[DOWN_RIGHT_FRONT]->propagateNeighbors(Side::FRONT);
			break;
		case BACK:
			m_cells[UP_LEFT_BACK]->setNeighbor(Side::BACK, m_neighbors[BACK]);
			m_cells[UP_RIGHT_BACK]->setNeighbor(Side::BACK, m_neighbors[BACK]);
			m_cells[DOWN_LEFT_BACK]->setNeighbor(Side::BACK, m_neighbors[BACK]);
			m_cells[DOWN_RIGHT_BACK]->setNeighbor(Side::BACK, m_neighbors[BACK]);

			m_cells[UP_LEFT_BACK]->propagateNeighbors(Side::BACK);
			m_cells[UP_RIGHT_BACK]->propagateNeighbors(Side::BACK);
			m_cells[DOWN_LEFT_BACK]->propagateNeighbors(Side::BACK);
			m_cells[DOWN_RIGHT_BACK]->propagateNeighbors(Side::BACK);
			break;
		}
	}

}

inline void VoxelOctree::rootUpdateNeighbors()
{

#ifdef BENCHMARK
	clock_t t = clock();
#endif

	std::queue<VoxelOctree*> queue;

	visited = true;
	queue.push(this);

	while (!queue.empty()) {

		VoxelOctree* curNode = queue.front();
		queue.pop();

		curNode->updateNeighbors();

		for (VoxelOctree* node : curNode->getCells()) {

			if (!node->visited) {
				node->visited = true;
				queue.push(node);
			}
		}
	}

#ifdef BENCHMARK
	t = clock() - t;

	if (!m_parent) // Only root print the whole bench result
		std::cout << "[BENCH] VoxelOctree::rootUpdateNeighbors() | depth " << DEPTH << " : " << ((float)t) << " ms" << std::endl;
#endif

}

inline bool VoxelOctree::debugIsPath(std::vector<OctreePosition> path)
{
	VoxelOctree* current = this;
	bool rightPath = true;

	if (current->parent()) {

		while (current->parent() && rightPath) {

			if (!path.empty() && current->relativePosition() == path.back()) {
				current = current->parent();
				path.pop_back();
			}
			else {
				rightPath = false;
			}
		}

	}
	else {
		rightPath = false;
	}

	if (!path.empty())
		rightPath = false;

	return rightPath;
}

inline void VoxelOctree::countExistingVoxel(int &count)
{
	if (exist) {
		count++;

		if (haveChilds()) {
			for (int i = 0; i < 8; i++)
				m_cells[i]->countExistingVoxel(count);
		}
	}
}

inline int VoxelOctree::sizeOf()
{
	 return m_vertices.size() * sizeof(int) + sizeof(*this);
}

inline void VoxelOctree::addVertices()
{


	float halfSize = m_voxelSize / 2;

	glm::vec3 normal_x = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 normal_mx = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 normal_y = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 normal_my = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 normal_z = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 normal_mz = glm::vec3(0.0f, 0.0f, -1.0f);

	// The 8 vertices are numerated as:

	//         4-------7
	//        /|      /|
	//       / |     / |
	//      5--|----6  |
	//      |  0----|--3
	//      | /     | /
	//      |/      |/
	//      1-------2

	//      y
	//		|
	//      |  x
	//      | /
	//      |/
	//      O-------> z

	glm::vec3 v0 = glm::vec3(m_worldCenter.x + halfSize, m_worldCenter.y - halfSize, m_worldCenter.z - halfSize);
	glm::vec3 v1 = glm::vec3(m_worldCenter.x - halfSize, m_worldCenter.y - halfSize, m_worldCenter.z - halfSize);
	glm::vec3 v2 = glm::vec3(m_worldCenter.x - halfSize, m_worldCenter.y - halfSize, m_worldCenter.z + halfSize);
	glm::vec3 v3 = glm::vec3(m_worldCenter.x + halfSize, m_worldCenter.y - halfSize, m_worldCenter.z + halfSize);
	glm::vec3 v4 = glm::vec3(m_worldCenter.x + halfSize, m_worldCenter.y + halfSize, m_worldCenter.z - halfSize);
	glm::vec3 v5 = glm::vec3(m_worldCenter.x - halfSize, m_worldCenter.y + halfSize, m_worldCenter.z - halfSize);
	glm::vec3 v6 = glm::vec3(m_worldCenter.x - halfSize, m_worldCenter.y + halfSize, m_worldCenter.z + halfSize);
	glm::vec3 v7 = glm::vec3(m_worldCenter.x + halfSize, m_worldCenter.y + halfSize, m_worldCenter.z + halfSize);

	int vaoIndex = ref_vertices->size();

	// UP Face
	ref_vertices->push_back(Vertex(v7, normal_y)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v4, normal_y)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v5, normal_y)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v6, normal_y)); m_vertices.push_back(vaoIndex); vaoIndex++;

	// DOWN Face
	ref_vertices->push_back(Vertex(v0, normal_my)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v3, normal_my)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v2, normal_my)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v1, normal_my)); m_vertices.push_back(vaoIndex); vaoIndex++;

	// LEFT Face
	ref_vertices->push_back(Vertex(v4, normal_mz)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v0, normal_mz)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v1, normal_mz)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v5, normal_mz)); m_vertices.push_back(vaoIndex); vaoIndex++;

	// RIGHT Face
	ref_vertices->push_back(Vertex(v6, normal_z)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v2, normal_z)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v3, normal_z)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v7, normal_z)); m_vertices.push_back(vaoIndex); vaoIndex++;

	// BACK Face
	ref_vertices->push_back(Vertex(v5, normal_mx)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v1, normal_mx)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v2, normal_mx)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v6, normal_mx)); m_vertices.push_back(vaoIndex); vaoIndex++;

	// FRONT Face
	ref_vertices->push_back(Vertex(v7, normal_x)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v3, normal_x)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v0, normal_x)); m_vertices.push_back(vaoIndex); vaoIndex++;
	ref_vertices->push_back(Vertex(v4, normal_x)); m_vertices.push_back(vaoIndex); vaoIndex++;
}


