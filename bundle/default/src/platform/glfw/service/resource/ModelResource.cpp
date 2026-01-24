#include <service/resource/ModelResource.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <engine/utils/Log.hpp>

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

std::optional<TextureRef> ModelResource::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
    if (mat->GetTextureCount(type) > 0) {
        aiString str;
        mat->GetTexture(type, 0, &str);
        std::string filename = std::string(str.C_Str());
        return m_textureResource.texture2D(filename.c_str());
    }
    return std::nullopt;
}

MaterialRef ModelResource::processMaterial(aiMesh* mesh, const aiScene* scene)
{
    // TODO: Here, we only create a PBR material without any default uniforms => leaks from other materials.
    // We should move this code to the higher level (ModelFactory) to use MaterialFactory in our case.
    auto materialRef = m_materialResource.create(
        m_shaderResource.create("PBRShader", "assets/shaders/Default.vert", "assets/shaders/PBR.frag"));
    if (mesh->mMaterialIndex >= 0) {
        // TODO: Cache materials to avoid duplicates
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (auto texture = loadMaterialTextures(material, aiTextureType_BASE_COLOR)) {
            m_materialResource.setUniform(materialRef, "baseColorMap", *texture);
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_METALNESS)) {
            m_materialResource.setUniform(materialRef, "metallicMap", *texture);
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS)) {
            m_materialResource.setUniform(materialRef, "roughnessMap", *texture);
        }
        if (auto texture = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION)) {
            m_materialResource.setUniform(materialRef, "aoMap", *texture);
        }
    }
    return materialRef;
}

MeshRef ModelResource::processMesh(aiMesh* mesh, const glm::mat4& nodeTransform)
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

    return m_meshResource.create(vertices, indices, nodeTransform);
}

std::vector<ModelResource::MeshMaterialBinding> ModelResource::processNode(aiNode* node, const aiScene* scene,
                                                                           const glm::mat4& parentTransform)
{
    glm::mat4 nodeTransform = parentTransform * AiMatrixToGlmMat4(node->mTransformation);

    std::vector<MeshMaterialBinding> meshes;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh              = scene->mMeshes[node->mMeshes[i]];
        auto    processedMesh     = processMesh(mesh, nodeTransform);
        auto    processedMaterial = processMaterial(mesh, scene);
        meshes.push_back({processedMesh, processedMaterial});
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        auto childMeshes = processNode(node->mChildren[i], scene, nodeTransform);
        meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
    }
    return meshes;
}

ModelRef ModelResource::create(const char* modelPath)
{
    ModelRef         newModelRef = m_idManager.alloc();
    Assimp::Importer import;
    const aiScene*   scene =
        import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Log::Print("Error while loading model: " + std::string(import.GetErrorString()), LogLevel::Error);
        return {};
    }

    m_modelMeshes[newModelRef] = processNode(scene->mRootNode, scene, glm::mat4(1.0f));

    return newModelRef;
}

void ModelResource::forEach(ModelRef modelRef, const std::function<void(MeshRef, MaterialRef, size_t)>& func) const
{
    auto it = m_modelMeshes.find(modelRef);
    if (it == m_modelMeshes.end()) {
        Log::Print("Model not found", LogLevel::Warning, true);
        return;
    }

    for (size_t i = 0; i < it->second.size(); ++i) {
        const auto& meshMaterialBinding = it->second[i];
        func(meshMaterialBinding.meshRef, meshMaterialBinding.materialRef, i);
    }
}