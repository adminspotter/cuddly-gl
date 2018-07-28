/* pie_menu.h                                              -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Jul 2018, 08:01:58 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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
 * This file contains the declaration for the popup pie menu.  It is a
 * composite subclass which will draw a round or oval popup window,
 * chopped into several segments.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_PIE_MENU_H__
#define __INC_CUDDLY_PIE_MENU_H__

#include "manager.h"

namespace ui
{
    class pie_menu : public manager
    {
      protected:
        int popup_button;

        int get_popup(GLuint, void *) const;
        void set_popup(GLuint, const void *);
        virtual void set_resize(GLuint, const void *) override;

        static void show(active *, void *, void *);
        static void hide(active *, void *, void *);

        virtual void set_desired_size(void) override;

        virtual vertex_buffer *generate_points(void) override;

        int which_sector(glm::ivec2&);
        ui::widget *which_child(glm::ivec2&);

      public:
        pie_menu(composite *, GLuint, GLuint);
        virtual ~pie_menu();

        virtual int get(GLuint, GLuint, void *) const override;
        virtual void set(GLuint, GLuint, const void *) override;

        virtual void mouse_pos_callback(glm::ivec2&) override;
        virtual void mouse_btn_callback(ui::btn_call_data&) override;

        virtual void draw(GLuint, const glm::mat4&) override;
    };
}

#endif /* __INC_CUDDLY_PIE_MENU_H__ */
