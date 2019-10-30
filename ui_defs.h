/* ui_defs.h                                               -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 29 Oct 2019, 08:05:41 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2019  Trinity Annabelle Quirk
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
 * This file contains namespace definitions for the CuddlyGL UI
 * classes.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_UI_DEFS_H__
#define __INC_CUDDLY_UI_DEFS_H__

#include <GL/gl.h>

#include <glm/vec2.hpp>

namespace ui
{
    /* Mouse motion callback routines */
    typedef struct mouse_callback_call
    {
        glm::ivec2 location;
    }
    mouse_call_data;

    /* Button press/release callback routines */
    typedef struct btn_callback_call
    {
        glm::ivec2 location;
        GLuint button;
        GLuint state;
        GLuint mods;
    }
    btn_call_data;

    /* Key press/release callback routines */
    typedef struct key_callback_call
    {
        glm::ivec2 location;
        uint32_t character;
        GLuint key;
        GLuint state;
        GLuint mods;
    }
    key_call_data;

    /* Resize callback routines */
    typedef struct resize_callback_call
    {
        glm::ivec2 new_size;
    }
    resize_call_data;

    /* Focus callback routines */
    typedef struct focus_callback_call
    {
        bool focus;
    }
    focus_call_data;

#define GET_VA template<typename A, typename... Args>                \
               int get(GLuint e, GLuint t, A *v, Args... args) const  \
               {                                                     \
                   return this->get(e, t, v) + this->get(args...);   \
               };

#define SET_VA template<typename A, typename... Args>                \
               void set(GLuint e, GLuint t, A v, Args... args)       \
               {                                                     \
                   this->set(e, t, v);                               \
                   this->set(args...);                               \
               };

    namespace element
    {
        const GLuint size = 1, attribute = 2;
        const GLuint border = 3, margin = 4, color = 5, position = 6;
        const GLuint radius = 7;
        const GLuint font = 8, string = 9, image = 10;
        const GLuint state = 11;
        const GLuint child = 12;
        const GLuint cursor = 13, repeat = 14;
        const GLuint transform = 15, pixel_size = 16, child_spacing = 17;
        const GLuint resize = 18;
        const GLuint order = 19;
        const GLuint popup = 20;
    }

    namespace size
    {
        const GLuint width = 1, height = 2, all = 3;
        const GLuint rows = 4, columns = 5, grid = 6;
        const GLuint max_width = 7, max_height = 8, max = 9;
    }

    namespace attribute
    {
        const GLuint position = 1, color = 2, texture = 3;
        const GLuint text_bgnd = 4;
    }

    namespace side
    {
        const GLuint top = 1, left = 2, right = 4, bottom = 8, all = 15;
        const GLuint outer = 1, inner = 8;
    }

    namespace corner
    {
        const GLuint top_left = 1, top_right = 2;
        const GLuint bottom_left = 4, bottom_right = 8;
        const GLuint top = 3, left = 5, right = 10, bottom = 12, all = 15;
    }

    namespace color
    {
        const GLuint foreground = 1, background = 2;
    }

    namespace position
    {
        const GLuint x = 1, y = 2, all = 3;
    }

    namespace callback
    {
        const GLuint enter = 1, leave = 2, btn_down = 3, btn_up = 4;
        const GLuint motion = 5, key_down = 6, key_up = 7, resize = 8;
        const GLuint focus = 9;
    }

    namespace mouse
    {
        const int button0 = 0, button1 = 1, button2 = 2, button3 = 3;
        const int button4 = 4, button5 = 5, button6 = 6, button7 = 7;
        const int down = 254, up = 255;
    }

    namespace key
    {
        const int no_key = 0;
        /* Regular keys */
        const int space = 1, apostrophe = 2, comma = 3, dash = 4, period = 5;
        const int slash = 6, semicolon = 7, equal = 8, grave = 9;
        const int backslash = 10, l_bracket = 11, r_bracket = 12;
        const int key_0 = 13, key_1 = 14, key_2 = 15, key_3 = 16, key_4 = 17;
        const int key_5 = 18, key_6 = 19, key_7 = 20, key_8 = 21, key_9 = 22;
        const int a = 23, b = 24, c = 25, d = 26, e = 27, f = 28;
        const int g = 29, h = 30, i = 31, j = 32, k = 33, l = 34;
        const int m = 35, n = 36, o = 37, p = 38, q = 39, r = 40;
        const int s = 41, t = 42, u = 43, v = 44, w = 45, x = 46;
        const int y = 47, z = 48;
        /* Keypad keys */
        const int kp_0 = 49, kp_1 = 50, kp_2 = 51, kp_3 = 52, kp_4 = 53;
        const int kp_5 = 54, kp_6 = 55, kp_7 = 56, kp_8 = 57, kp_9 = 58;
        const int kp_period = 59, kp_slash = 60, kp_asterisk = 61;
        const int kp_dash = 62, kp_plus = 63;
        /* Nonprinting keys */
        const int non_printing = 64;
        const int kp_enter = 65, esc = 66, enter = 67, tab = 68, bkspc = 69;
        const int ins = 70, del = 71;
        const int l_arrow = 72, r_arrow = 73, u_arrow = 74, d_arrow = 75;
        const int pg_up = 76, pg_down = 77, home = 78, end = 79;
        const int prt_scr = 80, pause = 81;
        const int num_lk = 82, scroll_lk = 83, caps_lk = 84;
        /* Function keys */
        const int f1 = 85, f2 = 86, f3 = 87, f4 = 88, f5 = 89, f6 = 90;
        const int f7 = 91, f8 = 92, f9 = 93, f10 = 94, f11 = 95, f12 = 96;
        const int f13 = 97, f14 = 98, f15 = 99, f16 = 100, f17 = 101, f18 = 102;
        const int f19 = 103, f20 = 104, f21 = 105, f22 = 106, f23 = 107;
        const int f24 = 108;

        const int down = 254, up = 255;
    }

    namespace key_mod
    {
        const int shift = 1, ctrl = 2, alt = 4, meta = 8;
        const int super = 16, hyper = 32;
    }

    namespace cursor
    {
        const GLuint position = 1, blink = 2;
    }

    namespace repeat
    {
        const GLuint initial = 1, secondary = 2;
    }

    namespace transform
    {
        const GLuint translate = 1;
    }

    namespace resize
    {
        const GLuint none = 0, shrink = 1, grow = 2, all = 3;
    }

    namespace order
    {
        const GLuint row = 1, column = 2;
    }

    namespace ownership
    {
        const GLuint shared = 1, owned = 2;
    }

    namespace popup
    {
        const GLuint visible = 1, button = 2, menu = 3;
    }

    namespace state
    {
        const GLuint visible = 1, active = 2, armed = 3, checked = 4;
        const GLuint radio_box = 5;
    }

    namespace child
    {
        const GLuint radio = 1, focused = 2;
    }
}

#endif /* __INC_CUDDLY_UI_DEFS_H__ */
