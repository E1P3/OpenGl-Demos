#ifndef CAPTURE_DEPTH_H
#define CAPTURE_DEPTH_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../resourceManager.h"
#include <glad/glad.h>
#include <iostream>
#include "stb_image_write.h"

static void captureDepth(){
        // Create FBO
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    int width = ResourceManager::getScreenWidth();
    int height = ResourceManager::getScreenHeight();

    // Create depth buffer texture
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // Check FBO completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // Error handling
        std::cerr << "Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render your scene to the FBO here

    // Read the depth buffer into an image
    std::vector<float> depthData(width * height);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthData.data());

    std::string filename = "depth_image" + std::to_string(ResourceManager::getDeltaTime()) + ".png";
    stbi_write_png(filename.c_str(), width, height, 1, depthData.data(), 0);

    // Cleanup
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &depthTexture);
}

#endif