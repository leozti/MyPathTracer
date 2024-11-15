#pragma once

#include "myPT.hpp"
#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(const std::string& objFilePath) : objFilePath(objFilePath) {}

        void initialize();

        unsigned int numberOfMeshes() const {return meshes.size();}

        const Mesh& getMesh(int index){return meshes[index];}

        // Builds a Bounding Volume Hierarchy from Meshes in Model
        std::shared_ptr<BvhNode> buildBvh();

    private:
        // A Model is a list of meshes
        std::vector<Mesh> meshes;
        // path of .obj file
        std::string objFilePath; 
        // texture images that have already been loaded
        std::vector<std::shared_ptr<Image>> loadedTexImages; 

        // Finds all meshes contained in the node and its children
        // and adds them to the vector `meshes`
        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *assimpMesh, const aiScene *scene);

        // Loads material (1 per-mesh) for the specified mesh
        shared_ptr<Material> loadMaterial(aiMesh *assimpMesh, const aiScene *assimpScene);

        const std::shared_ptr<ImageTexture> loadTexture(aiMaterial *material, aiTextureType texType);
};