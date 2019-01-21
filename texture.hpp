#pragma once

#include <GL/glew.h>

#include <string>
#include <array>

enum class TextureFlags
{
    Default = 0,
    MirroredRepeat
};

enum class TextureType
{
    Tex2D,
    Cubemap
};

class Texture
{
    public:
        Texture(TextureType type = TextureType::Tex2D);
        Texture(GLuint id, TextureType type = TextureType::Tex2D);
        Texture(Texture&& other) noexcept;
        Texture(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;
        ~Texture();

        void loadCubemap(const std::array<std::string, 6>& files);
        void loadFromFile(const std::string& path, TextureFlags flags = TextureFlags::Default);
        void loadFromMemory(unsigned char* data, int width, int height, TextureFlags flags = TextureFlags::Default, int channels = 3);
        void bind(int index = 0) const;

        const TextureType& type() const;

    private:
        GLuint Id;
        int Width, Height;
        TextureType Type;

        GLenum getType(int channels);
};
