#pragma once

#include <PCH.h>

#include "Util/Singleton.h"

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    static void ResetUI();
    static void ToggleUI();

private:
    Application() = default;
};
