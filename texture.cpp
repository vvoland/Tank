#include "texture.hpp"

#include <stb_image/stb_image.h>
#include <GL/glew.h>
#include <string>
#include <stdexcept>

Texture::Texture()
{
    glGenTextures(1, &Id);
}

Texture::Texture(GLuint id)
{
    Id = id;
}

Texture::Texture(Texture&& other) noexcept
{
    Id = other.Id;
    other.Id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    std::swap(Id, other.Id);
    return *this;
}

Texture::~Texture()
{
    if(Id > 0)
        glDeleteTextures(1, &Id);
}

void Texture::loadFromFile(const std::string& path, TextureFlags flags)
{
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(),
                                    &width, &height, &channels, 0);
    if(data == NULL)
        throw std::runtime_error("Failed to load texture " + path);
    loadFromMemory(data, width, height, flags, channels);
    stbi_image_free(data);
}

void Texture::loadFromMemory(unsigned char* data, int width, int height, TextureFlags flags, int channels)
{
    GLenum type = getType(channels);
    Width = width;
    Height = height;
    bind();

    GLenum wrapMode = GL_REPEAT;
    switch(flags)
    {
        case TextureFlags::MirroredRepeat:
            wrapMode = GL_MIRRORED_REPEAT;
            break;
        default:
            break;
    }

    glBindTexture(GL_TEXTURE_2D, Id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                                0, type, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind(int index) const
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, Id);
}

GLenum Texture::getType(int channels)
{
    switch(channels)
    {
        case 1:
            return GL_R;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            break;
    }

    throw std::runtime_error("Unsupported channel count");
}

