/* pie_menu.h                                              -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Feb 2017, 09:25:12 tquirk
 *
 * Revision IX game client
 * Copyright (C) 2016  Trinity Annabelle Quirk
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

#ifndef __INC_R9_PIE_MENU_H__
#define __INC_R9_PIE_MENU_H__

#include "manager.h"

namespace ui
{
    class pie_menu : public manager
    {
      protected:
        int popup_button;

        int get_popup(GLuint, void *);
        void set_popup(GLuint, void *);
        virtual void set_resize(GLuint, void *) override;

        static void show(active *, void *, void *);
        static void hide(active *, void *, void *);

        virtual vertex_buffer *generate_points(void) override;

      public:
        pie_menu(composite *, GLuint, GLuint);
        virtual ~pie_menu();

        virtual int get(GLuint, GLuint, void *) override;
        virtual void set(GLuint, GLuint, void *) override;

        virtual void draw(GLuint, const glm::mat4&) override;
    };
}

#endif /* __INC_R9_PIE_MENU_H__ */
