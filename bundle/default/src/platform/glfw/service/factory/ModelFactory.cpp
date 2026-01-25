#include <service/factory/ModelFactory.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <engine/utils/Log.hpp>

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>

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

std::optional<TextureRef> ModelFactory::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* modelPath)
{
    if (mat->GetTextureCount(type) > 0) {
        Log::Print("Loading texture of type " + std::to_string(type) + " for model: " + std::string(modelPath),
                   LogLevel::Debug);
        aiString str;
        mat->GetTexture(type, 0, &str);
        std::string texPath = str.C_Str();
        std::replace(texPath.begin(), texPath.end(), '\\', '/');
        if (texPath.size() > 0 && texPath[0] == '*') {
            // Embedded texture — must be loaded via scene->GetEmbeddedTexture
        }
        if (std::filesystem::path(texPath).is_absolute()) {
            return m_textureFactory.Texture2D(texPath.c_str());
        }
        // Relative path
        std::filesystem::path modelDir =
            std::filesystem::current_path() / std::filesystem::path(modelPath).parent_path();
        std::filesystem::path fullPath = modelDir / texPath;
        return m_textureFactory.Texture2D(fullPath.string().c_str());
    }
    return std::nullopt;
}

MaterialRef ModelFactory::processMaterial(aiMesh* mesh, const aiScene* scene, const char* modelPath)
{
    PBRMaterialParameters options;
    if (mesh->mMaterialIndex >= 0) {
        // TODO: Cache materials to avoid duplicates
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (auto texture = loadMaterialTextures(material, aiTextureType_BASE_COLOR, modelPath)) {
            options.baseColorMap = *texture;
        } else if (texture = loadMaterialTextures(material, aiTextureType_DIFFUSE, modelPath)) {
            options.baseColorMap = *texture;
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_METALNESS, modelPath)) {
            options.metallicMap = *texture;
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, modelPath)) {
            options.roughnessMap = *texture;
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, modelPath)) {
            options.aoMap = *texture;
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_NORMALS, modelPath)) {
            options.normalMap = *texture;
        }
    }
    auto materialRef = m_materialFactory.PBR(options);
    return materialRef;
}

MeshRef ModelFactory::processMesh(aiMesh* mesh, const glm::mat4& nodeTransform)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex    vertex;
        glm::vec3 vector;
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.position = vector;
        vector.x        = mesh->mNormals[i].x;
        vector.y        = mesh->mNormals[i].y;
        vector.z        = mesh->mNormals[i].z;
        vertex.normal   = vector;
        if (mesh->mTangents) {
            vector.x       = mesh->mTangents[i].x;
            vector.y       = mesh->mTangents[i].y;
            vector.z       = mesh->mTangents[i].z;
            vertex.tangent = vector;
        }
        if (mesh->mBitangents) {
            vector.x         = mesh->mBitangents[i].x;
            vector.y         = mesh->mBitangents[i].y;
            vector.z         = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        // process vertex positions, normals and texture coordinates
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return m_meshFactory.Raw(vertices, indices, nodeTransform);
}

std::vector<MeshMaterialBinding> ModelFactory::processNode(aiNode* node, const aiScene* scene,
                                                           const glm::mat4& parentTransform, const char* modelPath)
{
    glm::mat4 nodeTransform = parentTransform * AiMatrixToGlmMat4(node->mTransformation);

    std::vector<MeshMaterialBinding> meshes;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh              = scene->mMeshes[node->mMeshes[i]];
        auto    processedMesh     = processMesh(mesh, nodeTransform);
        auto    processedMaterial = processMaterial(mesh, scene, modelPath);
        meshes.push_back({processedMesh, processedMaterial});
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        auto childMeshes = processNode(node->mChildren[i], scene, nodeTransform, modelPath);
        meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
    }
    return meshes;
}

ModelRef ModelFactory::LoadModel(const char* modelPath)
{
    Assimp::Importer import;
    const aiScene*   scene =
        import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Log::Print("Error while loading model: " + std::string(import.GetErrorString()), LogLevel::Error);
        return 0;
    }

    return m_modelResource.create(processNode(scene->mRootNode, scene, glm::mat4(1.0f), modelPath));
}