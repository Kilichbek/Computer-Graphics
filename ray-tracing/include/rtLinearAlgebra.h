#pragma once

#include <cmath>
#include <algorithm>
#include "ofMain.h"

#define EPS 1e-4

static inline glm::vec3 scale_vec(const float alpha, const glm::vec3& v)
{
	return glm::vec3(alpha*v[0], alpha*v[1], alpha*v[2]);
}

static inline glm::vec3 hadamard_product(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3(
		v1[0] * v2[0],
		v1[1] * v2[1],
		v1[2] * v2[2]
	);
}
static inline glm::vec3 add_vecs(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3(
		v1[0] + v2[0],
		v1[1] + v2[1],
		v1[2] + v2[2]
	);
}

static inline glm::vec3 subtract_vecs(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3(
		v1[0] - v2[0],
		v1[1] - v2[1],
		v1[2] - v2[2]
	);
}
static inline float dot_product(const glm::vec3& v1, const glm::vec3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

static inline glm::vec3 cross_product(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3(
		v1[1]*v2[2] - v1[2]*v2[1],
		v1[2]*v2[0] - v1[0]*v2[2],
		v1[0]*v2[1] - v1[1]*v2[0]
	);
}

static float vec_length(const glm::vec3& v)
{
	float accum = 0.0;
	for (int i=0; i<3; i++){
		accum += v[i] * v[i];
	}
	return sqrt(accum);
}

static glm::vec3 normalize(const glm::vec3& v)
{
	float vec_len = vec_length(v);
	
	if (vec_len == 0.0) 
		throw std::overflow_error("Divide by zero exception");

	return glm::vec3(
		v[0] / vec_len,
		v[1] / vec_len,
		v[2] / vec_len
	);
}

static bool solve_quadratic(const float a, const float b, const float c, float & t1, float & t2)
{
    auto discriminant = b*b - 4.0*a*c;
    
    if(discriminant < 0.0) return false;
    
    auto root = sqrt(discriminant);
    
    t1 = (-b + root) / (2.0 * a);
    t2 = (-b - root) / (2.0 * a);

    return true;
}

static inline float min_distance(const float t1, const float t2)
{
    auto min_t = std::min(t1,t2);
    auto max_t = std::max(t1,t2);

    return (min_t < EPS) ? max_t: min_t;
}

static inline float max_distance(const float t1, const float t2)
{
    auto min_t = std::min(t1,t2);
    auto max_t = std::max(t1,t2);

    return (max_t < EPS) ? min_t: max_t;
}

static glm::vec3 clamp(const glm::vec3& v, const float min_val, const float max_val)
{
    glm::vec3 clamped_vec = v;
	for (int i=0; i<3; i++){
		auto temp = std::max(v[i],min_val);
		clamped_vec[i] = std::min(temp,max_val);
	}
	return clamped_vec;
}

static glm::vec4 matmul(const glm::mat4& matrix, const glm::vec4& v)
{
	glm::vec4 answer;

	for(int r=0; r<4; r++){
		answer[r] = 0.0;
		for(int c=0;c<4; c++){
			answer[r] += matrix[c][r]*v[c];
		}
	}

	return answer;
}