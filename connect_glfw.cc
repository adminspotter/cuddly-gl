/* connect_glfw.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Nov 2017, 06:57:37 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2017  Trinity Annabelle Quirk
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
 * This file contains connector callbacks to be able to use the
 * CuddlyGL widget toolkit under GLFW.
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "connect_glfw.h"

#include <iostream>
#include <map>

static int convert_glfw_mods(int);
void key_callback(GLFWwindow *, int, int, int, int);
void char_callback(GLFWwindow *, unsigned int, int);
void mouse_position_callback(GLFWwindow *, double, double);
void mouse_button_callback(GLFWwindow *, int, int, int);
void window_size_callback(GLFWwindow *, int, int);

static std::map<int, int> glfw_key_map =
{
    { GLFW_KEY_LEFT, ui::key::l_arrow },
    { GLFW_KEY_RIGHT, ui::key::r_arrow },
    { GLFW_KEY_UP, ui::key::u_arrow },
    { GLFW_KEY_DOWN, ui::key::d_arrow },
    { GLFW_KEY_HOME, ui::key::home },
    { GLFW_KEY_END, ui::key::end },
    { GLFW_KEY_BACKSPACE, ui::key::bkspc },
    { GLFW_KEY_DELETE, ui::key::del },
    { GLFW_KEY_ESCAPE, ui::key::esc },
    { GLFW_PRESS, ui::key::down },
    { GLFW_RELEASE, ui::key::up }
};

static std::map<int, int> glfw_mouse_button_map =
{
    { GLFW_MOUSE_BUTTON_1, ui::mouse::button0 },
    { GLFW_MOUSE_BUTTON_2, ui::mouse::button1 },
    { GLFW_MOUSE_BUTTON_3, ui::mouse::button2 },
    { GLFW_MOUSE_BUTTON_4, ui::mouse::button3 },
    { GLFW_MOUSE_BUTTON_5, ui::mouse::button4 },
    { GLFW_MOUSE_BUTTON_6, ui::mouse::button5 },
    { GLFW_MOUSE_BUTTON_7, ui::mouse::button6 },
    { GLFW_MOUSE_BUTTON_8, ui::mouse::button7 }
};

static std::map<int, int> glfw_mouse_action_map =
{
    { GLFW_PRESS, ui::mouse::down },
    { GLFW_RELEASE, ui::mouse::up }
};

static ui::context *context = NULL;

void ui_connect_glfw(ui::context *ctx, GLFWwindow *w)
{
    context = ctx;

    glfwSetKeyCallback(w, key_callback);
    glfwSetCharModsCallback(w, char_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);
    glfwSetCursorPosCallback(w, mouse_position_callback);
    glfwSetWindowSizeCallback(w, window_size_callback);
}

int convert_glfw_mods(int mods)
{
    int retval = 0;

    if (mods & GLFW_MOD_SHIFT)
        retval |= ui::key_mod::shift;
    if (mods & GLFW_MOD_CONTROL)
        retval |= ui::key_mod::ctrl;
    if (mods & GLFW_MOD_ALT)
        retval |= ui::key_mod::alt;
    if (mods & GLFW_MOD_SUPER)
        retval |= ui::key_mod::super;
    return retval;
}

void key_callback(GLFWwindow *w, int key, int scan, int action, int mods)
{
    int ui_key = 0, ui_state, ui_mods;

    if (glfw_key_map.find(key) == glfw_key_map.end())
        return;

    ui_key = glfw_key_map[key];
    ui_state = glfw_key_map[action];
    ui_mods = convert_glfw_mods(mods);

    context->key_callback(ui_key, 0, ui_state, ui_mods);
}

void char_callback(GLFWwindow *w, unsigned int c, int mods)
{
    int ui_mods = convert_glfw_mods(mods);

    context->key_callback(ui::key::no_key, c, ui::key::down, ui_mods);
}

void mouse_position_callback(GLFWwindow *w, double xpos, double ypos)
{
    context->mouse_pos_callback((int)xpos, (int)ypos);
}

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods)
{
    int btn, act, ui_mods;

    btn = glfw_mouse_button_map[button];
    act = glfw_mouse_action_map[action];
    ui_mods = convert_glfw_mods(mods);

    context->mouse_btn_callback(btn, act, ui_mods);
}

void window_size_callback(GLFWwindow *w, int width, int height)
{
    glm::ivec2 sz(width, height);

    context->set(ui::element::size, ui::size::all, sz);
}
