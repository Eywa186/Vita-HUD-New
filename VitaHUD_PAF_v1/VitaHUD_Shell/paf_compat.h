#pragma once

/*
 * VitaHUD PAF compatibility shim
 * --------------------------------
 * This lets the Phase 1 skeleton compile in a normal VitaSDK GitHub Actions
 * container that does not provide Sony/PAF headers.
 *
 * IMPORTANT:
 * This is NOT the real PAF implementation.
 * It only provides empty class/function stubs so we can keep building the
 * project structure until real PAF headers/RCO wiring are added.
 */

#include <wchar.h>

namespace paf
{
    typedef const wchar_t *wstring;

    namespace math
    {
        class v4
        {
        public:
            float x;
            float y;
            float z;
            float w;

            v4(float value = 0.0f)
            {
                x = value;
                y = value;
                z = value;
                w = value;
            }

            void set_x(float value)
            {
                x = value;
            }

            void set_y(float value)
            {
                y = value;
            }
        };
    }

    namespace common
    {
        namespace transition
        {
            enum Type
            {
                Type_Fadein1 = 0,
                Type_Fadeout1 = 1
            };
        }
    }

    namespace ui
    {
        class Plane
        {
        public:
            void Show(common::transition::Type type)
            {
                (void)type;
            }

            void Hide(common::transition::Type type)
            {
                (void)type;
            }

            void SetPos(math::v4 pos)
            {
                (void)pos;
            }
        };

        class Text : public Plane
        {
        public:
            void SetString(wstring text)
            {
                (void)text;
            }
        };
    }

    class Plugin
    {
    public:
        class PageOpenParam
        {
        public:
            int overwrite_draw_priority;

            PageOpenParam()
            {
                overwrite_draw_priority = 0;
            }
        };

        class PageCloseParam
        {
        public:
            bool fade;

            PageCloseParam()
            {
                fade = false;
            }
        };

        class TemplateOpenParam
        {
        public:
            TemplateOpenParam()
            {
            }
        };
    };
}
