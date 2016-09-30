#pragma once

#include <iostream>
#include <glm/gtc/noise.hpp>

struct NoiseProperties {

	float frequency;
	float scale;
	int nbOctaves;

	NoiseProperties(const NoiseProperties &np) { frequency = frequency; scale = scale; nbOctaves = nbOctaves; }
	NoiseProperties(float f = 1.f, float s = 1.f, int nbOct = 1) : frequency(f), scale(s), nbOctaves(nbOct) {}

	void print() { std::cout << frequency << " " << scale << " " << nbOctaves << std::endl; }
};

class Noise
{
public:

	Noise();
	
	static float noise2D(float x, float y, float frequency, float scale, int nbOctaves);
	static float noise2D(float x, float y, NoiseProperties *np);

	static float noise3D(float x, float y, float z, float frequency, float scale, int nbOctaves);
	static float noise3D(float x, float y, float z, NoiseProperties noiseProperties);
};

float Noise::noise2D(float x, float y, float frequency, float scale, int nbOctaves) {

	float sum = 0.0f;

	// Compute the sum for each octave
	for (int oct = 0; oct < nbOctaves; oct++) {

		glm::vec2 p(x * frequency, y * frequency);
		float val = glm::simplex(p) / scale;
		sum += val;

		frequency *= 2.f;
		scale *= 2.f;
	}

	return sum;
}

float Noise::noise2D(float x, float y, NoiseProperties *np) {

	float frequency = np->frequency;
	float scale = np->scale;
	int nbOctaves = np->nbOctaves;

	float sum = 0.0f;

	// Compute the sum for each octave
	for (int oct = 0; oct < nbOctaves; oct++) {

		glm::vec2 p(x * frequency, y * frequency);
		float val = glm::simplex(p) / scale;
		sum += val;

		frequency *= 2.f;
		scale *= 2.f;
	}

	return sum;
}


float Noise::noise3D(float x, float y, float z, float frequency, float scale, int nbOctaves) {

	float sum = 0.0f;

	// Compute the sum for each octave
	for (int oct = 0; oct < nbOctaves; oct++) {

		glm::vec3 p(x * frequency, y * frequency, z * frequency);
		float val = glm::simplex(p) / scale;
		sum += val;

		frequency *= 2.f;
		scale *= 2.f;
	}

	return sum;
}

float Noise::noise3D(float x, float y, float z, NoiseProperties noiseProperties) {
	return noise3D(x, y, z, noiseProperties.frequency, noiseProperties.scale, noiseProperties.nbOctaves);
}