#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            iss >> v.x;
            iss >> v.y;
            iss >> v.z;
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx, idxText;
            iss >> trash;
            while (iss >> idx >> trash >> idxText >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                idxText--;
                f.push_back(idx);
                f.push_back(idxText);
            }
            // std::cout << f[0] << "/" << f[1] << " " << f[2] << "/" << f[3] << " " << f[4] << "/" << f[5] << std::endl;
            faces_.push_back(f);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash;
            iss >> trash;
            Vec3f v;
            iss >> v.x;
            iss >> v.y;
            iss >> v.z;
            vertsText_.push_back(v);
            // std::cout << v.x << " " << v.y << " " << v.z << std::endl;
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# "  << vertsText_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

int Model::nvertsText() {
    return (int)vertsText_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::vertText(int i) {
    return vertsText_[i];
}
