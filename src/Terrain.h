#ifndef TERRAIN_H
#define TERRAIN_H

#define PROCESS_PER_FRAME 1
#define REMAIN_PROCESS_FRAME 10
#define UPDATE_NEAREST_FRAME 60

#include "VoxelOctree.h"

class TextureRGB;
class NormalMap;
class HeightMap;
class Camera;
class Object;

class Terrain
{

public:

	Terrain(HeightMap* heightMap, Camera* camera, int nbChunk = 1, float chunkSize = 1.f);
	~Terrain();

	TextureRGB* colorMap() { return m_colorMap; }

	void addMeshesToObject(Object* object);
	void findNearest();
	void process();

	int sizeOf() const;

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
	bool m_alt2;
	int m_nearestFrameCounter;
	int m_remainFrameCounter;
	int m_findNearestCounter;
};

#endif // !TERRAIN_H