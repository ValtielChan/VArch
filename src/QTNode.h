#ifndef QTNODE_H
#define QTNODE_H

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STL
#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>

#include "Mesh.h"

class Object;

#define MAX_DEPTH 5
#define THRESHOLD 50
#define FRUSTUM_CULLING true

/// <summary>
/// Quadtree cuting of geometry. 
/// Nota: Used (terrain-LOD) rendering only.
/// </summary>
class QTNode
{

public:

	enum ChildIndex {
		UP_LEFT_CHILD, UP_RIGHT_CHILD, BOTTOM_LEFT_CHILD, BOTTOM_RIGHT_CHILD
	};

	enum NeighborIndex {
		UP_NEIGHBOR, RIGHT_NEIGHBOR, BOTTOM_NEIGHBOR, LEFT_NEIGHBOR
	};

	enum DegenerationCase {
		NONE, EVERYONE, TRI, DUAL_VERTICAL, DUAL_HORIZONTAL, LEFT, RIGHT, UP, DOWN
	};

	enum CoverageType {
		FULL_COVERED,
		NON_COVERED,
		UP_LEFT_COVERED,
		UP_RIGHT_COVERED,
		DOWN_LEFT_COVERED,
		DOWN_RIGHT_COVERED,
		UP_RIGHT_DOWN_COVERED,
		UP_LEFT_DOWN_COVERED,
		RIGHT_UP_LEFT_COVERED,
		RIGHT_DOWN_LEFT_COVERED
	};

	QTNode(Object* object, std::vector<GLuint> vertexArray, std::vector<GLuint> *sharedPrimitives, std::vector<Vertex> *sharedVertexArray, glm::vec3 center, float step); // Root constructor
	QTNode(Object* object, QTNode * parent, ChildIndex relativeIndex, std::vector<GLuint> parentVertices, std::vector<GLuint> *sharedPrimitives, std::vector<Vertex> *sharedVertexArray, glm::vec3 center, glm::vec3 minPos, glm::vec3 maxPos);
	~QTNode();

	// Graph parcour
	bool visited;

	QTNode* getParent() { return m_parent; }
	float getStep() { return m_step; }
	int getDepth() { return m_depth;  }
	glm::vec3 getCenter() { return m_center; }
	QTNode* getChild(ChildIndex index) { return m_childs[index]; }
	QTNode* getNeighbor(NeighborIndex index) { return m_neighbours[index]; }
	float getFinestStep() { return m_step / powf(2, MAX_DEPTH); }
	std::vector<QTNode*> getChilds();
	std::vector<Vertex> boundingVertices();
	bool checkNeighbour(NeighborIndex index);

	bool isSubdivided() { return m_subdivided; }
	bool isSelected() { return m_selected; }

	/// <summary>
	/// Il s'agit d'indiquer comment un noeud est "couvert" par ses fils
	/// Par exemple si tous les fils sont selectionné, Il n'y aura pas besoin de dégénérer les triangles du dit noeud 
	/// car tous ses voisin auront forcément la même profondeur
	/// </summary>
	CoverageType coverage();

	bool allChildsSelected() { 
		if(m_depth <= MAX_DEPTH) 
			return m_childs[0]->isSelected() && m_childs[1]->isSelected() && m_childs[2]->isSelected() && m_childs[3]->isSelected();
		else 
			return false;
	}


	void setNeighbour(QTNode* node, NeighborIndex index) { m_neighbours[index] = node; }
	void select(bool b);

	// Method

	void clear();
	
	void rootSetNeighbours();

	/// <summary>
	/// ajoute 4 QTNodes fils (un niveau supplémentaire dans le QTtree). Nota : ajoute dans m_vertexArray (Scene::m_VAO) les vertex manquants si
	/// non encore existants
	/// </summary>
	void subdivide();

	/// <summary>
	/// determine si la POV est dans le chunk, si c'est dedans, détermine la profondeur adéquate et sélectionne les fils qui vont bien
	/// </summary>
	void selection(glm::vec3 pos);

	/// <summary>
	/// Reset la sélection (et que la sélection)
	/// </summary>
	void resetSelection();

	/// <summary>
	/// Met à jour le voisinage d'un noeud en cas de subdivision entrainant un changement
	/// </summary>
	void updateNeighbours();

	/// <summary>
	/// Pour les QTNodes sélectionnés, triangule dans ::m_sharedPrimitives (aka Scene::m_Terrain). Nota : les vertices existent déjà (cf. ::selection)
	/// </summary>
	void buildTriangles();

private:

	// Tree Structure
	QTNode * m_parent;
	QTNode * m_childs[4]; // 0:Up Left, 1:Up Right, 2:Bottom Left, 3:Botton Right
	QTNode * m_neighbours[4]; // 0:Up, 1:Right, 2:Bottom, 3:Left

	glm::vec3 m_childsCenters[4];

	// Mesh Shared Geometry & VertexArray (pointer towards an external geometry)
	std::vector<GLuint> *m_sharedPrimitives;
	std::vector<Vertex> *m_sharedVertexArray;
	Object* ref_object;

	std::vector<GLuint> m_vertexArray; // the 9 vertices of this QTNode (index in external m_sharedVertexArray)

	// Local Attributes
	bool m_subdivided; // (volatile) m_childs creation needed 
	bool m_selected; // (volatile) tells that this QTnode (this level in Quadtree) is selected for current image 

	int m_depth;
	float m_step; // Distance betweek two adjacent vertices (Half of total node width)
	glm::vec3 m_minPos;
	glm::vec3 m_maxPos;

	glm::vec3 m_center; // World center of the node
	ChildIndex m_relativeIndex; // Where is the node compared to his parent
};

#endif // QTNODE_H
