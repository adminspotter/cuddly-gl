#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#define GLFW_INCLUDE_GL_3
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui.h"
#include "font.h"
#include "panel.h"
#include "label.h"
#include "button.h"
#include "text_field.h"
#include "password.h"
#include "manager.h"
#include "row_column.h"
#include "popupmenu.h"
#include "multi_label.h"

void error_callback(int, const char *);
void window_size_callback(GLFWwindow *w, int, int);
void mouse_position_callback(GLFWwindow *, double, double);
void mouse_button_callback(GLFWwindow *, int, int, int);
void key_callback(GLFWwindow *, int, int, int, int);
void char_callback(GLFWwindow *, unsigned int, int);
void create_image(int, int);
void enter_callback(ui::active *, void *, void *);
void leave_callback(ui::active *, void *, void *);
void clicky_callback(ui::active *, void *, void *);

ui::context *ctx;
ui::panel *p1;
ui::label *l1;
ui::button *b1, *b2;
ui::text_field *t1;
ui::password *pw1;
ui::manager *m1;
ui::row_column *r1;
ui::popupmenu *pu1;
ui::multi_label *ml1;

std::string font_name("techover.ttf"), greeting("Howdy!");
std::string multiline("Hi this is     adm1nspotter, and I like    cheese!\nHowdy!\nabcdefghijklmnopqrstuvwxyz");
std::vector<std::string> paths =
{
    "./test",
    "~/Library/Fonts",
    "/Library/Fonts",
    "/Network/Library/Fonts",
    "/System/Library/Fonts",
    "/System/Folder/Fonts",
};

unsigned char colors[6][4] =
{
    {255, 0, 0, 255},
    {255, 153, 0, 255},
    {255, 255, 0, 255},
    {0, 255, 0, 255},
    {0, 0, 255, 255},
    {128, 0, 128, 255},
};
ui::image img;

int main(int argc, char **argv)
{
    GLFWwindow *w;
    GLuint border = 1, wid = 72, hei = 48, xpos, ypos, max_len, spacing;
    GLuint gridx, gridy;
    glm::vec4 fg1 = {1.0, 1.0, 1.0, 1.0}, fg2 = {0.0, 1.0, 1.0, 1.0};
    glm::vec4 bg1 = {0.2, 0.2, 0.2, 1.0}, bg2 = {0.2, 0.2, 0.2, 0.2};
    ui::font *std_font = new ui::font(font_name, 30, paths);
    ui::font *tiny_font = new ui::font(font_name, 15, paths);
    int button;

    if (glfwInit() == GL_FALSE)
    {
        std::cout << "failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if ((w = glfwCreateWindow(800, 600, "ui-test", NULL, NULL)) == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(w);
    glfwSetWindowSizeCallback(w, window_size_callback);
    glfwSetKeyCallback(w, key_callback);
    glfwSetCharModsCallback(w, char_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);
    glfwSetCursorPosCallback(w, mouse_position_callback);

    std::cout << "creating image" << std::endl;
    create_image(wid, hei);

    std::cout << "creating context" << std::endl;
    ctx = new ui::context(800, 600);
    std::cout << "creating panel 1" << std::endl;
    p1 = new ui::panel(ctx, 50, 50);
    xpos = 50;
    ypos = 50;
    border = 2;
    p1->set_va(ui::element::color, ui::color::foreground, &fg1,
               ui::element::color, ui::color::background, &fg2,
               ui::element::border, ui::side::all, &border,
               ui::element::margin, ui::side::all, &border,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos, 0);
    std::cout << "creating label 1" << std::endl;
    l1 = new ui::label(ctx, 0, 0);
    xpos = 50;
    ypos = 125;
    border = 1;
    l1->set_va(ui::element::font, ui::ownership::shared, std_font,
               ui::element::string, 0, &greeting,
               ui::element::color, ui::color::foreground, &fg1,
               ui::element::border, ui::side::all, &border,
               ui::element::margin, ui::side::all, &border,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos, 0);
    std::cout << "creating button 1" << std::endl;
    b1 = new ui::button(ctx, 0, 0);
    xpos = 50;
    ypos = 175;
    border = 0;
    b1->set_va(ui::element::image, 0, &img,
               ui::element::margin, ui::side::all, &border,
               ui::element::border, ui::side::all, &border,
               ui::element::color, ui::color::foreground, &fg1,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos, 0);
    std::cout << "creating password 1" << std::endl;
    pw1 = new ui::password(ctx, 0, 0);
    xpos = 50;
    ypos = 250;
    border = 1;
    max_len = 5;
    pw1->set_va(ui::element::font, ui::ownership::shared, std_font,
                ui::element::border, ui::side::all, &border,
                ui::element::color, ui::color::foreground, &fg1,
                ui::element::color, ui::color::background, &bg2,
                ui::element::position, ui::position::x, &xpos,
                ui::element::position, ui::position::y, &ypos,
                ui::element::size, ui::size::max_width, &max_len, 0);
    std::cout << "creating manager 1" << std::endl;
    m1 = new ui::manager(ctx, 200, 200);
    xpos = 350;
    ypos = 150;
    border = 1;
    spacing = 10;
    m1->set_va(ui::element::border, ui::side::all, &border,
               ui::element::color, ui::color::foreground, &fg1,
               ui::element::color, ui::color::background, &bg2,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos,
               ui::element::child_spacing, ui::size::width, &spacing,
               ui::element::child_spacing, ui::size::height, &spacing, 0);
    std::cout << "creating button 2" << std::endl;
    b2 = new ui::button(m1, 0, 0);
    xpos = 10;
    ypos = 10;
    border = 5;
    b2->set_va(ui::element::font, ui::ownership::shared, std_font,
               ui::element::string, 0, &greeting,
               ui::element::margin, ui::side::all, &border,
               ui::element::border, ui::side::all, &border,
               ui::element::color, ui::color::foreground, &fg2,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos, 0);
    std::cout << "creating text field 1" << std::endl;
    t1 = new ui::text_field(m1, 0, 0);
    xpos = 10;
    ypos = 100;
    border = 1;
    max_len = 10;
    t1->set_va(ui::element::font, ui::ownership::shared, std_font,
               ui::element::string, 0, &greeting,
               ui::element::size, ui::size::max_width, &max_len,
               ui::element::border, ui::side::all, &border,
               ui::element::color, ui::color::foreground, &fg1,
               ui::element::color, ui::color::background, &bg1,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos, 0);
    std::cout << "creating row-column 1" << std::endl;
    r1 = new ui::row_column(ctx, 10, 10);
    xpos = 350;
    ypos = 350;
    border = 1;
    gridx = 2;
    gridy = 3;
    spacing = 10;
    r1->set_va(ui::element::border, ui::side::all, &border,
               ui::element::size, ui::size::columns, &gridx,
               ui::element::size, ui::size::rows, &gridy,
               ui::element::color, ui::color::foreground, &fg1,
               ui::element::color, ui::color::background, &bg1,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos,
               ui::element::child_spacing, ui::size::width, &spacing,
               ui::element::child_spacing, ui::size::height, &spacing, 0);
    r1->add_callback(ui::callback::enter, enter_callback, NULL);
    r1->add_callback(ui::callback::leave, leave_callback, NULL);
    for (int q = 0; q < 7; ++q)
    {
        std::cout << "  creating child " << q << std::endl;
        std::ostringstream s;
        ui::button *b = new ui::button(r1, 0, 0);

        s << "Button " << q;
        std::string str = s.str();
        border = 1;
        b->set_va(ui::element::font, ui::ownership::shared, std_font,
                  ui::element::string, 0, &str,
                  ui::element::border, ui::side::all, &border, 0);
        b->add_callback(ui::callback::enter, enter_callback, NULL);
        b->add_callback(ui::callback::leave, leave_callback, NULL);
    }
    std::cout << "creating popup 1" << std::endl;
    pu1 = new ui::popupmenu(ctx, 200, 125);
    border = 1;
    button = ui::mouse::button0;
    pu1->set_va(ui::element::border, ui::side::outer, &border,
                ui::element::margin, ui::side::outer, &border,
                ui::element::border, ui::side::inner, &border,
                ui::element::popup, ui::popup::button, &button, 0);
    for (int q = 0; q < 7; ++q)
    {
        std::cout << "  creating child " << q << std::endl;
        std::ostringstream s;
        ui::label *pul = new ui::label(pu1, 0, 0);

        s << (char)('a' + q);
        std::string str = s.str();
        pul->set_va(ui::element::font, ui::ownership::shared, tiny_font,
                    ui::element::string, 0, &str, 0);
        pul->add_callback(ui::callback::enter, enter_callback, NULL);
        pul->add_callback(ui::callback::leave, leave_callback, NULL);
    }
    std::cout << "creating multi_label" << std::endl;
    ml1 = new ui::multi_label(ctx, 200, 10);
    xpos = 50;
    ypos = 300;
    border = 1;
    ml1->set_va(ui::element::font, ui::ownership::shared, std_font,
                ui::element::string, 0, &multiline,
                ui::element::color, ui::color::foreground, &fg1,
                ui::element::border, ui::side::all, &border,
                ui::element::margin, ui::side::all, &border,
                ui::element::position, ui::position::x, &xpos,
                ui::element::position, ui::position::y, &ypos, 0);
    std::cout << "done creating things" << std::endl;

    while (!glfwWindowShouldClose(w))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ctx->draw();
        glfwSwapBuffers(w);
        glfwPollEvents();
    }
    delete ctx;
    delete tiny_font;
    delete std_font;
    glfwTerminate();
    return 0;
}

void error_callback(int err, const char *desc)
{
    std::cout << "glfw error: " << desc << " (" << err << ')' << std::endl;
}

void window_size_callback(GLFWwindow *w, int width, int height)
{
    glm::ivec2 sz(width, height);

    ctx->set(ui::element::size, ui::size::all, &sz);
}

void key_callback(GLFWwindow *w, int key, int scan, int action, int mods)
{
    int ui_key = 0, ui_state, ui_mods = 0;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(w, GL_TRUE);
    switch (key)
    {
      case GLFW_KEY_LEFT:       ui_key = ui::key::l_arrow;  break;
      case GLFW_KEY_RIGHT:      ui_key = ui::key::r_arrow;  break;
      case GLFW_KEY_UP:         ui_key = ui::key::u_arrow;  break;
      case GLFW_KEY_DOWN:       ui_key = ui::key::d_arrow;  break;
      case GLFW_KEY_HOME:       ui_key = ui::key::home;     break;
      case GLFW_KEY_END:        ui_key = ui::key::end;      break;
      case GLFW_KEY_BACKSPACE:  ui_key = ui::key::bkspc;    break;
      case GLFW_KEY_DELETE:     ui_key = ui::key::del;      break;
      default:              return;
    }
    ui_state = (action == GLFW_PRESS ? ui::key::down : ui::key::up);
    if (mods & GLFW_MOD_SHIFT)
        ui_mods |= ui::key_mod::shift;
    if (mods & GLFW_MOD_CONTROL)
        ui_mods |= ui::key_mod::ctrl;
    if (mods & GLFW_MOD_ALT)
        ui_mods |= ui::key_mod::alt;
    if (mods & GLFW_MOD_SUPER)
        ui_mods |= ui::key_mod::super;
    ctx->key_callback(ui_key, 0, ui_state, ui_mods);
}

void char_callback(GLFWwindow *w, unsigned int c, int mods)
{
    int ui_mods = 0;

    if (mods & GLFW_MOD_SHIFT)
        ui_mods |= ui::key_mod::shift;
    if (mods & GLFW_MOD_CONTROL)
        ui_mods |= ui::key_mod::ctrl;
    if (mods & GLFW_MOD_ALT)
        ui_mods |= ui::key_mod::alt;
    if (mods & GLFW_MOD_SUPER)
        ui_mods |= ui::key_mod::super;
    ctx->key_callback(ui::key::no_key, c, ui::key::down, mods);
}

void mouse_position_callback(GLFWwindow *w, double xpos, double ypos)
{
    ctx->mouse_pos_callback((int)xpos, (int)ypos);
}

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods)
{
    int btn, act;

    switch (button)
    {
      default:
      case GLFW_MOUSE_BUTTON_1:  btn = ui::mouse::button0;  break;
      case GLFW_MOUSE_BUTTON_2:  btn = ui::mouse::button1;  break;
      case GLFW_MOUSE_BUTTON_3:  btn = ui::mouse::button2;  break;
      case GLFW_MOUSE_BUTTON_4:  btn = ui::mouse::button3;  break;
      case GLFW_MOUSE_BUTTON_5:  btn = ui::mouse::button4;  break;
      case GLFW_MOUSE_BUTTON_6:  btn = ui::mouse::button5;  break;
      case GLFW_MOUSE_BUTTON_7:  btn = ui::mouse::button6;  break;
      case GLFW_MOUSE_BUTTON_8:  btn = ui::mouse::button7;  break;
    }

    switch (action)
    {
      default:
      case GLFW_PRESS:    act = ui::mouse::down;  break;
      case GLFW_RELEASE:  act = ui::mouse::up;    break;
    }

    ctx->mouse_btn_callback(btn, act);
}

void create_image(int width, int height)
{
    img.width = width;
    img.height = height;
    img.per_pixel = 4;
    img.data = new unsigned char[width * height * img.per_pixel];
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < width; ++k)
                memcpy(&img.data[((height - 1 - (i * 8) - j) * width + k) * 4],
                       colors[i],
                       sizeof(unsigned char) * 4);
}

/* ARGSUSED */
void enter_callback(ui::active *p, void *client, void *call)
{
    std::cout << "we're in!" << std::endl;
}

/* ARGSUSED */
void leave_callback(ui::active *p, void *client, void *call)
{
    std::cout << "out, baby!" << std::endl;
}

/* ARGSUSED */
void clicky_callback(ui::active *p, void *client, void *call)
{
    std::cout << "clicky clicky!" << std::endl;
}
