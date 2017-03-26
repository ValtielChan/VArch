#include "QTNode.h"

#include "Object.h"

QTNode::QTNode(Object* object, std::vector<GLuint> vertexArray, std::vector<GLuint> *sharedPrimitives, std::vector<Vertex> *sharedVertexArray, glm::vec3 center, float step)
	: m_vertexArray(vertexArray),
	m_sharedPrimitives(sharedPrimitives),
	m_sharedVertexArray(sharedVertexArray),
	ref_object(object),
	m_center(center),
	m_step(step),
	m_depth(0),
	m_subdivided(false),
	m_selected(true),
	m_parent(NULL)
{
	for (int i = 0; i < 4; i++) {
		m_childs[i] = NULL;
		m_neighbours[i] = NULL;
	}

	m_childsCenters[0] = m_center - glm::vec3(m_step / 2, 0, m_step / 2);
	m_childsCenters[1] = m_center - glm::vec3(-m_step / 2, 0, m_step / 2);
	m_childsCenters[2] = m_center - glm::vec3(m_step / 2, 0, -m_step / 2);
	m_childsCenters[3] = m_center - glm::vec3(-m_step / 2, 0, -m_step / 2);

	m_minPos = sharedVertexArray->at(0).position;
	m_maxPos = sharedVertexArray->at(8).position;

	for (int i = 0; i < 9; i++) {

		glm::vec3 pos = sharedVertexArray->at(i).position;
		sharedVertexArray->at(i).texCoord = glm::vec2((pos.x - m_minPos.x) / (m_maxPos.x - m_minPos.x), (pos.z - m_minPos.z) / (m_maxPos.z - m_minPos.z));
	}

	// DEBUG
	srand(time(NULL));

	visited = false;
}

QTNode::QTNode(Object* object, QTNode * parent, ChildIndex relativeIndex, std::vector<GLuint> parentVertices, std::vector<GLuint> *sharedPrimitives, std::vector<Vertex> *sharedVertexArray, glm::vec3 center, glm::vec3 minPos, glm::vec3 maxPos)
	: m_parent(parent),
	m_sharedPrimitives(sharedPrimitives),
	m_sharedVertexArray(sharedVertexArray),
	ref_object(object),
	m_center(center),
	m_subdivided(false),
	m_selected(false),
	m_relativeIndex(relativeIndex),
	m_minPos(minPos),
	m_maxPos(maxPos)
{
	// Init
	m_step = m_parent->getStep() / 2;
	m_depth = m_parent->getDepth() + 1;

	for (int i = 0; i < 4; i++) {
		m_childs[i] = NULL;
		m_neighbours[i] = NULL;
	}

	// Build vertexArray
	int vaCurSize = sharedVertexArray->size();

	Vertex v1 = Vertex(glm::vec3(m_center - glm::vec3(0, 0, m_step)));
	Vertex v2 = Vertex(glm::vec3(m_center - glm::vec3(m_step, 0, 0)));
	Vertex v3 = Vertex(glm::vec3(m_center));
	Vertex v4 = Vertex(glm::vec3(m_center + glm::vec3(m_step, 0, 0)));
	Vertex v5 = Vertex(glm::vec3(m_center + glm::vec3(0, 0, m_step)));

	v1.texCoord = glm::vec2((v1.position.x - minPos.x) / (maxPos.x - minPos.x), (v1.position.z - minPos.z) / (maxPos.z - minPos.z));
	v2.texCoord = glm::vec2((v2.position.x - minPos.x) / (maxPos.x - minPos.x), (v2.position.z - minPos.z) / (maxPos.z - minPos.z));
	v3.texCoord = glm::vec2((v3.position.x - minPos.x) / (maxPos.x - minPos.x), (v3.position.z - minPos.z) / (maxPos.z - minPos.z));
	v4.texCoord = glm::vec2((v4.position.x - minPos.x) / (maxPos.x - minPos.x), (v4.position.z - minPos.z) / (maxPos.z - minPos.z));
	v5.texCoord = glm::vec2((v5.position.x - minPos.x) / (maxPos.x - minPos.x), (v5.position.z - minPos.z) / (maxPos.z - minPos.z));

	m_vertexArray.push_back(parentVertices[0]);
	m_vertexArray.push_back(vaCurSize); sharedVertexArray->push_back(v1);
	m_vertexArray.push_back(parentVertices[1]);

	m_vertexArray.push_back(vaCurSize + 1); sharedVertexArray->push_back(v2);
	m_vertexArray.push_back(vaCurSize + 2); sharedVertexArray->push_back(v3);
	m_vertexArray.push_back(vaCurSize + 3); sharedVertexArray->push_back(v4);

	m_vertexArray.push_back(parentVertices[2]);
	m_vertexArray.push_back(vaCurSize + 4); sharedVertexArray->push_back(v5);
	m_vertexArray.push_back(parentVertices[3]);

	if (m_depth < MAX_DEPTH) {
		m_childsCenters[0] = m_center - glm::vec3(m_step / 2, 0, m_step / 2);
		m_childsCenters[1] = m_center - glm::vec3(-m_step / 2, 0, m_step / 2);
		m_childsCenters[2] = m_center - glm::vec3(m_step / 2, 0, -m_step / 2);
		m_childsCenters[3] = m_center - glm::vec3(-m_step / 2, 0, -m_step / 2);
	}

	visited = false;
}

QTNode::~QTNode() {
	clear();
}

void QTNode::clear()
{
	if (m_depth <= MAX_DEPTH) {

		delete m_childs[0];
		delete m_childs[1];
		delete m_childs[2];
		delete m_childs[3];
	}

	if (m_depth == 0) {
		for (int i = 0; i < 4; i++)
			m_childs[i] = NULL;

		m_subdivided = false;
		m_selected = true;
	}
	else {
		m_vertexArray.clear();
	}
}

std::vector<QTNode*> QTNode::getChilds()
{

	std::vector<QTNode*> childs;

	if (m_childs[0])
		childs.push_back(m_childs[0]);

	if (m_childs[1])
		childs.push_back(m_childs[1]);

	if (m_childs[2])
		childs.push_back(m_childs[2]);

	if (m_childs[3])
		childs.push_back(m_childs[3]);

	return childs;
}

std::vector<Vertex> QTNode::boundingVertices()
{
	std::vector<Vertex> vertices;

	vertices.push_back(m_sharedVertexArray->at(m_vertexArray[0]));
	vertices.push_back(m_sharedVertexArray->at(m_vertexArray[2]));
	vertices.push_back(m_sharedVertexArray->at(m_vertexArray[6]));
	vertices.push_back(m_sharedVertexArray->at(m_vertexArray[8]));

	return vertices;
}

void QTNode::select(bool b)
{
	if (b && m_parent && !m_selected) {
		m_parent->select(true);
		m_selected = true;

		for (int i = 0; i < 4; i++) {
			if (m_neighbours[i] && m_neighbours[i]->getDepth() < m_depth && !m_neighbours[i]->isSelected())
				m_neighbours[i]->select(true);
			else if (m_neighbours[i] && m_neighbours[i]->getDepth() == m_depth && !m_neighbours[i]->isSelected() && m_neighbours[i]->getParent())
				m_neighbours[i]->getParent()->select(true);
		}
	}
	else {
		m_selected = b;
	}
}

/// <summary>
/// ajoute 4 QTNodes fils (un niveau supplémentaire dans le QTtree). Nota : ajoute dans m_vertexArray (Scene::m_VAO) les vertex manquants si
/// non encore existants
/// </summary>
void QTNode::subdivide()
{
	glm::vec3 childCenter;
	std::vector<GLuint> parentVerts;

	if (!m_subdivided) {

		// UP_LEFT_CHILD :

		parentVerts.push_back(m_vertexArray[0]);
		parentVerts.push_back(m_vertexArray[1]);
		parentVerts.push_back(m_vertexArray[3]);
		parentVerts.push_back(m_vertexArray[4]);

		childCenter = m_center - glm::vec3(m_step / 2, 0, m_step / 2);

		// Internal pointing
		//+---+---+
		//| X |-> |
		//+-|-+---+
		//| V |   |
		//+---+---+

		m_childs[UP_LEFT_CHILD] = new QTNode(ref_object, this, UP_LEFT_CHILD, parentVerts, m_sharedPrimitives, m_sharedVertexArray, childCenter, m_minPos, m_maxPos);

		parentVerts.clear();

		// UP_RIGHT_CHILD :

		parentVerts.push_back(m_vertexArray[1]);
		parentVerts.push_back(m_vertexArray[2]);
		parentVerts.push_back(m_vertexArray[4]);
		parentVerts.push_back(m_vertexArray[5]);

		childCenter = m_center - glm::vec3(-m_step / 2, 0, m_step / 2);

		// Internal pointing
		//+---+---+
		//| <-| X |
		//+---+-|-+
		//|   | V |
		//+---+---+

		m_childs[UP_RIGHT_CHILD] = new QTNode(ref_object, this, UP_RIGHT_CHILD, parentVerts, m_sharedPrimitives, m_sharedVertexArray, childCenter, m_minPos, m_maxPos);

		parentVerts.clear();

		// BOTTOM_LEFT_CHILD :

		parentVerts.push_back(m_vertexArray[3]);
		parentVerts.push_back(m_vertexArray[4]);
		parentVerts.push_back(m_vertexArray[6]);
		parentVerts.push_back(m_vertexArray[7]);

		childCenter = m_center - glm::vec3(m_step / 2, 0, -m_step / 2);

		// Internal pointing
		//+---+---+
		//| ^ |   |
		//+-|-+---+
		//| X |-> |
		//+---+---+

		m_childs[BOTTOM_LEFT_CHILD] = new QTNode(ref_object, this, BOTTOM_LEFT_CHILD, parentVerts, m_sharedPrimitives, m_sharedVertexArray, childCenter, m_minPos, m_maxPos);

		parentVerts.clear();

		// BOTTOM_RIGHT_CHILD :

		parentVerts.push_back(m_vertexArray[4]);
		parentVerts.push_back(m_vertexArray[5]);
		parentVerts.push_back(m_vertexArray[7]);
		parentVerts.push_back(m_vertexArray[8]);

		childCenter = m_center - glm::vec3(-m_step / 2, 0, -m_step / 2);

		// Internal pointing
		//+---+---+
		//|   | ^ |
		//+---+-|-+
		//| <-| X |
		//+---+---+

		m_childs[BOTTOM_RIGHT_CHILD] = new QTNode(ref_object, this, BOTTOM_RIGHT_CHILD, parentVerts, m_sharedPrimitives, m_sharedVertexArray, childCenter, m_minPos, m_maxPos);

		//Brothers setup
		m_childs[UP_LEFT_CHILD]->setNeighbour(m_childs[UP_RIGHT_CHILD], RIGHT_NEIGHBOR);
		m_childs[UP_LEFT_CHILD]->setNeighbour(m_childs[BOTTOM_LEFT_CHILD], BOTTOM_NEIGHBOR);

		m_childs[UP_RIGHT_CHILD]->setNeighbour(m_childs[BOTTOM_RIGHT_CHILD], BOTTOM_NEIGHBOR);
		m_childs[UP_RIGHT_CHILD]->setNeighbour(m_childs[UP_LEFT_CHILD], LEFT_NEIGHBOR);

		m_childs[BOTTOM_LEFT_CHILD]->setNeighbour(m_childs[UP_LEFT_CHILD], UP_NEIGHBOR);
		m_childs[BOTTOM_LEFT_CHILD]->setNeighbour(m_childs[BOTTOM_RIGHT_CHILD], RIGHT_NEIGHBOR);

		m_childs[BOTTOM_RIGHT_CHILD]->setNeighbour(m_childs[UP_RIGHT_CHILD], UP_NEIGHBOR);
		m_childs[BOTTOM_RIGHT_CHILD]->setNeighbour(m_childs[BOTTOM_LEFT_CHILD], LEFT_NEIGHBOR);

		m_subdivided = true;
	}

	// Sudivide coarse neighbors if necessary
	if (m_neighbours[UP_NEIGHBOR] && m_neighbours[UP_NEIGHBOR]->getDepth() < m_depth)
		m_neighbours[UP_NEIGHBOR]->subdivide();

	if (m_neighbours[RIGHT_NEIGHBOR] && m_neighbours[RIGHT_NEIGHBOR]->getDepth() < m_depth)
		m_neighbours[RIGHT_NEIGHBOR]->subdivide();

	if (m_neighbours[BOTTOM_NEIGHBOR] && m_neighbours[BOTTOM_NEIGHBOR]->getDepth() < m_depth)
		m_neighbours[BOTTOM_NEIGHBOR]->subdivide();

	if (m_neighbours[LEFT_NEIGHBOR] && m_neighbours[LEFT_NEIGHBOR]->getDepth() < m_depth)
		m_neighbours[LEFT_NEIGHBOR]->subdivide();

	// Update 
	updateNeighbours();

	m_childs[UP_LEFT_CHILD]->updateNeighbours();
	m_childs[UP_RIGHT_CHILD]->updateNeighbours();
	m_childs[BOTTOM_LEFT_CHILD]->updateNeighbours();
	m_childs[BOTTOM_RIGHT_CHILD]->updateNeighbours();
}

void QTNode::rootSetNeighbours()
{
	std::queue<QTNode*> queue;

	visited = true;
	queue.push(this);

	while (!queue.empty()) {

		QTNode* curNode = queue.front();
		queue.pop();

		//std::cout << "Relative Index : " << curNodem_relativeIndex << std::endl;
		curNode->updateNeighbours();

		for (QTNode* n : curNode->getChilds()) {

			if (!n->visited) {
				n->visited = true;
				queue.push(n);
				//std::cout << "Add child to queue" << std::endl;
			}
		}
	}
}

/**
Pre-condition : the parent node have an optimal neighborhood (same depth)
**/
void QTNode::updateNeighbours() {

	if (m_parent) {

		switch (m_relativeIndex) {
		case UP_LEFT_CHILD:

			// Set pointers to Up neighbor and the Left one
			if (m_parent->getNeighbor(UP_NEIGHBOR)) {
				if (m_parent->getNeighbor(UP_NEIGHBOR)->isSubdivided()) {
					m_neighbours[UP_NEIGHBOR] = m_parent->getNeighbor(UP_NEIGHBOR)->getChild(BOTTOM_LEFT_CHILD);
					m_parent->getNeighbor(UP_NEIGHBOR)->getChild(BOTTOM_LEFT_CHILD)->setNeighbour(this, BOTTOM_NEIGHBOR);
				}
				else {
					m_neighbours[UP_NEIGHBOR] = m_parent->getNeighbor(UP_NEIGHBOR);
				}
			}

			if (m_parent->getNeighbor(LEFT_NEIGHBOR)) {
				if (m_parent->getNeighbor(LEFT_NEIGHBOR)->isSubdivided()) {
					m_neighbours[LEFT_NEIGHBOR] = m_parent->getNeighbor(LEFT_NEIGHBOR)->getChild(UP_RIGHT_CHILD);
					m_parent->getNeighbor(LEFT_NEIGHBOR)->getChild(UP_RIGHT_CHILD)->setNeighbour(this, RIGHT_NEIGHBOR);
				}
				else {
					m_neighbours[LEFT_NEIGHBOR] = m_parent->getNeighbor(LEFT_NEIGHBOR);
				}
			}

			break;
		case UP_RIGHT_CHILD:

			// Set pointers to Up neighbor and the Right one

			if (m_parent->getNeighbor(UP_NEIGHBOR)) {
				if (m_parent->getNeighbor(UP_NEIGHBOR)->isSubdivided()) {
					m_neighbours[UP_NEIGHBOR] = m_parent->getNeighbor(UP_NEIGHBOR)->getChild(BOTTOM_RIGHT_CHILD);
					m_parent->getNeighbor(UP_NEIGHBOR)->getChild(BOTTOM_RIGHT_CHILD)->setNeighbour(this, BOTTOM_NEIGHBOR);
				}
				else {
					m_neighbours[UP_NEIGHBOR] = m_parent->getNeighbor(UP_NEIGHBOR);
				}
			}

			if (m_parent->getNeighbor(RIGHT_NEIGHBOR)) {
				if (m_parent->getNeighbor(RIGHT_NEIGHBOR)->isSubdivided()) {
					m_neighbours[RIGHT_NEIGHBOR] = m_parent->getNeighbor(RIGHT_NEIGHBOR)->getChild(UP_LEFT_CHILD);
					m_parent->getNeighbor(RIGHT_NEIGHBOR)->getChild(UP_LEFT_CHILD)->setNeighbour(this, LEFT_NEIGHBOR);
				}
				else {
					m_neighbours[RIGHT_NEIGHBOR] = m_parent->getNeighbor(RIGHT_NEIGHBOR);
				}
			}

			break;
		case BOTTOM_LEFT_CHILD:

			// Set pointers to Bottom neighbor and the Left one

			if (m_parent->getNeighbor(BOTTOM_NEIGHBOR)) {
				if (m_parent->getNeighbor(BOTTOM_NEIGHBOR)->isSubdivided()) {
					m_neighbours[BOTTOM_NEIGHBOR] = m_parent->getNeighbor(BOTTOM_NEIGHBOR)->getChild(UP_LEFT_CHILD);
					m_parent->getNeighbor(BOTTOM_NEIGHBOR)->getChild(UP_LEFT_CHILD)->setNeighbour(this, UP_NEIGHBOR);
				}
				else {
					m_neighbours[BOTTOM_NEIGHBOR] = m_parent->getNeighbor(BOTTOM_NEIGHBOR);
				}
			}

			if (m_parent->getNeighbor(LEFT_NEIGHBOR)) {
				if (m_parent->getNeighbor(LEFT_NEIGHBOR)->isSubdivided()) {
					m_neighbours[LEFT_NEIGHBOR] = m_parent->getNeighbor(LEFT_NEIGHBOR)->getChild(BOTTOM_RIGHT_CHILD);
					m_parent->getNeighbor(LEFT_NEIGHBOR)->getChild(BOTTOM_RIGHT_CHILD)->setNeighbour(this, RIGHT_NEIGHBOR);
				}
				else {
					m_neighbours[LEFT_NEIGHBOR] = m_parent->getNeighbor(LEFT_NEIGHBOR);
				}
			}

			break;
		case BOTTOM_RIGHT_CHILD:

			// Set pointers to Bottom neighbor and the Left one

			if (m_parent->getNeighbor(BOTTOM_NEIGHBOR)) {
				if (m_parent->getNeighbor(BOTTOM_NEIGHBOR)->isSubdivided()) {
					m_neighbours[BOTTOM_NEIGHBOR] = m_parent->getNeighbor(BOTTOM_NEIGHBOR)->getChild(UP_RIGHT_CHILD);
					m_parent->getNeighbor(BOTTOM_NEIGHBOR)->getChild(UP_RIGHT_CHILD)->setNeighbour(this, UP_NEIGHBOR);
				}
				else {
					m_neighbours[BOTTOM_NEIGHBOR] = m_parent->getNeighbor(BOTTOM_NEIGHBOR);
				}
			}

			if (m_parent->getNeighbor(RIGHT_NEIGHBOR)) {
				if (m_parent->getNeighbor(RIGHT_NEIGHBOR)->isSubdivided()) {
					m_neighbours[RIGHT_NEIGHBOR] = m_parent->getNeighbor(RIGHT_NEIGHBOR)->getChild(BOTTOM_LEFT_CHILD);
					m_parent->getNeighbor(RIGHT_NEIGHBOR)->getChild(BOTTOM_LEFT_CHILD)->setNeighbour(this, LEFT_NEIGHBOR);
				}
				else {
					m_neighbours[RIGHT_NEIGHBOR] = m_parent->getNeighbor(RIGHT_NEIGHBOR);
				}
			}

			break;
		}
	}
}

/// <summary>
/// determine si la POV est dans le chunk, si c'est dedans, détermine la profondeur adéquate et sélectionne les fils qui vont bien
/// </summary>
void QTNode::selection(glm::vec3 pos) // POV = world position
{
	//float threshold = 5;

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 2; i++) {

			bool goDeeper = false;

			if (m_depth < MAX_DEPTH) {

				// Distance between node & POV
				//glm::vec3 childCenter = m_childs[i * 2 + j]->getCenter();
				glm::vec3 childCenter = m_childsCenters[i * 2 + j];
				childCenter += ref_object->transform.position();

				float x = childCenter.x - pos.x;
				float y = childCenter.z - pos.z;
				float dist = sqrt(x*x + y*y);

				//std::cout << dist << std::endl;

				if (dist < THRESHOLD / (m_depth + 1)) { // Hint (TODO) : THRESHOLD represente les range de LOD de rendu

					if (!m_subdivided)
						subdivide();

					goDeeper = true;
				}
			}

			if (goDeeper) {
				m_childs[i * 2 + j]->selection(pos);
			}
		}
	}

	// Refine needed neighbours
	for (int i = 0; i < 4; i++) {
		if (m_neighbours[i] && m_neighbours[i]->getDepth() == m_depth && !m_neighbours[i]->isSelected() && m_neighbours[i]->getParent())
			m_neighbours[i]->getParent()->select(true);
		else if (m_neighbours[i] && m_neighbours[i]->getDepth() < m_depth && !m_neighbours[i]->isSelected())
			m_neighbours[i]->select(true);
	}

	m_selected = true;
}

void QTNode::buildTriangles()
{
	CoverageType ct = coverage();

	switch (ct) {

		// Check Coverage case (c.f. coverage())
	case FULL_COVERED:

		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 2; i++) {

				if (m_depth <= MAX_DEPTH && m_childs[j * 2 + i]->isSelected()) {
					m_childs[j * 2 + i]->buildTriangles();
				}
				else {

					m_sharedPrimitives->push_back(m_vertexArray[j * 3 + i]);
					m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
					m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

					m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
					m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + (i + 1)]);
					m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);
				}
			}
		}

		break;

	case RIGHT_UP_LEFT_COVERED:

		m_childs[0]->buildTriangles();
		m_childs[1]->buildTriangles();

		// Apply appropriate degenaration case if necessary
		if (!checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else {

			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}

		break;

	case RIGHT_DOWN_LEFT_COVERED:

		m_childs[2]->buildTriangles();
		m_childs[3]->buildTriangles();

		if (!checkNeighbour(UP_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}

		break;

	case UP_LEFT_DOWN_COVERED:

		m_childs[0]->buildTriangles();
		m_childs[2]->buildTriangles();

		if (!checkNeighbour(RIGHT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]);
			m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[2]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[2]);

		}
		else {

			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}

		break;

	case UP_RIGHT_DOWN_COVERED:

		m_childs[1]->buildTriangles();
		m_childs[3]->buildTriangles();

		if (!checkNeighbour(LEFT_NEIGHBOR)) {


			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}

		break;

	case UP_LEFT_COVERED:

		m_childs[0]->buildTriangles();

		if (!checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (!checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]);
			m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[2]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[2]);

		}
		else if (checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}

		else {

			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 2; i++) {

					if (j * 2 + i != 0) {

						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + (i + 1)]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

					}
				}
			}
		}

		break;

	case UP_RIGHT_COVERED:

		m_childs[1]->buildTriangles();

		if (!checkNeighbour(LEFT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (!checkNeighbour(LEFT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (checkNeighbour(LEFT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else {

			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 2; i++) {

					if (j * 2 + i != 1) {

						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + (i + 1)]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);
					}
				}
			}
		}

		break;

	case DOWN_LEFT_COVERED:

		m_childs[2]->buildTriangles();

		if (!checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(UP_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (!checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(UP_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(UP_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else {

			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 2; i++) {

					if (j * 2 + i != 2) {

						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + (i + 1)]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

					}
				}
			}
		}

		break;

	case DOWN_RIGHT_COVERED:

		m_childs[3]->buildTriangles();

		if (!checkNeighbour(LEFT_NEIGHBOR) && !checkNeighbour(UP_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (!checkNeighbour(LEFT_NEIGHBOR) && checkNeighbour(UP_NEIGHBOR)) {


			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (checkNeighbour(LEFT_NEIGHBOR) && !checkNeighbour(UP_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else {

			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 2; i++) {

					if (j * 2 + i != 3) {

						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
						m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + (i + 1)]);
						m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

					}
				}
			}
		}

		break;

	case NON_COVERED:

		if (checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 2; i++) {

					m_sharedPrimitives->push_back(m_vertexArray[j * 3 + i]);
					m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
					m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);

					m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + i]);
					m_sharedPrimitives->push_back(m_vertexArray[(j + 1) * 3 + (i + 1)]);
					m_sharedPrimitives->push_back(m_vertexArray[j * 3 + (i + 1)]);
				}
			}
		}
		else if (!checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else if (checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (!checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else if (checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}
		else if (!checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[4]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (!checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[3]);
			m_sharedPrimitives->push_back(m_vertexArray[3]); m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[5]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]);

		}
		else if (checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[7]); m_sharedPrimitives->push_back(m_vertexArray[6]);
			m_sharedPrimitives->push_back(m_vertexArray[1]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[7]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[7]);

		}
		else if (!checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && checkNeighbour(LEFT_NEIGHBOR)
			|| checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)
			|| !checkNeighbour(UP_NEIGHBOR) && checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)
			|| !checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)
			|| !checkNeighbour(UP_NEIGHBOR) && !checkNeighbour(RIGHT_NEIGHBOR) && !checkNeighbour(BOTTOM_NEIGHBOR) && !checkNeighbour(LEFT_NEIGHBOR)) {

			m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[2]); m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[8]); m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[4]);
			m_sharedPrimitives->push_back(m_vertexArray[6]); m_sharedPrimitives->push_back(m_vertexArray[0]); m_sharedPrimitives->push_back(m_vertexArray[4]);

		}

		break;
	}
}

QTNode::CoverageType QTNode::coverage()
{
	if (m_depth >= MAX_DEPTH) {
		return NON_COVERED;
	}
	else if (m_subdivided) {

		if ((m_childs[0]->isSelected() && m_childs[3]->isSelected()) || (m_childs[1]->isSelected() && m_childs[2]->isSelected()))
			return FULL_COVERED;

		else if (m_childs[0]->isSelected() && m_childs[1]->isSelected())
			return RIGHT_UP_LEFT_COVERED;

		else if (m_childs[0]->isSelected() && m_childs[2]->isSelected())
			return UP_LEFT_DOWN_COVERED;

		else if (m_childs[1]->isSelected() && m_childs[3]->isSelected())
			return UP_RIGHT_DOWN_COVERED;

		else if (m_childs[2]->isSelected() && m_childs[3]->isSelected())
			return RIGHT_DOWN_LEFT_COVERED;

		else if (m_childs[0]->isSelected() && !m_childs[1]->isSelected() && !m_childs[2]->isSelected() && !m_childs[3]->isSelected())
			return UP_LEFT_COVERED;

		else if (m_childs[1]->isSelected() && !m_childs[2]->isSelected() && !m_childs[3]->isSelected() && !m_childs[0]->isSelected())
			return UP_RIGHT_COVERED;

		else if (m_childs[2]->isSelected() && !m_childs[3]->isSelected() && !m_childs[0]->isSelected() && !m_childs[1]->isSelected())
			return DOWN_LEFT_COVERED;

		else if (m_childs[3]->isSelected() && !m_childs[0]->isSelected() && !m_childs[1]->isSelected() && !m_childs[2]->isSelected())
			return DOWN_RIGHT_COVERED;

		else
			return NON_COVERED;
	}
	else {
		return NON_COVERED;
	}
}

//Degenerate if it return false
bool QTNode::checkNeighbour(NeighborIndex index) {

	if (m_neighbours[index]) {
		if (m_neighbours[index]->getDepth() == m_depth)
			return m_neighbours[index]->isSelected();
		else
			return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// Reset la sélection (et que la sélection)
/// </summary>
void QTNode::resetSelection() {

	if (m_depth != 0)
		m_selected = false;

	if (m_depth < MAX_DEPTH && m_subdivided) {

		m_childs[0]->resetSelection();
		m_childs[1]->resetSelection();
		m_childs[2]->resetSelection();
		m_childs[3]->resetSelection();
	}
}