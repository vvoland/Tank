#pragma once

#include <GL/glew.h>

#include <string>

enum class TextureFlags
{
    Default = 0,
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

        void loadFromFile(const std::string& path);
        void loadFromMemory(unsigned char* data, int width, int height, int channels = 3, TextureFlags flags = TextureFlags::Default);
        void bind(int index = 0) const;

    private:
        GLuint Id;
        int Width, Height;

        GLenum getType(int channels);
};
