#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "enum.h"



struct Voxel {

	bool value;
	glm::vec2 texCoord; // If color value set into texture
	glm::vec3 color; // Vertex Color

	Voxel(bool e = false, glm::vec3 c = glm::vec3(1), glm::vec2 tc = glm::vec2(0)) 
		: value(e), color(c), texCoord(tc) {}

};

class VoxelGrid
{
public:
	VoxelGrid(int width, int height, int depth, float voxelSize = 1.f);
	~VoxelGrid();

	Mesh* mesh() { return m_mesh; }

	Voxel& voxel(int x, int y, int z) { return m_grid[(x * m_height + y) * m_width + z]; }
	void setVoxel(int x, int y, int z, Voxel voxel) { m_grid[(x * m_height + y) * m_width + z] = voxel; }
	void fill();
	void value();

	void buildMesh();
	bool checkSide(Side side, int x, int y, int z);

private:

	Voxel * m_grid;

	float m_voxelSize;

	int m_width; // X axis
	int m_height; // Y axis
	int m_depth; // Z axis

	Mesh* m_mesh;
};

VoxelGrid::VoxelGrid(int width, int height, int depth, float voxelSize)
	: m_width(width),
	m_height(height),
	m_depth(depth),
	m_voxelSize(voxelSize)
{
	m_grid = new Voxel[width*height*depth]();
	m_mesh = new Mesh();
}

VoxelGrid::~VoxelGrid()
{
}

void VoxelGrid::fill()
{
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int z = 0; z < m_depth; z++) {
			
				voxel(x, y, z).value = true;
			}
		}
	}
}

void VoxelGrid::value()
{
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int z = 0; z < m_depth; z++) {

				voxel(x, y, z).value = false;
			}
		}
	}
}

void VoxelGrid::buildMesh()
{
	float halfVoxel = m_voxelSize / 2.f;

	std::vector<Vertex> *vertexArray = m_mesh->getVertices();
	std::vector<GLuint> *indices = m_mesh->getIndices();
	int ei = 0; //element index

	glm::vec3 normal_x = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 normal_mx = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 normal_y = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 normal_my = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 normal_z = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 normal_mz = glm::vec3(0.0f, 0.0f, -1.0f);

	vertexArray->clear();
	indices->clear();

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int z = 0; z < m_depth; z++) {

				// World position of voxel center
				float gx = halfVoxel + x * m_voxelSize;
				float gy = halfVoxel + y * m_voxelSize;
				float gz = halfVoxel + z * m_voxelSize;

				// The 8 vertices are numerated as:

                //         4-------7
                //        /|      /|
                //       / |     / |
                //      5--|----6  |
                //      |  0----|--3
                //      | /     | /
                //      |/      |/
                //      1-------2

				glm::vec3 v0 = glm::vec3(gx - halfVoxel, gy - halfVoxel, gz - halfVoxel);
				glm::vec3 v1 = glm::vec3(gx - halfVoxel, gy - halfVoxel, gz + halfVoxel);
				glm::vec3 v2 = glm::vec3(gx + halfVoxel, gy - halfVoxel, gz + halfVoxel);
				glm::vec3 v3 = glm::vec3(gx + halfVoxel, gy - halfVoxel, gz - halfVoxel);
				glm::vec3 v4 = glm::vec3(gx - halfVoxel, gy + halfVoxel, gz - halfVoxel);
				glm::vec3 v5 = glm::vec3(gx - halfVoxel, gy + halfVoxel, gz + halfVoxel);
				glm::vec3 v6 = glm::vec3(gx + halfVoxel, gy + halfVoxel, gz + halfVoxel);
				glm::vec3 v7 = glm::vec3(gx + halfVoxel, gy + halfVoxel, gz - halfVoxel);

				Voxel &curVoxel = voxel(x, y, z);

				if (curVoxel.value && !checkSide(Side::UP, x, y, z)) {

                    vertexArray->push_back(Vertex(v7, normal_y, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v4, normal_y, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v5, normal_y, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v6, normal_y, curVoxel.texCoord, curVoxel.color));

                    indices->push_back(0 + ei); indices->push_back(1 + ei); indices->push_back(2 + ei);
                    indices->push_back(3 + ei); indices->push_back(0 + ei); indices->push_back(2 + ei);

                    ei += 4;
                }

                if (curVoxel.value && !checkSide(Side::DOWN, x, y, z)) {

                    vertexArray->push_back(Vertex(v0, normal_my, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v3, normal_my, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v2, normal_my, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v1, normal_my, curVoxel.texCoord, curVoxel.color));

                    indices->push_back(0 + ei); indices->push_back(1 + ei); indices->push_back(2 + ei);
                    indices->push_back(3 + ei); indices->push_back(0 + ei); indices->push_back(2 + ei);

                    ei += 4;
                }

                if (curVoxel.value && !checkSide(Side::LEFT, x, y, z)) {

                    vertexArray->push_back(Vertex(v4, normal_mx, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v0, normal_mx, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v1, normal_mx, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v5, normal_mx, curVoxel.texCoord, curVoxel.color));

                    indices->push_back(0 + ei); indices->push_back(1 + ei); indices->push_back(2 + ei);
                    indices->push_back(3 + ei); indices->push_back(0 + ei); indices->push_back(2 + ei);

                    ei += 4;
                }

                if (curVoxel.value && !checkSide(Side::RIGHT, x, y, z)) {

                    vertexArray->push_back(Vertex(v6, normal_x, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v2, normal_x, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v3, normal_x, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v7, normal_x, curVoxel.texCoord, curVoxel.color));

                    indices->push_back(0 + ei); indices->push_back(1 + ei); indices->push_back(2 + ei);
                    indices->push_back(3 + ei); indices->push_back(0 + ei); indices->push_back(2 + ei);

                    ei += 4;
                }

                if (curVoxel.value && !checkSide(Side::BACK, x, y, z)) {

                    vertexArray->push_back(Vertex(v7, normal_mz, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v3, normal_mz, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v0, normal_mz, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v4, normal_mz, curVoxel.texCoord, curVoxel.color));

                    indices->push_back(0 + ei); indices->push_back(1 + ei); indices->push_back(2 + ei);
                    indices->push_back(3 + ei); indices->push_back(0 + ei); indices->push_back(2 + ei);

                    ei += 4;
                }

                if (curVoxel.value && !checkSide(Side::FRONT, x, y, z)) {

                    vertexArray->push_back(Vertex(v5, normal_z, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v1, normal_z, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v2, normal_z, curVoxel.texCoord, curVoxel.color));
                    vertexArray->push_back(Vertex(v6, normal_z, curVoxel.texCoord, curVoxel.color));

                    indices->push_back(0 + ei); indices->push_back(1 + ei); indices->push_back(2 + ei);
                    indices->push_back(3 + ei); indices->push_back(0 + ei); indices->push_back(2 + ei);

                    ei += 4;
                }
			}
		}
	}
}

bool VoxelGrid::checkSide(Side side, int x, int y, int z) {

	switch (side) {
	case UP:
		if (y + 1 < m_height && m_grid[(x * m_height + y + 1) * m_width + z].value)
			return true;
		break;
	case DOWN:
		if (y - 1 >= 0 && m_grid[(x * m_height + y - 1) * m_width + z].value)
			return true;
		break;
	case LEFT:
		if (x - 1 >= 0 && m_grid[((x - 1) * m_height + y) * m_width + z].value) {
			return true;
		}
		break;
	case RIGHT:
		if (x + 1 < m_width && m_grid[((x + 1) * m_height + y) * m_width + z].value) {
			return true;
		}
		break;
	case FRONT:
		if (z + 1 < m_width && m_grid[(x * m_height + y) * m_width + z + 1].value) {
			return true;
		}
		break;
	case BACK:
		if (z - 1 >= 0 && m_grid[(x * m_height + y) * m_width + z - 1].value) {
			return true;
		}
		break;
	}

	return false;
}