#include "mesh.hpp"

void Mesh::loadTriangles(aiMesh *assimpMesh) {
    triangles.clear();
    for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++) {
        // All faces should be triangles
        Vertex v0, v1, v2;
        aiFace face = assimpMesh->mFaces[i];
        if (face.mNumIndices != 3) {
            fatalError("Error: one of the faces in the mesh is not a triangle");
        }
        v0 = getVertexData(assimpMesh, face.mIndices[0]);
        v1 = getVertexData(assimpMesh, face.mIndices[1]);
        v2 = getVertexData(assimpMesh, face.mIndices[2]);

        triangles.push_back(make_shared<Triangle>(v0, v1, v2, material));
    }
}

std::shared_ptr<BvhNode> Mesh::buildBvh(){
    HittableList list;
    for (unsigned int i = 0; i < triangles.size(); i++) {
        list.add(triangles[i]);
    }
    return std::make_shared<BvhNode>(list);
}        

Vertex Mesh::getVertexData(aiMesh *assimpMesh, unsigned int index){
    Vertex v;
    v.position = getVertexPosition(assimpMesh, index);
    v.normal = getVertexNormal(assimpMesh, index);
    v.texCoords = getVertexTexCoords(assimpMesh, index);
    return v;
} 

Point3 Mesh::getVertexPosition(aiMesh *assimpMesh, unsigned int index){
    Point3 position;
    position.x = assimpMesh->mVertices[index].x;
    position.y = assimpMesh->mVertices[index].y;
    position.z = assimpMesh->mVertices[index].z;
    return position;
}

Vec3 Mesh::getVertexNormal(aiMesh *assimpMesh, unsigned int index){
    Vec3 normal;
    normal.x = assimpMesh->mNormals[index].x;
    normal.y = assimpMesh->mNormals[index].y;
    normal.z = assimpMesh->mNormals[index].z;
    return normal;
}

glm::vec2 Mesh::getVertexTexCoords(aiMesh *assimpMesh, unsigned int index){
    glm::vec2 texCoords;
    if (assimpMesh->mTextureCoords[0]) { 
        // The mesh contains texture coordinates (!= NULL)
        texCoords.x = assimpMesh->mTextureCoords[0][index].x;
        texCoords.y = assimpMesh->mTextureCoords[0][index].y;
    } else {
        texCoords = glm::vec2(0.0f, 0.0f);
    }
    return texCoords;
}