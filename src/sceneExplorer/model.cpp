#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION

using std::string;

using namespace comUtils::materials;

void Model::draw(Shader &shader) {
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate);	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fatalError(string("ERROR::ASSIMP::") + import.GetErrorString());
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
} 

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        glm::vec3 vector; 
        
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if(mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates? (!= NULL)
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        int strLength = objFilePath.length();
        string mtlFilePath = objFilePath;
        mtlFilePath.replace(strLength-3, 3, "mtl");
     
        matType matType = determineMatType(material,
                                readIllModel(material->GetName().C_Str(), mtlFilePath));        

        Mesh mesh(vertices, indices, matType);
        // Materials other than lambertian have default colors
        if (matType == LAMBERTIAN) {
            // look for texture
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                Texture tex = loadTexture(material, aiTextureType_DIFFUSE);
                mesh.addTexture(tex);
            } else {
                aiColor3D color;
                material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                mesh.setColor(glm::vec4(color.r, color.g, color.b, 1.0f));
            }
        }
        return mesh;
    } else {
        // default material is lambertian
        return Mesh(vertices, indices, LAMBERTIAN);
    }
}

Texture Model::loadTexture(aiMaterial *material, aiTextureType texType){
    aiString texFileName; 
    material->GetTexture(texType, 0, &texFileName);
    string tmp = objFilePath;
    string texFilePath = tmp.erase(tmp.find_last_of("/")+1) + texFileName.C_Str(); 

    Texture texture;

    bool skip = false;
    for (unsigned int i = 0; i < texturesLoaded.size(); i++) {
        if(std::strcmp(texturesLoaded[i].path.c_str(), texFilePath.c_str()) == 0) {
            texture = texturesLoaded[i];
            skip = true;
            break;
        } 
    }
    if (!skip) {
        texture.id = textureFromFile(texFilePath);
        texture.type = texType;
        texture.path = texFilePath;
        texturesLoaded.push_back(texture); // add to loaded textures
    }
    return texture;
}

// Loads texture into OpenGL context and returns texture ID
unsigned int Model::textureFromFile(const string& path, bool gamma) {

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        stbi_image_free(data);
        fatalError("Texture failed to load at path: " + path);
    }

    return textureID;
}