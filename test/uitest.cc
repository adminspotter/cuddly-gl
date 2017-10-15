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
#include "widget.h"
#include "label.h"
#include "button.h"
#include "text_field.h"
#include "password.h"
#include "manager.h"
#include "row_column.h"
#include "pie_menu.h"
#include "multi_label.h"

#include "connect_glfw.h"

void error_callback(int, const char *);
void window_size_callback(GLFWwindow *w, int, int);
void create_image(int, int);
void close_key_callback(ui::active *, void *, void *);
void enter_callback(ui::active *, void *, void *);
void leave_callback(ui::active *, void *, void *);
void print_sizes(ui::active *, void *, void *);

ui::context *ctx;
ui::widget *w1;
ui::label *l1;
ui::button *b1, *b2;
ui::text_field *t1;
ui::password *pw1;
ui::manager *m1;
ui::row_column *r1;
ui::pie_menu *pu1;
ui::multi_label *ml1;

std::string font_name("techover.ttf"), greeting("Howdy!");
std::string multiline("Hi this is     adm1nspotter, and I like    cheese!\nHowdy!\nabcdefghijklmnopqrstuvwxyz");
std::vector<std::string> paths =
{
    ".",
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
    std::cout << "creating image" << std::endl;
    create_image(wid, hei);

    std::cout << "creating context" << std::endl;
    ctx = new ui::context(800, 600);
    ui_connect_glfw(ctx, w);
    ctx->add_callback(ui::callback::key_down, close_key_callback, w);

    std::cout << "creating widget 1" << std::endl;
    w1 = new ui::widget(ctx, 50, 50);
    xpos = 50;
    ypos = 50;
    border = 2;
    w1->set_va(ui::element::color, ui::color::foreground, &fg1,
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
    xpos = 250;
    ypos = 35;
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
    xpos = 600;
    ypos = 35;
    border = 1;
    gridx = 1;
    gridy = 0;
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
        ui::multi_label *l = new ui::multi_label(r1, 0, 0);

        s << "Label " << q << "\n" << greeting;
        std::string str = s.str();
        border = 1;
        wid = 100;
        l->set_va(ui::element::font, ui::ownership::shared, std_font,
                  ui::element::string, 0, &str,
                  ui::element::border, ui::side::all, &border,
                  ui::element::size, ui::size::width, &wid, 0);
        l->add_callback(ui::callback::btn_down, print_sizes, NULL);
    }
    std::cout << "creating popup 1" << std::endl;
    pu1 = new ui::pie_menu(ctx, 200, 125);
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

void close_key_callback(ui::active *a, void *call, void *client)
{
    ui::key_call_data *call_data = (ui::key_call_data *)call;
    GLFWwindow *w = (GLFWwindow *)client;

    if (call_data->key == ui::key::esc && call_data->state == ui::key::down)
        glfwSetWindowShouldClose(w, GL_TRUE);
}

/* ARGSUSED */
void enter_callback(ui::active *a, void *client, void *call)
{
    std::cout << "we're in!" << std::endl;
}

/* ARGSUSED */
void leave_callback(ui::active *a, void *client, void *call)
{
    std::cout << "out, baby!" << std::endl;
}

/* ARGSUSED */
void print_sizes(ui::active *a, void *client, void *call)
{
    glm::ivec2 pos, size;
    GLuint border[4], margin[4];
    ui::widget *w = dynamic_cast<ui::widget *>(a);

    if (w == NULL)
        return;

    w->get_va(ui::element::position, ui::position::all, &pos,
              ui::element::size, ui::size::all, &size,
              ui::element::border, ui::side::top, &border[0],
              ui::element::border, ui::side::left, &border[1],
              ui::element::border, ui::side::right, &border[2],
              ui::element::border, ui::side::bottom, &border[3],
              ui::element::margin, ui::side::top, &margin[0],
              ui::element::margin, ui::side::left, &margin[1],
              ui::element::margin, ui::side::right, &margin[2],
              ui::element::margin, ui::side::bottom, &margin[3], 0);
    std::cout << "pos <" << pos.x << ", " << pos.y << ">" << std::endl;
    std::cout << "size <" << size.x << ", " << size.y << ">" << std::endl;
    std::cout << "border <" << border[0] << ", " << border[1] << ", "
              << border[2] << ", " << border[3] << ">" << std::endl;
    std::cout << "margin <" << margin[0] << ", " << margin[1] << ", "
              << margin[2] << ", " << margin[3] << ">" << std::endl;
}
