/* rounded_panel.h                                         -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 08 Mar 2016, 08:13:01 tquirk
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
 * This file contains the definition for a panel with rounded corners.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_ROUNDED_PANEL_H__
#define __INC_R9_ROUNDED_PANEL_H__

#include "panel.h"

namespace ui
{
    namespace element
    {
        const GLuint radius = 5;
    }

    namespace corner
    {
        const GLuint top_left = 1, top_right = 2,
            bottom_left = 4, bottom_right = 8,
            top = 3, left = 5, right = 10, bottom = 12, all = 15;
    }

    class rounded_panel : public panel
    {
      protected:
        GLuint radius_val, elem_count;
        bool radius[4];

        void set_radius(GLuint, GLuint);

        virtual void populate_buffers(void);

      public:
        rounded_panel(context *, GLuint, GLuint);
        virtual ~rounded_panel();

        virtual GLuint get(GLuint, GLuint);
        virtual void set(GLuint, GLuint, GLuint);
    };
}

#endif /* __INC_R9_ROUNDED_PANEL_H__ */
