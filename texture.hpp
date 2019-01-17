#pragma once

#include <GL/glew.h>

#include <string>

enum class TextureFlags
{
    Default = 0,
    MirroredRepeat
};

class Texture
{
    public:
        Texture();
        Texture(GLuint id);
        Texture(Texture&& other) noexcept;
        Texture(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;
        ~Texture();

        void loadFromFile(const std::string& path, TextureFlags flags = TextureFlags::Default);
        void loadFromMemory(unsigned char* data, int width, int height, TextureFlags flags = TextureFlags::Default, int channels = 3);
        void bind(int index = 0) const;

    private:
        GLuint Id;
        int Width, Height;

        GLenum getType(int channels);
};
