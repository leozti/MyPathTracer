#pragma once

#include "mySceneExp.hpp"
#include "shader.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
}; 

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
            comUtils::materials::matType materialType);

        void draw(Shader &shader);

        // Meshes can only have one color at a time
        void setColor(const glm::vec4& color){
            // Material types other than lambertian have default colors
            if (materialType == comUtils::materials::LAMBERTIAN) {
                this->color = color;
            }
        }
        // Lambertian materials can have a corresponding texture
        void addTexture(Texture texture) {
            if (materialType == comUtils::materials::LAMBERTIAN) {
                this->texture = texture;
                hasValidTexture = true;
            }
        }
    private:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        comUtils::materials::matType materialType;
        glm::vec4 color;
        Texture texture;
        bool hasValidTexture = false;
        // render data
        unsigned int VAO, VBO, EBO;
        
        // Initializes buffers
        void setUpMesh();
};