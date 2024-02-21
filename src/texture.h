#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include <glad/glad.h>
#include "utils/stb_image.h"

enum TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    ROUGHNESS
};

class Texture {
public:
    Texture(TextureType type, std::string filePath, bool useMipmaps = true, GLenum interpolation = GL_LINEAR) {
        this->filePath = filePath;
        this->type = type;
        this->ID = load(useMipmaps, interpolation);
    }
    ~Texture() = default;

    unsigned int load(bool useMipmaps = true, GLenum interpolation = GL_LINEAR) {
        std::cout << "Loading texture: " << filePath;
        const char* path = filePath.c_str();
        unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
        if (data) {
            GLenum format;
            if (channels == 1)
                format = GL_RED;
            else if (channels == 3)
                format = GL_RGB;
            else if (channels == 4)
                format = GL_RGBA;

            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            
            if (useMipmaps) {
                glGenerateMipmap(GL_TEXTURE_2D);
                if (interpolation == GL_NEAREST)
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                else
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

            stbi_image_free(data);

            std::cout << "          Done" << std::endl;
        } else {
            std::cout << "          Failed" << std::endl;
            stbi_image_free(data);
        }

        return ID;
    }

    unsigned int getID() const {
        return ID;
    }

    const std::string& getFilePath() const {
        return filePath;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    TextureType getType() const {
        return type;
    }

    int getChannels() const {
        return channels;
    }

private:
    unsigned int ID;
    std::string filePath;
    int width, height;
    TextureType type;
    int channels;
};

#endif 
