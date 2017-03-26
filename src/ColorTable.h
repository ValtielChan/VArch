#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

class ColorTable
{
	int m_size;

	std::vector<int> m_tags;
	glm::vec3 *m_colors;

public:
	ColorTable(int size);

	void addTag(glm::vec3 color, float location);
	glm::vec3 *generateTable();

	glm::vec2 texCoord(float index);
	glm::vec3 *glColors();

	int size() const & { return m_size; }
	glm::vec3 color(int index) const & { return m_colors[index]; }

	void printTags();

	// Preset
	static ColorTable Nature(int size);
	static ColorTable Volcano(int size);
};

#endif //COLORTABLE_H