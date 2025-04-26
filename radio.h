/* radio.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Apr 2025, 11:41:31 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2025  Trinity Annabelle Quirk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * This file contains a template definition which adds radio-box
 * behaviour to composite objects.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_RADIO_H__
#define __INC_CUDDLY_RADIO_H__

#include "composite.h"
#include "toggle.h"

namespace ui
{
    template <class T>
    class radio_box : public T
    {
      protected:
        int get_radio_child(widget **v) const
            {
                bool checked;
                for (auto i : this->children)
                {
                    ui::toggle *r = dynamic_cast<ui::toggle *>(i);
                    if (r == NULL)
                        continue;
                    r->get(ui::element::state, ui::state::checked, &checked);
                    if (checked == true)
                    {
                        *v = i;
                        return 0;
                    }
                }
                *v = NULL;
                return 1;
            };
        void set_radio_child(widget *v)
            {
                auto found = std::find(this->children.begin(),
                                       this->children.end(),
                                       v);
                if (found != this->children.end())
                {
                    ui::toggle *set_radio = dynamic_cast<ui::toggle *>(v);
                    for (auto i : this->children)
                    {
                        ui::toggle *r = dynamic_cast<ui::toggle *>(i);
                        if (r != NULL && r != set_radio)
                            r->set(ui::element::state,
                                   ui::state::checked,
                                   false);
                    }
                }
            };
        virtual int get_child(GLuint t, ui::widget **v) const override
            {
                if (t == ui::child::radio)
                    return this->get_radio_child(v);
                return T::get_child(t, v);
            };
        virtual void set_child(GLuint t, ui::widget *v) override
            {
                if (t == ui::child::radio)
                    this->set_radio_child(v);
                else
                    T::set_child(t, v);
            };

        static void radio_selection(ui::active *a, void *call, void *client)
            {
                ui::toggle *t = dynamic_cast<ui::toggle *>(a);

                if (t != NULL)
                {
                    ui::radio_box<T> *rb
                        = reinterpret_cast<ui::radio_box<T> *>(client);

                    rb->set(ui::element::child,
                            ui::child::radio,
                            (ui::widget *)t);
                }
            };

      public:
        explicit radio_box(composite *c)
            : rect(0, 0), active(0, 0), T(c)
            {};
        template<typename... Args>
        radio_box(composite *c, Args... args)
            : rect(0, 0), active(0, 0), T(c)
            {
                this->set(args...);
            };
        ~radio_box() {};

        virtual void add_child(ui::widget *w) override
            {
                T::add_child(w);
                w->add_callback(ui::callback::btn_down,
                                ui::radio_box<T>::radio_selection,
                                (void *)this);
            };
    };
}

#endif /* __INC_CUDDLY_RADIO_H__ */
