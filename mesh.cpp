#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <string>

Mesh::Mesh()
{

}

Mesh::Mesh(Mesh&& other) noexcept
{
    Vertices = std::move(other.Vertices);
    Normals = std::move(other.Normals);
    Uvs = std::move(other.Uvs);
    Indices = std::move(other.Indices);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    std::swap(Vertices, other.Vertices);
    std::swap(Normals, other.Normals);
    std::swap(Uvs, other.Uvs);
    std::swap(Indices, other.Indices);
    return *this;
}

Mesh::~Mesh()
{
}

static glm::vec3 aiVecToGlm(aiVector3D vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

static glm::vec2 aiVec3ToGlm2(aiVector3D vec)
{
    return glm::vec2(vec.x, vec.y);
}

void Mesh::loadFromFile(const std::string& path)
{
    Assimp::Importer importer;
    auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals;
    const aiScene *scene = importer.ReadFile(path.c_str(), flags);

    if (scene == nullptr)
        throw MeshLoadingException(importer.GetErrorString());
    if (scene->mNumMeshes == 0)
        throw MeshLoadingException("No meshes!");
    if (scene->mNumMeshes > 1)
        throw MeshLoadingException("Loading more than 1 mesh is unsupported");

    const aiMesh *mesh = scene->mMeshes[0];

    Vertices.clear();
    Normals.clear();
    Uvs.clear();
    Indices.clear();

    Vertices.resize(mesh->mNumVertices);
    std::transform(mesh->mVertices, mesh->mVertices + mesh->mNumVertices, Vertices.begin(), aiVecToGlm);

    unsigned int lastIndicesNum = 0;
    std::for_each(mesh->mFaces, mesh->mFaces + mesh->mNumFaces, [this, &lastIndicesNum](aiFace face)
    {
        if (lastIndicesNum != 0 && lastIndicesNum != face.mNumIndices)
            throw MeshLoadingException("Inconsistent face size!");
        auto indicesPos = Indices.size();
        Indices.resize(indicesPos + face.mNumIndices);
        std::copy(face.mIndices, face.mIndices + face.mNumIndices, Indices.begin() + indicesPos);

        lastIndicesNum = face.mNumIndices;
    });

    if (mesh->HasNormals())
    {
        Normals.resize(mesh->mNumVertices);
        std::transform(mesh->mNormals, mesh->mNormals + mesh->mNumVertices, Normals.begin(), aiVecToGlm);
    }
    if (mesh->HasTextureCoords(0))
    {
        Uvs.resize(mesh->mNumVertices);
        std::transform(mesh->mTextureCoords[0], mesh->mTextureCoords[0] + mesh->mNumVertices, Uvs.begin(), aiVec3ToGlm2);
    }
}

bool Mesh::hasUvs() const
{
    return !Uvs.empty();
}

bool Mesh::hasNormals() const
{
    return !Normals.empty();
}

std::unordered_map<std::string, Mesh> Mesh::loadAllFromFile(const std::string &path)
{
    std::unordered_map<std::string, Mesh> meshes;
    Assimp::Importer importer;
    auto flags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs;
    const aiScene *scene = importer.ReadFile(path.c_str(), flags);

    if (scene == nullptr)
        throw MeshLoadingException(importer.GetErrorString());
    if (scene->mNumMeshes == 0)
        throw MeshLoadingException("No meshes!");
    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        const aiMesh *objMesh = scene->mMeshes[i];
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        vertices.resize(objMesh->mNumVertices);
        std::transform(objMesh->mVertices, objMesh->mVertices + objMesh->mNumVertices, vertices.begin(), aiVecToGlm);

        unsigned int lastIndicesNum = 0;
        std::for_each(objMesh->mFaces, objMesh->mFaces + objMesh->mNumFaces, [&indices, &lastIndicesNum](aiFace face)
        {
            if (lastIndicesNum != 0 && lastIndicesNum != face.mNumIndices)
                throw MeshLoadingException("Inconsistent face size!");
            auto indicesPos = indices.size();
            indices.resize(indicesPos + face.mNumIndices);
            std::copy(face.mIndices, face.mIndices + face.mNumIndices, indices.begin() + indicesPos);

            lastIndicesNum = face.mNumIndices;
        });

        if (objMesh->HasNormals())
        {
            normals.resize(objMesh->mNumVertices);
            std::transform(objMesh->mNormals, objMesh->mNormals + objMesh->mNumVertices, normals.begin(), aiVecToGlm);
        }
        if (objMesh->HasTextureCoords(0))
        {
            uvs.resize(objMesh->mNumVertices);
            std::transform(objMesh->mTextureCoords[0], objMesh->mTextureCoords[0] + objMesh->mNumVertices, uvs.begin(),
                           aiVec3ToGlm2);
        }

        Mesh& mesh = (meshes[objMesh->mName.C_Str()] = Mesh());
        mesh.Vertices = vertices;
        mesh.Uvs = uvs;
        mesh.Normals = normals;
        mesh.Indices = indices;
    }

    return meshes;
}
