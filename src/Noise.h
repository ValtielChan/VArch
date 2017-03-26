#ifndef NOISE_H
#define NOISE_H

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

	Noise() {}
	~Noise() {}

	static float noise2D(float x, float y, float frequency, float scale, int nbOctaves);
	static float noise2D(float x, float y, NoiseProperties *np);

	static float noise3D(float x, float y, float z, float frequency, float scale, int nbOctaves);
	static float noise3D(float x, float y, float z, NoiseProperties noiseProperties);
};

#endif // NOISE_H