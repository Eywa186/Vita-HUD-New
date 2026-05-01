#pragma once

#include "global.h"

namespace vitahud
{
    class Hud
    {
    public:
        static void Create();
        static void Destroy();
        static void SetVisible(bool visible);
        static void SetPosition(int position);
        static void Update(void *arg);

    private:
        static void UpdateCachedText();
        static void ApplyText();
    };
}
