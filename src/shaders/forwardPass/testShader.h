#ifndef TEST_SHADER_H
#define TEST_SHADER_H

#include "../../shader.h"
#include "../../resourceManager.h"
#include "../../entityModules/renderModule.h"


class TestShader : public Shader {
public:
    TestShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
    }

    void Render() override {
        // Bind the shader program
        this->Use();

        // Bind the VAO
        glBindVertexArray(VAO);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Check for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            // Handle the error here
            std::cout << "OpenGL error: " << error << std::endl;
        }

        // Unbind the VAO
        glBindVertexArray(0);
    }

    void init(){
         this->Use();
        // Create triangle vertices
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // bottom left
            0.5f, -0.5f, 0.0f, // bottom right
            0.0f, 0.5f, 0.0f // top
        };

        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);
    }

    unsigned int VAO, VBO, EBO, ID;
    

};

#endif // TEST_SHADER_H