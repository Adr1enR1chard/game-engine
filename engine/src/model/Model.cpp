#include <engine/model/Model.hpp>

#include <engine/model/MaterialInstance.hpp>

#include <iostream>

glm::mat4 AiMatrixToGlmMat4(const aiMatrix4x4& from)
{
    glm::mat4 to;
    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;
    return to;
}

void Model::Draw(MaterialInstance& materialInstance, glm::mat4 modelMatrix) const
{
    for (const auto& mesh : meshes) {
        mesh->Draw(materialInstance, modelMatrix);
    }
}

// void Model::DrawWithExternalShader(Shader& shader, glm::mat4 modelMatrix) const
// {
//     for (const auto& mesh : meshes) {
//         shader.setMat4("model", modelMatrix * mesh->localModel);
//         mesh->Draw();
//     }
// }

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene*   scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
    glm::mat4 nodeTransform = parentTransform * AiMatrixToGlmMat4(node->mTransformation);

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh          = scene->mMeshes[node->mMeshes[i]];
        auto    processedMesh = processMesh(mesh, scene, nodeTransform);
        meshes.push_back(processedMesh);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, nodeTransform);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* /*scene*/, const glm::mat4& nodeTransform)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex    vertex;
        glm::vec3 vector;
        vector.x         = mesh->mVertices[i].x;
        vector.y         = mesh->mVertices[i].y;
        vector.z         = mesh->mVertices[i].z;
        vertex.position  = vector;
        vector.x         = mesh->mNormals[i].x;
        vector.y         = mesh->mNormals[i].y;
        vector.z         = mesh->mNormals[i].z;
        vertex.normal    = vector;
        vector.x         = mesh->mTangents[i].x;
        vector.y         = mesh->mTangents[i].y;
        vector.z         = mesh->mTangents[i].z;
        vertex.tangent   = vector;
        vector.x         = mesh->mBitangents[i].x;
        vector.y         = mesh->mBitangents[i].y;
        vector.z         = mesh->mBitangents[i].z;
        vertex.bitangent = vector;
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        // process vertex positions, normals and texture coordinates
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    // if (mesh->mMaterialIndex >= 0)
    // {
    //     aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    //     std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
    //                                                             aiTextureType_DIFFUSE, "texture_diffuse");
    //     textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //     std::vector<Texture> specularMaps = loadMaterialTextures(material,
    //                                                              aiTextureType_SPECULAR, "texture_specular");
    //     textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // }

    return std::make_shared<Mesh>(vertices, indices, nodeTransform);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    mat;
    type;
    typeName;
    return std::vector<Texture>();
}
