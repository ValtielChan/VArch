#include "ColorTable.h"

//#include "TextureRGB.h"

ColorTable::ColorTable(int size) : m_size(size)
{
	m_colors = new glm::vec3[size]();
}

void ColorTable::addTag(glm::vec3 color, float location)
{
	m_tags.push_back((m_size - 1)*location);
	m_colors[m_tags.back()] = color;
}

glm::vec3 * ColorTable::generateTable()
{
	for (int i = 0; i < m_tags.size() - 1; i++) {

		glm::vec3 c1 = m_colors[m_tags[i]];
		glm::vec3 c2 = m_colors[m_tags[i + 1]];

		float delta = abs(m_tags[i] - m_tags[i + 1]);

		float rStep = (c2.r - c1.r) / delta;
		float gStep = (c2.g - c1.g) / delta;
		float bStep = (c2.b - c1.b) / delta;

		for (int j = m_tags[i] + 1; j < m_tags[i + 1]; j++) {
			glm::vec3 prevColor = m_colors[j - 1];
			m_colors[j] = glm::vec3(prevColor.r + rStep, prevColor.g + gStep, prevColor.b + bStep);
		}
	}

	return m_colors;
}

glm::vec2 ColorTable::texCoord(float index)
{
	return glm::vec2(index / (m_size - 1), 0.f);
}

glm::vec3 *ColorTable::glColors()
{
	return m_colors;
}

void ColorTable::printTags()
{
	for (int t : m_tags)
		std::cout << t << " ";

	std::cout << std::endl;
}

ColorTable ColorTable::Nature(int size)
{
	ColorTable ct(size);

	ct.addTag(glm::vec3(1.f, 1.f, 0.6f), 0.f);
	ct.addTag(glm::vec3(0.f, 1.f, 0.f), 0.3f);
	ct.addTag(glm::vec3(0.7f, 0.7f, 0.7f), 0.7f);
	ct.addTag(glm::vec3(0.1f, 0.1f, 0.1f), 0.9f);
	ct.addTag(glm::vec3(1.f, 1.f, 1.f), 1.f);

	ct.generateTable();

	return ct;
}

ColorTable ColorTable::Volcano(int size)
{
	ColorTable ct(size);

	ct.addTag(glm::vec3(1.f, 0.f, 0.f), 0.f);
	ct.addTag(glm::vec3(1.f, 0.6f, 0.f), 0.3f);
	ct.addTag(glm::vec3(0.7f, 0.7f, 0.7f), 0.7f);
	ct.addTag(glm::vec3(0.f, 0.f, 0.f), 1.f);

	ct.generateTable();

	return ct;
}
