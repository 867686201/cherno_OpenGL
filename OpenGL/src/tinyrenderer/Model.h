#pragma once

#include <vector>
#include "glm/glm.hpp"

using Face = std::vector<int>;

class Model {
private:
	std::vector<glm::vec3> verts_;
	std::vector<Face> faces_;
public:
	Model(const char* filename);
	Model(const char* filename, int x);
	~Model() {}
	int nverts() { return (int)verts_.size(); }
	int nfaces() { return (int)faces_.size(); }
	Face face(int idx) { return faces_[idx]; }
	glm::vec3 vert(int i) { return verts_[i]; }
};