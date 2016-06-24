/* ui_defs.h                                               -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 24 Jun 2016, 07:18:48 tquirk
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
 * This file contains namespace definitions for the R9 ui classes.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_UI_DEFS_H__
#define __INC_R9_UI_DEFS_H__

namespace ui
{
    namespace element
    {
        const GLuint size = 1, attribute = 2;
        const GLuint border = 3, margin = 4, color = 5, position = 6;
        const GLuint radius = 7;
        const GLuint font = 8, string = 9, bgimage = 10;
    }

    namespace size
    {
        const GLuint width = 1, height = 2;
    }

    namespace attribute
    {
        const GLuint position = 1, normal = 2, color = 3, texture = 4;
        const GLuint use_text = 5;
    }

    namespace side
    {
        const GLuint top = 1, left = 2, right = 4, bottom = 8, all = 15;
    }

    namespace corner
    {
        const GLuint top_left = 1, top_right = 2,
            bottom_left = 4, bottom_right = 8,
            top = 3, left = 5, right = 10, bottom = 12, all = 15;
    }

    namespace color
    {
        const GLuint foreground = 1, background = 2;
    }

    namespace position
    {
        const GLuint x = 1, y = 2;
    }

    namespace callback
    {
        const GLuint enter = 1, leave = 2, down = 3, up = 4, motion = 5;
    }

    namespace cursor
    {
        const int button0 = 0, button1 = 1, button2 = 2, button3 = 3;
        const int button4 = 4, button5 = 5, button6 = 6, button7 = 7;
        const int down = 254, up = 255;
    }
}

#endif /* __INC_R9_UI_DEFS_H__ */
