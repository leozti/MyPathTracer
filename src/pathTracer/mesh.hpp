#pragma once

#include "myPT.hpp"
#include "triangle.hpp"
#include "material.hpp"
#include "bvh.hpp"

#include <assimp/scene.h>

class Mesh {
    public:
        Mesh(std::shared_ptr<Material> material) : material(material) {};

        // Loads triangles of the assimp mesh.
        // All faces in the assimp mesh need to be triangles.
        void loadTriangles(aiMesh *assimpMesh);

        unsigned int numberOfTriangles() const {return triangles.size();}

        // Returns a Bounding Volume Hierarchy built with triangles in mesh
        std::shared_ptr<BvhNode> buildBvh();

    private:
        // A mesh is represented as a list of triangles,
        // all sharing the same material.
        std::vector<std::shared_ptr<Triangle>> triangles;
        std::shared_ptr<Material> material;
        
        // Returns data assigned to the mesh's vertex with index `index`
        Vertex getVertexData(aiMesh *assimpMesh, unsigned int index);

        Point3 getVertexPosition(aiMesh *assimpMesh, unsigned int index);

        Vec3 getVertexNormal(aiMesh *assimpMesh, unsigned int index);

        glm::vec2 getVertexTexCoords(aiMesh *assimpMesh, unsigned int index);
};