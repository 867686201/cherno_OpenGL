#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"


Model::Model(const char* filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {            // ��ȡ���������У����� "v 0.123 0.234 0.345"
            iss >> trash;                           // ��ȡ v, ����Ϊ char
            glm::vec3 v;
            for (int i = 0; i < 3; i++)
            {
                iss >> v[i];    // ���ζ�ȡ 0.123 0.234 0.345
            }
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {       // ��ȡ�������У����� "f 1/2/3 4/5/6 7/8/9"
            Face f;
            int itrash, idx;
            iss >> trash;                           // ��ȡ f
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {  // ÿ�ζ�ȡ 1, "/", 2, "/", 3 ���к�4������trash, ֻ��ȡ��һ�� 1 
                idx--;                              // obj �ļ���, ������ 1 ��ʼ, ���Ҫ --
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::Model(const char* filename, int x) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {            // ��ȡ���������У����� "v 0.123 0.234 0.345"
            iss >> trash;                           // ��ȡ v, ����Ϊ char
            glm::vec3 v;
            for (int i = 0; i < 3; i++)
            {
                iss >> v[i];    // ���ζ�ȡ 0.123 0.234 0.345
                v[i] /= 2;      // ��ȡ benny
            }
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {       // ��ȡ�������У����� "f 1/2/3 4/5/6 7/8/9"
            Face f;
            int itrash, idx;
            iss >> trash;                           // ��ȡ f
            while (iss >> idx >> trash >> trash >> itrash) {  // ��ȡ benny.obj
                idx--;                              // obj �ļ���, ������ 1 ��ʼ, ���Ҫ --
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

