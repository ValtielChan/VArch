#ifndef VOXELOCTREE_H
#define VOXELOCTREE_H

#include <ctime>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "enum.h"
#include "Mesh.h"

//#define BENCHMARK

class TextureRGB;
class NormalMap;
class HeightMap;
class Camera;

struct VoxelOctreeLOD {
	int maxDepth;
	int depthLOD;
	float threshold;
	bool permissiveFrustum;

	VoxelOctreeLOD(int md, int dLOD, float th, bool pf)
		: maxDepth(md), depthLOD(dLOD), threshold(th), permissiveFrustum(pf) {}
};

struct Voxel {

	glm::vec3 color;
	glm::vec3 normal;
	glm::vec3 min;
	glm::vec3 max;

	Voxel(glm::vec3 c, glm::vec3 n, glm::vec3 mi, glm::vec3 ma)
		: color(c), normal(n), min(mi), max(ma) {}
};

struct Cell {
	Voxel voxel;
	int childs[8];

	Cell(Voxel v) : voxel(v) {}

	void addChild(int i, int childIndex) {
		if (i >= 0 && i < 8) childs[i] = childIndex;
	}
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
	VoxelOctree(VoxelOctreeLOD& lod, glm::vec3 worldCenter = glm::vec3(0), float voxelSize = 1.f);

	/// <summary>
	/// Child Octree construtor
	/// </summary>
	VoxelOctree(VoxelOctree* parent, Mesh* mesh, glm::vec3 worldCenter, float voxelSize, int depth, OctreePosition relPos, VoxelOctreeLOD& lod);

	~VoxelOctree();

	void setNeighbor(Side side, VoxelOctree * neighbor);

	VoxelOctree* parent() { return m_parent; }
	VoxelOctree* getCell(OctreePosition position);
	std::vector<VoxelOctree*> getCells();
	int depth() { return m_depth; }
	Mesh* mesh() { return m_mesh; }
	float voxelSize() { return m_voxelSize; }
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
	void select(Camera* camera = NULL);

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

	/// <summary>
	/// build GLSL uniform ready structure
	/// </summary>
	int GPUStructure(std::vector<Cell*> &GLSLArray);
	void VoxelOctree::GLSLArray(std::vector<Cell*> &GLSLArray);

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
	VoxelOctreeLOD m_lod;

	void addVertices();
};

#endif // !VOXELOCTREE_H