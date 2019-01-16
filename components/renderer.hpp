#pragma once

#include "component.hpp"
#include "draw_call.hpp"

class Renderer : public Component
{
    public:
        explicit Renderer(DrawCall drawCall);

        DrawCall Call;
};

template<>
struct ComponentTrait<Renderer>
{
    static int constexpr Index = 1;
};