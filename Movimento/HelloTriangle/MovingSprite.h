#pragma once
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
class MovingSprite {
public:

    GLuint VAO;
    GLuint textureID;
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
    Shader* shader;
    int currentFrame;
    int totalFrames;
    int frameDuration;
    int elapsedTime;
    int columns;
    int rows;


    MovingSprite(GLuint textureID, Shader* shader, int columns, int rows, float frameDuration)
        : textureID(textureID), shader(shader), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.0f), columns(columns), rows(rows), frameDuration(frameDuration), elapsedTime(0), currentFrame(0) {
        VAO = setupGeometry();
        totalFrames = columns * rows;
    }

    
    int setupGeometry() {
        GLfloat vertices[] = {
            // x     y    z       r    g      b     s     t
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // v0
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // v1
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // v2
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // v1
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // v2
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f   // v3
        };

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return VAO;
    }

    // Função para desenhar o sprite
    void draw(bool right, bool left, bool up, bool down) {
        shader->Use();
        

    // Atualizar a animação
        elapsedTime += glfwGetTime();
        if (elapsedTime % frameDuration == 0) {
            currentFrame = (currentFrame + 1) % totalFrames;                
        }

        if (up && position.y < 336) {

            // Calcular as coordenadas de textura para o quadro atual
            int column = currentFrame % columns;
            int row = 0;
            float s0 = (float)column / (float)columns;
            float t0 = (float)row / (float)rows;
            float s1 = s0 + 1.0f / (float)columns;
            float t1 = t0 + 1.0f / (float)rows;

            position.y += 0.2;
            scale.x -= 0.05;
            scale.y -= 0.05;
            //cout << position.y << endl;
            GLfloat vertices[] = {
                // x     y    z       r    g      b     s     t
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t0,  // v0
                 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t1,  // v2
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s0, t1,  // v2
                 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s1, t1   // v3
            };

            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        };


        if (right && position.x < 650) {

            // Calcular as coordenadas de textura para o quadro atual
            int column = currentFrame % columns;
            int row = 1;
            float s0 = (float)column / (float)columns;
            float t0 = (float)row / (float)rows;
            float s1 = s0 + 1.0f / (float)columns;
            float t1 = t0 + 1.0f / (float)rows;

            position.x += 0.2;
            //cout << position.x << endl;

            GLfloat vertices[] = {
                // x     y    z       r    g      b     s     t
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t0,  // v0
                 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t1,  // v2
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s0, t1,  // v2
                 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s1, t1   // v3
            };
        
            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        };

        if (left && position.x > 150) {

            // Calcular as coordenadas de textura para o quadro atual
            int column = currentFrame % columns;
            int row = 2;
            float s0 = (float)column / (float)columns;
            float t0 = (float)row / (float)rows;
            float s1 = s0 + 1.0f / (float)columns;
            float t1 = t0 + 1.0f / (float)rows;

            position.x -= 0.2;
            //cout << position.x << endl;

            GLfloat vertices[] = {
                // x     y    z       r    g      b     s     t
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t0,  // v0
                 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t1,  // v2
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s0, t1,  // v2
                 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s1, t1   // v3
            };

            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        };

        if (down && position.y > 55) {

            // Calcular as coordenadas de textura para o quadro atual
            int column = currentFrame % columns;
            int row = 3;
            float s0 = (float)column / (float)columns;
            float t0 = (float)row / (float)rows;
            float s1 = s0 + 1.0f / (float)columns;
            float t1 = t0 + 1.0f / (float)rows;

            position.y -= 0.2;
            scale.x += 0.05;
            scale.y += 0.05;
            //cout << position.y << endl;

            GLfloat vertices[] = {
                // x     y    z       r    g      b     s     t
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t0,  // v0
                 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, s0, t1,  // v2
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, s1, t0,  // v1
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s0, t1,  // v2
                 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, s1, t1   // v3
            };

            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        };

        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        shader->setMat4("model", glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};
