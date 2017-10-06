#include "ui_defs.h"
#include "connect_glfw.h"

#include <map>

static int convert_glfw_mods(int);

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

static std::map<int, int> glfw_mouse_map =
{
    { GLFW_MOUSE_BUTTON_1, ui::mouse::button0 },
    { GLFW_MOUSE_BUTTON_2, ui::mouse::button1 },
    { GLFW_MOUSE_BUTTON_3, ui::mouse::button2 },
    { GLFW_MOUSE_BUTTON_4, ui::mouse::button3 },
    { GLFW_MOUSE_BUTTON_5, ui::mouse::button4 },
    { GLFW_MOUSE_BUTTON_6, ui::mouse::button5 },
    { GLFW_MOUSE_BUTTON_7, ui::mouse::button6 },
    { GLFW_MOUSE_BUTTON_8, ui::mouse::button7 },
    { GLFW_PRESS, ui::mouse::down },
    { GLFW_RELEASE, ui::mouse::up }
};

static ui::context *context = NULL;

void ui_connect_glfw(ui::context *ctx, GLFWwindow *w)
{
    context = ctx;
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
