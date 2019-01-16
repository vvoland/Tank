#include "tank_app.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <string>
#include <cstdio>
#include "input.hpp"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

static App* APP = nullptr;
static bool CursorMode = false;
static int CursorModeKey = GLFW_KEY_X;
static double CursorModeOldX, CursorModeOldY;

static void keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods)
{
    KeyInputType type;

    if(action == GLFW_PRESS)
    {
        type = KeyInputType::Down;
        if(key == CursorModeKey)
        {
            CursorMode = !CursorMode;
            if(CursorMode)
            {
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwGetCursorPos(win, &CursorModeOldX, &CursorModeOldY);
            }
            else
            {
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPos(win, CursorModeOldX, CursorModeOldY);
            }
        }

    }
    else if(action == GLFW_RELEASE)
    {
        type = KeyInputType::Up;
    }
    else
        return;

    APP->onInput(type, key);

}

static void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(CursorMode)
        return;

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    APP->onMouse(xpos / display_w, ypos / display_h);
}

int main(int, char**)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(768, 768, "Tank", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    ImGui_ImplGlfwGL3_Init(window, true);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int oldWidth, oldHeight;
    glfwGetFramebufferSize(window, &oldWidth, &oldHeight);

    {
        TankApp app;
        APP = &app;
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, cursorCallback);

        double lastTime = glfwGetTime();
        app.onInit();
        app.onViewportChange(oldWidth, oldHeight);

        while (!glfwWindowShouldClose(window))
        {
            double nowTime = glfwGetTime();
            double deltaTime = nowTime - lastTime;

            lastTime = nowTime;

            app.DeltaTime = static_cast<float>(deltaTime);

            ImGui_ImplGlfwGL3_NewFrame();
            app.onGui();

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            if(width != oldWidth || height != oldHeight)
                app.onViewportChange(width, height);
            oldWidth = width;
            oldHeight = height;

            glClearColor(0x0 / 255.0f, 0x74 / 255.0f, 0xB7 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            app.onDraw();
            ImGui::Render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
