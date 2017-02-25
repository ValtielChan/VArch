#version 430

#define MAX_VOXEL 50

layout(rgba32f, binding = 0) uniform image2D frameBuffer;
layout (local_size_x = 16, local_size_y = 16) in;

uniform int texSize;

//  Voxel Scene uniforms
uniform sampler1D colors;
uniform sampler1D mins;
uniform sampler1D maxs;

struct hitinfo {
	vec2 lambda;
	int bi;
	int index;
};

// Camera & Matrices
uniform vec3 viewPos;
uniform vec3 ray00;
uniform vec3 ray10;
uniform vec3 ray01;
uniform vec3 ray11;

// Test scene
struct Box {
  vec3 min;
  vec3 max;
};

//const Box testBox = { vec3(-1.0, -1.0, -1.0), vec3(1.0, 1.0, 1.0) };

vec2 intersectBox(vec3 origin, vec3 dir, int index) {

	vec3 tMin = (texture(mins, index).xyz - origin) / dir; // min
	vec3 tMax = (texture(maxs, index).xyz - origin) / dir; // max

	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);

	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);

	return vec2(tNear, tFar);
}

bool intersectBoxes(vec3 origin, vec3 dir, out hitinfo info) {

	float smallest = 5000;
	bool found = false;

	vec2 lambda;
	int i = 0;
	for (i = 0; i < texSize; i++) {

		lambda = intersectBox(origin, dir, i);

		if (lambda.x > 0.0 && lambda.x < lambda.y && lambda.x < smallest) {

			info.lambda = lambda;
			info.bi = 0;
			info.index = i;
			smallest = lambda.x;
			return true;
		}
	}

	return false;
}

vec4 trace(vec3 origin, vec3 dir) {

	hitinfo hi;

	if (intersectBoxes(origin, dir, hi)) {
		//return vec4(texture(colors, hi.index).rgb, 1);
		return vec4(1.0, 0.0, 0.0, 1.0);
	}

	return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {

	ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
	ivec2 size = imageSize(frameBuffer);

	if (pix.x >= size.x || pix.y >= size.y) {
		return;
	}

	vec2 pos = vec2(pix) / vec2(size.x, size.x);
	vec3 dir = mix(mix(ray00, ray10, pos.x), mix(ray01, ray11, pos.x), pos.y);
	vec4 color = trace(viewPos, dir);

	imageStore(frameBuffer, pix, color);

	// DEBUG PRINT COLOR
	//imageStore(frameBuffer, pix, vec4(0, 0, texSize, 1));
}