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
        if (!line.compare(0, 2, "v ")) {            // 读取顶点数据行，例如 "v 0.123 0.234 0.345"
            iss >> trash;                           // 读取 v, 类型为 char
            glm::vec3 v;
            for (int i = 0; i < 3; i++)
            {
                iss >> v[i];    // 依次读取 0.123 0.234 0.345
            }
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {       // 读取面数据行，例如 "f 1/2/3 4/5/6 7/8/9"
            Face f;
            int itrash, idx;
            iss >> trash;                           // 读取 f
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {  // 每次读取 1, "/", 2, "/", 3 其中后4个都是trash, 只读取第一个 1 
                idx--;                              // obj 文件中, 索引从 1 开始, 因此要 --
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
        if (!line.compare(0, 2, "v ")) {            // 读取顶点数据行，例如 "v 0.123 0.234 0.345"
            iss >> trash;                           // 读取 v, 类型为 char
            glm::vec3 v;
            for (int i = 0; i < 3; i++)
            {
                iss >> v[i];    // 依次读取 0.123 0.234 0.345
                v[i] /= 2;      // 读取 benny
            }
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {       // 读取面数据行，例如 "f 1/2/3 4/5/6 7/8/9"
            Face f;
            int itrash, idx;
            iss >> trash;                           // 读取 f
            while (iss >> idx >> trash >> trash >> itrash) {  // 读取 benny.obj
                idx--;                              // obj 文件中, 索引从 1 开始, 因此要 --
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

