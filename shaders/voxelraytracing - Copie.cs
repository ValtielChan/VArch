#version 430

#define MAX_VOXEL 50

layout(rgba32f, binding = 0) uniform image2D frameBuffer;
layout (local_size_x = 16, local_size_y = 16) in;

uniform int texSize;

//  Voxel Scene uniforms
uniform sampler1D colors;
uniform sampler1D mins;
uniform sampler1D maxs;
uniform sampler1D childs;

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

	vec3 tMin = (texelFetch(mins, index, 0).xyz - origin) / dir; // min
	vec3 tMax = (texelFetch(maxs, index, 0).xyz - origin) / dir; // max

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
	int currentChilds[8];
	int currentIndex = 0;

	bool deeper;

	do {

		deeper = false;

		// Childs indexes are dispached on 2 texel
		vec4 tex1 = texelFetch(childs, currentIndex * 2, 0);
		vec4 tex2 = texelFetch(childs, (currentIndex * 2) + 1, 0);

		currentChilds[0] = int(tex1.x);
		currentChilds[1] = int(tex1.y);
		currentChilds[2] = int(tex1.z);
		currentChilds[3] = int(tex1.w);
		currentChilds[4] = int(tex2.x);
		currentChilds[5] = int(tex2.y);
		currentChilds[6] = int(tex2.z);
		currentChilds[7] = int(tex2.w);

		int i; int rightChild = -1;
		for (i = 0; i < 8; i++) {

			if (currentChilds[i] != -1) { // If child exist	

				deeper = true;

				lambda = intersectBox(origin, dir, currentChilds[i]);
				if (lambda.x > 0.0 && lambda.x < lambda.y && lambda.x < smallest) {
					smallest = lambda.x;
					info.lambda = lambda;
					info.bi = 0;
					info.index = currentChilds[i];
					rightChild = currentChilds[i];
				}
			}
		}

		currentIndex = rightChild;

	} while (deeper);

	if (!deeper)
		return true;
	else
		return false;
}

vec4 trace(vec3 origin, vec3 dir) {

	hitinfo hi;

	if (intersectBoxes(origin, dir, hi)) {
		return vec4(texelFetch(colors, hi.index, 0).rgb, 1);
		//return vec4(0, 0, 1, 1);
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
	//imageStore(frameBuffer, pix, normalize(texelFetch(childs, 1356, 0)));
}