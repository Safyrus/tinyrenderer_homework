#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "structs.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
	std::vector<Vec3f> vertsText_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	int nvertsText();
	Vec3f vert(int i);
	Vec3f vertText(int i);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__
