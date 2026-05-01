#pragma once

/*
 * VitaHUD PAF compatibility shim
 * --------------------------------
 * Lets this staged project compile in the standard VitaSDK GitHub Actions
 * container, which does not include real Sony/PAF headers.
 *
 * This shim does not render a real overlay. It only models enough of the PAF
 * API shape so the project architecture can compile while we wire the real
 * PAF/RCO environment later.
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

            void set_x(float value) { x = value; }
            void set_y(float value) { y = value; }
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
            Plane *GetChild(int index)
            {
                (void)index;
                return this;
            }

            int GetChildrenNum()
            {
                return 1;
            }

            Plane *FindChild(int id)
            {
                (void)id;
                return this;
            }

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

        ui::Plane *PageOpen(const char *name, PageOpenParam param)
        {
            (void)name;
            (void)param;
            static ui::Plane page;
            return &page;
        }

        void PageClose(const char *name, PageCloseParam param)
        {
            (void)name;
            (void)param;
        }

        void TemplateOpen(ui::Plane *root, int template_id, TemplateOpenParam param)
        {
            (void)root;
            (void)template_id;
            (void)param;
        }
    };
}
