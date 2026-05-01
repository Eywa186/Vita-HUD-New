#pragma once

#include "global.h"

namespace vitahud
{
    class Menu
    {
    public:
        static void Open();
        static void Close();
        static void HandleInput(unsigned int buttons, unsigned int pressed);
        static void Update(void *arg);

    private:
        static void RebuildText();
        static void ChangeCurrent(int direction);
    };
}
