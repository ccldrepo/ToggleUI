#pragma once

#include <PCH.h>

class Application
{
public:
    static Application* GetSingleton();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static void ResetUI();
    static void ToggleUI();

private:
    Application() = default;
};
