#include "ui_defs.h"
#include "connect_glfw.h"

#include <iostream>
#include <map>

static int convert_glfw_mods(int);
void key_callback(GLFWwindow *, int, int, int, int);
void char_callback(GLFWwindow *, unsigned int, int);
void mouse_position_callback(GLFWwindow *, double, double);
void mouse_button_callback(GLFWwindow *, int, int, int);

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
    int btn, act;

    btn = glfw_mouse_button_map[button];
    act = glfw_mouse_action_map[action];

    context->mouse_btn_callback(btn, act);
}
