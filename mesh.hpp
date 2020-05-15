#pragma once
#include <glm/fwd.hpp>

#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <string>
#include <vector>

class MeshLoadingException : public std::runtime_error
{
    public:
        explicit MeshLoadingException(std::string message)
            : std::runtime_error(message)
        {
        }
};

class Mesh
{
    public:
        Mesh();
        Mesh(Mesh&& other) noexcept;
        Mesh(const Mesh& other) = delete;
        Mesh& operator=(Mesh&& vbo) noexcept;
        ~Mesh();

        void loadFromFile(const std::string& path);

        static std::unordered_map<std::string, Mesh> loadAllFromFile(const std::string& path);

        std::vector<glm::vec3> Vertices;
        std::vector<glm::vec2> Uvs;
        std::vector<glm::vec3> Normals;
        std::vector<unsigned int> Indices;

        bool hasUvs() const;
        bool hasNormals() const;
};

