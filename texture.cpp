#include "texture.hpp"

#include <stb_image/stb_image.h>
#include <GL/glew.h>
#include <string>
#include <stdexcept>

Texture::Texture(TextureType type)
{
    Type = type;
    glGenTextures(1, &Id);
}

Texture::Texture(GLuint id, TextureType type)
{
    Type = type;
    Id = id;
}

Texture::Texture(Texture&& other) noexcept
{
    Id = other.Id;
    Type = other.Type;
    other.Id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    std::swap(Id, other.Id);
    std::swap(Type, other.Type);
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
    GLenum target;

    switch(Type)
    {
        case TextureType::Cubemap:
            target = GL_TEXTURE_CUBE_MAP;
            break;
        default:
        case TextureType::Tex2D:
            target = GL_TEXTURE_2D;
            break;

    }
    glBindTexture(target, Id);
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

void Texture::loadCubemap(const std::array<std::string, 6>& files)
{
    unsigned int i = 0;
    Type = TextureType::Cubemap;
    for(const std::string& path : files)
    {
        int width, height, channels;
        unsigned char *data = stbi_load(path.c_str(),
                                        &width, &height, &channels, 0);
        if (data == NULL)
            throw std::runtime_error("Failed to load texture " + path);
        GLenum type = getType(channels);
        Width = width;
        Height = height;
        glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

        GLenum wrapMode = GL_CLAMP_TO_EDGE;

        glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height,
                     0, type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        stbi_image_free(data);
        ++i;
    }
}

const TextureType& Texture::type() const
{
    return Type;
}

