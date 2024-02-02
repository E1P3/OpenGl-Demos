#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include "utils/stb_image.h"

class Cubemap {
public:
    Cubemap(const std::string* faces) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

        int width, height, channels;
        for (unsigned int i = 0; i < 6; i++) {
            std::cout << "Loading texture: " << faces[i];
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                std::cout << "          Done" << std::endl;
            } else {
                stbi_image_free(data);
                std::cout << "          Failed" << std::endl;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    ~Cubemap() = default;

    unsigned int getID() const {
        return ID;
    }

private:
    unsigned int ID;

};

#endif // CUBE_MAP_H