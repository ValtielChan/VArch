#include "Tree.h"

Tree::Tree(const TreeParameters &params, VoxelOctreeLOD &lod)
{
	srand(time(NULL));

	m_octree = new VoxelOctree(lod);

	float halfTrunkWidth = params.trunkWidth / 2.0f;

	glm::vec3 marchingVector = glm::vec3(0, -0.5, 0);
	glm::vec3 dominantDirection = glm::vec3(0, halfTrunkWidth, 0);

	// Trunk
	while (marchingVector.y + 0.5f < 0.25f) {
		m_octree->buildSphere(marchingVector, params.trunkWidth, glm::vec3(0.831, 0.372, 0.086));

		int variationBound = halfTrunkWidth * 100;

		// Random direction variation between -0.02 & 0.03
		float randX = (float)((rand() % variationBound) - variationBound / 2) / 100.f;
		float randZ = (float)((rand() % variationBound) - variationBound / 2) / 100.f;

		marchingVector += glm::vec3(randX, 0, randZ) + dominantDirection;
	}

	// BRANCHES
	for (int i = 0; i < params.nbBranches; i++) { // Nb Branches

		float branchWidth = params.trunkWidth / 2;
		glm::vec3 branchVector = marchingVector;

		// Random direction between -1.0f & 1.0f (except for y component)
		glm::vec3 direction = glm::vec3(
			(float)((rand() % 20) - 10) / 10.f,
			(float)(rand() % 10) / 10.f,
			(float)((rand() % 20) - 10) / 10.f
		);

		for (int j = 0; j < 20; j++) { // Nb iter

			float randX = (float)((rand() % 5) - 2) / 10.f;
			float randY = (float)(rand() % 5) / 10.f;
			float randZ = (float)((rand() % 5) - 2) / 10.f;

			if (j == 15) {
				m_octree->buildEllipsoid(
					branchVector, 
					glm::vec3(params.trunkWidth * 2, params.trunkWidth / 2, params.trunkWidth * 2), 
					glm::vec3(0, 1, 0)
				);
			}

			m_octree->buildSphere(branchVector, branchWidth, glm::vec3(0.831, 0.372, 0.086));

			branchVector += (glm::normalize(direction) + glm::vec3(randX, randY, randZ)) * branchWidth;
			branchWidth *= 0.92;
		}
	}

	//DEBUG
	/*glm::vec3 leafColor = glm::vec3(0, 1, 0);
	m_octree->buildEllipsoid(glm::vec3(0), glm::vec3(0.25, 0.1, 0.5), leafColor);*/

	// Build mesh
	m_octree->rootUpdateNeighbors();

	m_octree->resetSelection();
	m_octree->select();
	m_octree->buildTriangles();
}

void Tree::select(Camera* camera) {

	//m_octree->rootUpdateNeighbors();

	m_octree->resetSelection();
	m_octree->select(camera);
	m_octree->buildTriangles();
}

Tree::~Tree()
{
	delete m_octree;
}
