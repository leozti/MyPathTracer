#pragma once

#include "mySceneExp.hpp"
#include "shader.hpp"
#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(const std::string& objFilePath) : objFilePath(objFilePath){
            loadModel(objFilePath);
        }
        void draw(Shader &shader);	
    private:
        // path of .obj file of model
        std::string objFilePath;
        std::vector<Mesh> meshes;
        std::vector<Texture> texturesLoaded;

        void loadModel(std::string path);
        // Process all of the node's meshes (if any),
        // then do the same for each of its children
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        Texture loadTexture(aiMaterial *material, aiTextureType texType);

        static unsigned int textureFromFile(const std::string& path, bool gamma = false);
};