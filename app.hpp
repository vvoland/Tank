#pragma once
#include "input.hpp"

class App
{
    public:
        App() {}
        virtual ~App() {}

        virtual void onInit() = 0;
        virtual void onInput(KeyInputType type, KeyCode key) = 0;
        virtual void onGui() = 0;
        virtual void onDraw() = 0;
        virtual void onMouse(double x, double y) = 0;
        virtual void onViewportChange(int width, int height) = 0;

        float DeltaTime = 0.0f;
};


