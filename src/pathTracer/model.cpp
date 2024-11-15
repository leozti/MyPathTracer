#include "model.hpp"

using std::shared_ptr;
using std::make_shared;
using std::string;

using namespace comUtils::materials;

std::shared_ptr<BvhNode> Model::buildBvh(){
    // Bounding Volume Hierarchies of meshes
    HittableList meshBvhs;
    for (unsigned int i = 0; i < meshes.size(); i++) { 
        meshBvhs.add(meshes[i].buildBvh());
    }
    return make_shared<BvhNode>(meshBvhs);
}

void Model::initialize() {    
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objFilePath, aiProcess_Triangulate);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fatalError(string("ERROR::ASSIMP::") + importer.GetErrorString());
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *assimpMesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(assimpMesh, scene));			
    }
    // then do the same for each of the node's children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);			
    }
}

Mesh Model::processMesh(aiMesh *assimpMesh, const aiScene *scene) {
    Mesh mesh(loadMaterial(assimpMesh, scene));
    mesh.loadTriangles(assimpMesh);
    return mesh;
}

shared_ptr<Material> Model::loadMaterial(aiMesh *assimpMesh, const aiScene *assimpScene){
    if (assimpMesh->mMaterialIndex >= 0) {
        aiMaterial *assimpMat = assimpScene->mMaterials[assimpMesh->mMaterialIndex];
        // Access .mtl file
        int length = objFilePath.length();
        string mtlFilePath = objFilePath;
        mtlFilePath.replace(length-3, 3, "mtl");        
        matType matType = determineMatType(assimpMat, readIllModel(
                                (assimpMat->GetName()).C_Str(), mtlFilePath));

        switch (matType) {
            case LAMBERTIAN:
            default: {  
                // Look for texture (only 1 is allowed per-mesh)
                if (assimpMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                    shared_ptr<ImageTexture> texture = loadTexture(assimpMat, aiTextureType_DIFFUSE);
                    return make_shared<Lambertian>(texture);  
                } else {
                    // If there is no texture to load, use the specified color
                    aiColor3D albedo;
                    assimpMat->Get(AI_MATKEY_COLOR_DIFFUSE, albedo);
                    return make_shared<Lambertian>(Color(albedo.r, albedo.g, albedo.b));
                }
            }
            case METAL: {
                aiColor3D albedo;
                assimpMat->Get(AI_MATKEY_COLOR_SPECULAR, albedo);
                float fuzz;
                // We consider fuzziness as the opposite of shininess
                assimpMat->Get(AI_MATKEY_SHININESS, fuzz);
                fuzz = 1 - (fuzz/1000);
                return make_shared<Metal>(Color(albedo.r, albedo.g, albedo.b), fuzz);
            }
            case DIELECTRIC: {   
                float iof; // Index Of Refraction
                assimpMat->Get(AI_MATKEY_REFRACTI, iof);
                return make_shared<Dielectric>(iof);
            }
            case DIFFUSE_LIGHT: {
                aiColor3D emit;
                assimpMat->Get(AI_MATKEY_COLOR_EMISSIVE, emit);
                return make_shared<DiffuseLight>(Color(emit.r, emit.g, emit.b));
            }
        }
    } else {
        // default material
        return make_shared<Lambertian>(Color(0.0f,0.0f,0.0f));
    }
}

const std::shared_ptr<ImageTexture> Model::loadTexture(aiMaterial *material, aiTextureType texType){
    aiString texImgFileName; // name of texture image file
    material->GetTexture(texType, 0, &texImgFileName);
    string tmp = objFilePath;
    string texImgFilePath = tmp.erase(tmp.find_last_of("/")+1) + texImgFileName.C_Str(); 

    shared_ptr<Image> imgPtr = make_shared<Image>();
    bool skip = false;
    for (unsigned int i = 0; i < loadedTexImages.size(); i++) {
        if(std::strcmp(loadedTexImages[i]->getFilePath().c_str(), texImgFilePath.c_str()) == 0) {
            imgPtr = loadedTexImages[i];
            skip = true;
            break;
        } 
    }
    if (!skip) {
        imgPtr->load(texImgFilePath);
        loadedTexImages.push_back(imgPtr);
    }
    return make_shared<ImageTexture>(imgPtr);
}