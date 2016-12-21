#version 430

#define MAX_VOXEL 12000

layout(rgba32f, binding = 0) uniform image2D frameBuffer;
layout (local_size_x = 16, local_size_y = 16) in;

struct Voxel {
	vec3 min;
  	vec3 max;
  	vec4 color;
  	vec3 normal;
};

struct hitinfo {
	vec2 lambda;
	int bi;
};

// Voxel
uniform int nbVoxels;
uniform Voxel scene[MAX_VOXEL];

// Camera & Matrices
uniform vec3 viewPos;
uniform vec3 ray00;
uniform vec3 ray10;
uniform vec3 ray01;
uniform vec3 ray11;

// Test scene
struct box {
  vec3 min;
  vec3 max;
};

const box boxe = { vec3(-1.0, 0.0, -1.0), vec3(1.0, 1.0, 1.0) };

vec2 intersectBox(vec3 origin, vec3 dir, const box b) {

	vec3 tMin = (b.min - origin) / dir;
	vec3 tMax = (b.max - origin) / dir;

	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);

	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);

	return vec2(tNear, tFar);
}

bool intersectBoxes(vec3 origin, vec3 dir, out hitinfo info) {

	float smallest = 5000;
	bool found = false;

	vec2 lambda = intersectBox(origin, dir, boxe);

	if (lambda.x > 0.0 && lambda.x < lambda.y && lambda.x < smallest) {
		info.lambda = lambda;
		info.bi = 0;
		smallest = lambda.x;
		found = true;
	}

	return found;
}

vec4 trace(vec3 origin, vec3 dir) {

	hitinfo i;

	if (intersectBoxes(origin, dir, i)) {
		vec3 gray = vec3(0.6);
		return vec4(gray.rgb, 1.0);
	}

	return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {

	ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
	ivec2 size = imageSize(frameBuffer);

	if (pix.x >= size.x || pix.y >= size.y) {
		return;
	}

	vec2 pos = vec2(pix) / vec2(size.x, size.y);
	vec3 dir = mix(mix(ray00, ray01, pos.y), mix(ray10, ray11, pos.y), pos.x);
	vec4 color = trace(viewPos, dir);

	imageStore(frameBuffer, pix, color);
}