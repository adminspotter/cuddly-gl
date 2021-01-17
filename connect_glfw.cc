/* connect_glfw.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 17 Jan 2021, 10:31:18 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2021  Trinity Annabelle Quirk
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

#include "connect_glfw.h"

#include <map>

#if USING_X11
#include <X11/Xlib.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

static int global_auto_repeat = 0;
#endif /* USING X11 */

#include "text_field.h"

static int convert_glfw_mods(int);

void key_callback(GLFWwindow *, int, int, int, int);
void text_field_focus_hook(bool);
static GLFWwindow *focus_window = NULL;
static bool text_field_focus = false;
void char_callback(GLFWwindow *, unsigned int, int);

void mouse_position_callback(GLFWwindow *, double, double);
void mouse_button_callback(GLFWwindow *, int, int, int);
void window_size_callback(GLFWwindow *, int, int);
void focus_callback(GLFWwindow *, int);
void close_callback(GLFWwindow *);

static std::map<int, int> glfw_key_map =
{
    { GLFW_KEY_SPACE, ui::key::space },
    { GLFW_KEY_APOSTROPHE, ui::key::apostrophe },
    { GLFW_KEY_COMMA, ui::key::comma },
    { GLFW_KEY_MINUS, ui::key::dash },
    { GLFW_KEY_PERIOD, ui::key::period },
    { GLFW_KEY_SLASH, ui::key::slash },
    { GLFW_KEY_SEMICOLON, ui::key::semicolon },
    { GLFW_KEY_EQUAL, ui::key::equal },
    { GLFW_KEY_GRAVE_ACCENT, ui::key::grave },
    { GLFW_KEY_BACKSLASH, ui::key::backslash },
    { GLFW_KEY_LEFT_BRACKET, ui::key::l_bracket },
    { GLFW_KEY_RIGHT_BRACKET, ui::key::r_bracket },
    { GLFW_KEY_0, ui::key::key_0 },
    { GLFW_KEY_1, ui::key::key_1 },
    { GLFW_KEY_2, ui::key::key_2 },
    { GLFW_KEY_3, ui::key::key_3 },
    { GLFW_KEY_4, ui::key::key_4 },
    { GLFW_KEY_5, ui::key::key_5 },
    { GLFW_KEY_6, ui::key::key_6 },
    { GLFW_KEY_7, ui::key::key_7 },
    { GLFW_KEY_8, ui::key::key_8 },
    { GLFW_KEY_9, ui::key::key_9 },
    { GLFW_KEY_A, ui::key::a },
    { GLFW_KEY_B, ui::key::b },
    { GLFW_KEY_C, ui::key::c },
    { GLFW_KEY_D, ui::key::d },
    { GLFW_KEY_E, ui::key::e },
    { GLFW_KEY_F, ui::key::f },
    { GLFW_KEY_G, ui::key::g },
    { GLFW_KEY_H, ui::key::h },
    { GLFW_KEY_I, ui::key::i },
    { GLFW_KEY_J, ui::key::j },
    { GLFW_KEY_K, ui::key::k },
    { GLFW_KEY_L, ui::key::l },
    { GLFW_KEY_M, ui::key::m },
    { GLFW_KEY_N, ui::key::n },
    { GLFW_KEY_O, ui::key::o },
    { GLFW_KEY_P, ui::key::p },
    { GLFW_KEY_Q, ui::key::q },
    { GLFW_KEY_R, ui::key::r },
    { GLFW_KEY_S, ui::key::s },
    { GLFW_KEY_T, ui::key::t },
    { GLFW_KEY_U, ui::key::u },
    { GLFW_KEY_V, ui::key::v },
    { GLFW_KEY_W, ui::key::w },
    { GLFW_KEY_X, ui::key::x },
    { GLFW_KEY_Y, ui::key::y },
    { GLFW_KEY_Z, ui::key::z },
    { GLFW_KEY_KP_0, ui::key::kp_0 },
    { GLFW_KEY_KP_1, ui::key::kp_1 },
    { GLFW_KEY_KP_2, ui::key::kp_2 },
    { GLFW_KEY_KP_3, ui::key::kp_3 },
    { GLFW_KEY_KP_4, ui::key::kp_4 },
    { GLFW_KEY_KP_5, ui::key::kp_5 },
    { GLFW_KEY_KP_6, ui::key::kp_6 },
    { GLFW_KEY_KP_7, ui::key::kp_7 },
    { GLFW_KEY_KP_8, ui::key::kp_8 },
    { GLFW_KEY_KP_9, ui::key::kp_9 },
    { GLFW_KEY_KP_DECIMAL, ui::key::kp_period },
    { GLFW_KEY_KP_DIVIDE, ui::key::kp_slash },
    { GLFW_KEY_KP_MULTIPLY, ui::key::kp_asterisk },
    { GLFW_KEY_KP_SUBTRACT, ui::key::kp_dash },
    { GLFW_KEY_KP_ADD, ui::key::kp_plus },
    { GLFW_KEY_KP_ENTER, ui::key::kp_enter },
    { GLFW_KEY_ESCAPE, ui::key::esc },
    { GLFW_KEY_ENTER, ui::key::enter },
    { GLFW_KEY_TAB, ui::key::tab },
    { GLFW_KEY_BACKSPACE, ui::key::bkspc },
    { GLFW_KEY_INSERT, ui::key::ins },
    { GLFW_KEY_DELETE, ui::key::del },
    { GLFW_KEY_LEFT, ui::key::l_arrow },
    { GLFW_KEY_RIGHT, ui::key::r_arrow },
    { GLFW_KEY_UP, ui::key::u_arrow },
    { GLFW_KEY_DOWN, ui::key::d_arrow },
    { GLFW_KEY_PAGE_UP, ui::key::pg_up},
    { GLFW_KEY_PAGE_DOWN, ui::key::pg_down },
    { GLFW_KEY_HOME, ui::key::home },
    { GLFW_KEY_END, ui::key::end },
    { GLFW_KEY_PRINT_SCREEN, ui::key::prt_scr },
    { GLFW_KEY_PAUSE, ui::key::pause },
    { GLFW_KEY_NUM_LOCK, ui::key::num_lk },
    { GLFW_KEY_SCROLL_LOCK, ui::key::scroll_lk },
    { GLFW_KEY_CAPS_LOCK, ui::key::caps_lk },
    { GLFW_KEY_F1, ui::key::f1 },
    { GLFW_KEY_F2, ui::key::f2 },
    { GLFW_KEY_F3, ui::key::f3 },
    { GLFW_KEY_F4, ui::key::f4 },
    { GLFW_KEY_F5, ui::key::f5 },
    { GLFW_KEY_F6, ui::key::f6 },
    { GLFW_KEY_F7, ui::key::f7 },
    { GLFW_KEY_F8, ui::key::f8 },
    { GLFW_KEY_F9, ui::key::f9 },
    { GLFW_KEY_F10, ui::key::f10 },
    { GLFW_KEY_F11, ui::key::f11 },
    { GLFW_KEY_F12, ui::key::f12 },
    { GLFW_KEY_F13, ui::key::f13 },
    { GLFW_KEY_F14, ui::key::f14 },
    { GLFW_KEY_F15, ui::key::f15 },
    { GLFW_KEY_F16, ui::key::f16 },
    { GLFW_KEY_F17, ui::key::f17 },
    { GLFW_KEY_F18, ui::key::f18 },
    { GLFW_KEY_F19, ui::key::f19 },
    { GLFW_KEY_F20, ui::key::f20 },
    { GLFW_KEY_F21, ui::key::f21 },
    { GLFW_KEY_F22, ui::key::f22 },
    { GLFW_KEY_F23, ui::key::f23 },
    { GLFW_KEY_F24, ui::key::f24 },

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
#if USING_X11
    XKeyboardState kb_state;

    XGetKeyboardControl(glfwGetX11Display(), &kb_state);
    global_auto_repeat = kb_state.global_auto_repeat;
#endif /* USING_X11 */

    glfwSetKeyCallback(w, key_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);
    glfwSetCursorPosCallback(w, mouse_position_callback);
    glfwSetWindowSizeCallback(w, window_size_callback);
    glfwSetWindowFocusCallback(w, focus_callback);
    glfwSetWindowCloseCallback(w, close_callback);

    ui::text_field::focus_hook = &text_field_focus_hook;
    focus_window = w;
}

void ui_disconnect_glfw(ui::context *ctx, GLFWwindow *w)
{
    ui::text_field::focus_hook = NULL;
    close_callback(w);
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

    ui_key = glfw_key_map[key];
    ui_state = glfw_key_map[action];
    ui_mods = convert_glfw_mods(mods);

    /* Only deal with non-printing characters and key-up events here
     * when the char_callback is active.
     */
    if (text_field_focus == false
        || (ui_state == ui::key::up || ui_key > ui::key::non_printing))
        context->key_callback(ui_key, 0, ui_state, ui_mods);
}

void text_field_focus_hook(bool focus)
{
    if (focus == true)
        glfwSetCharModsCallback(focus_window, char_callback);
    else
        glfwSetCharModsCallback(focus_window, NULL);
    text_field_focus = focus;
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

void focus_callback(GLFWwindow *w, int focused)
{
#if USING_X11
    if (global_auto_repeat == 1)
    {
        if (focused)
            XAutoRepeatOff(glfwGetX11Display());
        else
            XAutoRepeatOn(glfwGetX11Display());
    }
#endif /* USING_X11 */
}

void close_callback(GLFWwindow *w)
{
#if USING_X11
    if (global_auto_repeat == 1)
        XAutoRepeatOn(glfwGetX11Display());
#endif /* USING_X11 */
}
