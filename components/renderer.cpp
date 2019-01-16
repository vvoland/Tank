#include "renderer.hpp"

Renderer::Renderer(DrawCall drawCall)
    : Call(std::move(drawCall))
{
}
