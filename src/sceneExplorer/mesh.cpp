#include "mesh.hpp"

using namespace comUtils::materials;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
            matType materialType) : vertices(vertices), indices(indices),
            materialType(materialType) {
    // Materials different from lambertian have a fixed color
    if (materialType == METAL) {
        color = METAL_COLOR;
    } else if (materialType == DIELECTRIC) {
        color = DIELECTRIC_COLOR;
    } else if (materialType == DIFFUSE_LIGHT) {
        color = DIFFUSE_LIGHT_COLOR;
    } else {
        color = glm::vec4(0.0f,0.0f,0.0f,1.0f);
    }
    setUpMesh();
};

void Mesh::setUpMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader)  {
    shader.setUniformBool("hasValidTexture", hasValidTexture);
    if (hasValidTexture) {
        glBindTexture(GL_TEXTURE_2D, texture.id);
    } else {
        shader.setUniformVec4("color", color);
    } 
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}