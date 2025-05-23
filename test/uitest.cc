#include <config.h>

#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#define GLFW_INCLUDE_GL_3
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ui.h"
#include "../font.h"
#include "../widget.h"
#include "../label.h"
#include "../button.h"
#include "../toggle.h"
#include "../text_field.h"
#include "../password.h"
#include "../manager.h"
#include "../row_column.h"
#include "../pie_menu.h"
#include "../radio.h"

#include "../connect_glfw.h"

void error_callback(int, const char *);
void create_image(int, int);
void close_key_callback(ui::active *, void *, void *);
void enter_callback(ui::active *, void *, void *);
void leave_callback(ui::active *, void *, void *);
void menu_callback(ui::active *, void *, void *);
void reorient_callback(ui::active *, void *, void *);
void print_widget_resources(ui::active *, void *, void *);
void print_button_resources(ui::active *, void *, void *);
void print_row_column_resources(ui::active *, void *, void *);
void print_radio(ui::active *, void *, void *);

ui::context *ctx;
ui::widget *w1;
ui::label *l1;
ui::button *b1, *b2;
ui::toggle *tgl1;
ui::text_field *t1;
ui::password *pw1;
ui::manager *m1;
ui::row_column *r1;
ui::radio_box<ui::row_column> *rb1;
ui::pie_menu *pu1;

std::string font_name("techover.ttf"), greeting("Howdy!");
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
    glm::vec4 fg1 = {1.0, 1.0, 1.0, 1.0}, fg2 = {0.0, 1.0, 1.0, 1.0};
    glm::vec4 bg1 = {0.2, 0.2, 0.2, 1.0}, bg2 = {0.2, 0.2, 0.2, 0.2};
    glm::vec4 bg3 = {0.0, 0.0, 0.0, 1.0};
    ui::font *std_font = new ui::font(font_name, 30, paths);
    ui::font *tiny_font = new ui::font(font_name, 15, paths);
    int button;

    if (glfwInit() == GL_FALSE)
    {
        std::cout << "failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#if NEED_GL_FORWARD_COMPAT
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif /* NEED_GL_FORWARD_COMPAT */

    if ((w = glfwCreateWindow(800, 600, "ui-test", NULL, NULL)) == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(w);

    std::cout << "creating image" << std::endl;
    create_image(72, 48);

    std::cout << "creating context" << std::endl;
    ctx = new ui::context(ui::element::size, ui::size::width, 800,
                          ui::element::size, ui::size::height, 600);
    ui_connect_glfw(ctx, w);
    ctx->add_callback(ui::callback::key_down, close_key_callback, w);

    std::cout << "creating widget 1" << std::endl;
    w1 = new ui::widget(ctx,
                        ui::element::size, ui::size::width, 50,
                        ui::element::size, ui::size::height, 50,
                        ui::element::color, ui::color::foreground, fg1,
                        ui::element::color, ui::color::background, fg2,
                        ui::element::border, ui::side::all, 2,
                        ui::element::margin, ui::side::all, 2,
                        ui::element::position, ui::position::x, -50,
                        ui::element::position, ui::position::y, -50);
    std::cout << "creating label 1" << std::endl;
    l1 = new ui::label(ctx,
                       ui::element::font, ui::ownership::shared, std_font,
                       ui::element::string, 0, greeting,
                       ui::element::color, ui::color::foreground, fg1,
                       ui::element::border, ui::side::all, 1,
                       ui::element::margin, ui::side::all, 1,
                       ui::element::position, ui::position::x, 50,
                       ui::element::position, ui::position::y, 125);
    std::cout << "creating button 1" << std::endl;
    b1 = new ui::button(ctx,
                        ui::element::image, 0, img,
                        ui::element::margin, ui::side::all, 0,
                        ui::element::border, ui::side::all, 0,
                        ui::element::color, ui::color::foreground, fg1,
                        ui::element::color, ui::color::background, bg3,
                        ui::element::position, ui::position::x, 50,
                        ui::element::position, ui::position::y, 175);
    b1->add_callback(ui::callback::btn_down, print_button_resources, NULL);
    std::cout << "creating password 1" << std::endl;
    pw1 = new ui::password(ctx,
                           ui::element::font, ui::ownership::shared, std_font,
                           ui::element::border, ui::side::all, 1,
                           ui::element::color, ui::color::foreground, fg1,
                           ui::element::color, ui::color::background, bg2,
                           ui::element::position, ui::position::x, 50,
                           ui::element::position, ui::position::y, 250,
                           ui::element::size, ui::size::max_width, 5);
    std::cout << "creating toggle 1" << std::endl;
    tgl1 = new ui::toggle(ctx,
                          ui::element::font, ui::ownership::shared, std_font,
                          ui::element::string, 0, greeting,
                          ui::element::border, ui::side::all, 1,
                          ui::element::color, ui::color::foreground, fg1,
                          ui::element::color, ui::color::background, bg3,
                          ui::element::position, ui::position::x, 50,
                          ui::element::position, ui::position::y, 300);
    std::cout << "creating manager 1" << std::endl;
    m1 = new ui::manager(ctx,
                         ui::element::size, ui::size::width, 200,
                         ui::element::size, ui::size::height, 200,
                         ui::element::border, ui::side::all, 1,
                         ui::element::color, ui::color::foreground, fg1,
                         ui::element::color, ui::color::background, bg2,
                         ui::element::position, ui::position::x, 250,
                         ui::element::position, ui::position::y, 35,
                         ui::element::child_spacing, ui::size::width, 10,
                         ui::element::child_spacing, ui::size::height, 10);
    std::cout << "creating button 2" << std::endl;
    b2 = new ui::button(m1,
                        ui::element::font, ui::ownership::shared, std_font,
                        ui::element::color, ui::color::foreground, fg2,
                        ui::element::string, 0, greeting,
                        ui::element::margin, ui::side::all, 5,
                        ui::element::border, ui::side::all, 5,
                        ui::element::position, ui::position::x, 10,
                        ui::element::position, ui::position::y, 10);
    std::cout << "creating text field 1" << std::endl;
    t1 = new ui::text_field(m1,
                            ui::element::font, ui::ownership::shared, std_font,
                            ui::element::string, 0, greeting,
                            ui::element::size, ui::size::max_width, 10,
                            ui::element::border, ui::side::all, 1,
                            ui::element::color, ui::color::foreground, fg1,
                            ui::element::color, ui::color::background, bg1,
                            ui::element::position, ui::position::x, 10,
                            ui::element::position, ui::position::y, 100);
    std::cout << "creating row-column 1" << std::endl;
    r1 = new ui::row_column(ctx,
                            ui::element::border, ui::side::all, 1,
                            ui::element::size, ui::size::columns, 1,
                            ui::element::size, ui::size::rows, 0,
                            ui::element::color, ui::color::foreground, fg1,
                            ui::element::color, ui::color::background, bg1,
                            ui::element::position, ui::position::x, 520,
                            ui::element::position, ui::position::y, 35,
                            ui::element::child_spacing, ui::size::width, 10,
                            ui::element::child_spacing, ui::size::height, 10);
    r1->add_callback(ui::callback::btn_down, reorient_callback, NULL);
    r1->add_callback(ui::callback::btn_down, print_row_column_resources, NULL);
    for (int q = 0; q < 7; ++q)
    {
        std::cout << "  creating child " << q << std::endl;
        std::ostringstream s;
        ui::label *l = new ui::label(r1);

        s << "Label " << q << "\n" << greeting;
        l->set(ui::element::font, ui::ownership::shared, std_font,
               ui::element::string, 0, s.str(),
               ui::element::border, ui::side::all, 1,
               ui::element::size, ui::size::width, 100);
        l->add_callback(ui::callback::btn_down, print_widget_resources, NULL);
    }
    std::cout << "creating radio box 1" << std::endl;
    rb1 = new ui::radio_box<ui::row_column>(
        ctx,
        ui::element::position, ui::position::x, 50,
        ui::element::position, ui::position::y, 350,
        ui::element::color, ui::color::foreground, fg1,
        ui::element::color, ui::color::background, bg1,
        ui::element::size, ui::size::columns, 1,
        ui::element::size, ui::size::rows, 0,
        ui::element::child_spacing, ui::size::width, 2,
        ui::element::child_spacing, ui::size::height, 5
    );
    for (int q = 0; q < 3; ++q)
    {
        std::cout << "  creating radio " << q << std::endl;
        std::ostringstream s;
        s << "radio " << (char)('a' + q);
        ui::toggle *rbt = new ui::toggle(
            rb1,
            ui::element::font, ui::ownership::shared, std_font,
            ui::element::string, 0, s.str(),
            ui::element::color, ui::color::foreground, fg1,
            ui::element::color, ui::color::background, bg1
        );
        rbt->add_callback(ui::callback::btn_up, print_radio, rb1);
    }
    std::cout << "creating popup 1" << std::endl;
    pu1 = new ui::pie_menu(ctx,
                           ui::element::size, ui::size::width, 200,
                           ui::element::size, ui::size::height, 125,
                           ui::element::border, ui::side::outer, 1,
                           ui::element::margin, ui::side::outer, 1,
                           ui::element::border, ui::side::inner, 1,
                           ui::element::popup, ui::popup::button,
                           ui::mouse::button0);
    for (intptr_t q = 0; q < 7; ++q)
    {
        std::cout << "  creating child " << q << std::endl;
        std::ostringstream s;
        ui::label *pul = new ui::label(pu1);

        s << (char)('a' + q);
        pul->set(ui::element::font, ui::ownership::shared, tiny_font,
                 ui::element::string, 0, s.str());
        pul->add_callback(ui::callback::btn_up, menu_callback, (void *)q);
        pul->add_callback(ui::callback::enter, enter_callback, (void *)q);
        pul->add_callback(ui::callback::leave, leave_callback, (void *)q);
    }

    std::cout << "done creating things" << std::endl;

    while (!glfwWindowShouldClose(w))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ctx->draw();
        glfwSwapBuffers(w);
        glfwPollEvents();
    }
    ui_disconnect_glfw(ctx, w);
    delete ctx;
    glfwTerminate();
    return 0;
}

void error_callback(int err, const char *desc)
{
    std::cout << "glfw error: " << desc << " (" << err << ')' << std::endl;
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
void enter_callback(ui::active *a, void *call, void *client)
{
    std::cout << "we're in " << (intptr_t)client << '!' << std::endl;
}

/* ARGSUSED */
void leave_callback(ui::active *a, void *call, void *client)
{
    std::cout << "out of " << (intptr_t)client << ", baby!" << std::endl;
}

void menu_callback(ui::active *a, void *call, void *client)
{
    std::cout << "menu sector " << (intptr_t)client << std::endl;
}

void reorient_callback(ui::active *a, void *call, void *client)
{
    ui::row_column *r = dynamic_cast<ui::row_column *>(a);
    ui::btn_call_data *call_data = (ui::btn_call_data *)call;

    if (r != NULL && call_data->button == ui::mouse::button0)
    {
        static glm::ivec2 grid(2, 4);
        static GLuint orient = ui::order::column;
        glm::ivec2 old_grid;
        GLuint old_orient;

        r->get(ui::element::size, ui::size::grid, &old_grid,
               ui::element::order, 0, &old_orient);
        r->set(ui::element::size, ui::size::grid, grid,
               ui::element::order, 0, orient);
        r->manage_children();
        grid = old_grid;
        orient = old_orient;
    }
}

/* ARGSUSED */
void print_widget_resources(ui::active *a, void *call, void *client)
{
    glm::ivec2 pos, size;
    glm::vec4 fg, bg;
    GLuint border[4], margin[4];
    bool visible;
    ui::widget *w = dynamic_cast<ui::widget *>(a);

    if (w == NULL)
        return;

    w->get(ui::element::position, ui::position::all, &pos,
           ui::element::size, ui::size::all, &size,
           ui::element::border, ui::side::top, &border[0],
           ui::element::border, ui::side::left, &border[1],
           ui::element::border, ui::side::right, &border[2],
           ui::element::border, ui::side::bottom, &border[3],
           ui::element::margin, ui::side::top, &margin[0],
           ui::element::margin, ui::side::left, &margin[1],
           ui::element::margin, ui::side::right, &margin[2],
           ui::element::margin, ui::side::bottom, &margin[3],
           ui::element::state, ui::state::visible, &visible,
           ui::element::color, ui::color::foreground, &fg,
           ui::element::color, ui::color::background, &bg);
    std::cout << "pos <" << pos.x << ", " << pos.y << ">" << std::endl;
    std::cout << "size <" << size.x << ", " << size.y << ">" << std::endl;
    std::cout << "border <" << border[0] << ", " << border[1] << ", "
              << border[2] << ", " << border[3] << ">" << std::endl;
    std::cout << "margin <" << margin[0] << ", " << margin[1] << ", "
              << margin[2] << ", " << margin[3] << ">" << std::endl;
    std::cout << "foreground <" << fg.r << ", " << fg.g << ", "
              << fg.b << ", " << fg.a << ">" << std::endl;
    std::cout << "background <" << bg.r << ", " << bg.g << ", "
              << bg.b << ", " << bg.a << ">" << std::endl;
    if (visible == true)
        std::cout << "visible" << std::endl;
}

void print_button_resources(ui::active *a, void *call, void *client)
{
    GLuint w, h;
    int x, y;
    ui::base_font *font;
    std::string str;
    ui::image img;
    bool arm, active, visible;
    ui::button *b = dynamic_cast<ui::button *>(a);

    if (b == NULL)
        return;

    b->get(ui::element::position, ui::position::x, &x,
           ui::element::position, ui::position::y, &y,
           ui::element::size, ui::size::width, &w,
           ui::element::size, ui::size::height, &h,
           ui::element::string, 0, &str,
           ui::element::font, 0, &font,
           ui::element::image, 0, &img,
           ui::element::state, ui::state::visible, &visible,
           ui::element::state, ui::state::armed, &arm,
           ui::element::state, ui::state::active, &active);
    std::cout << "pos <" << x << ", " << y << ">" << std::endl;
    std::cout << "size <" << w << ", " << h << ">" << std::endl;
    std::cout << "string [" << str << "]" << std::endl;
    if (arm == true)
        std::cout << "armed" << std::endl;
    if (active == true)
        std::cout << "active" << std::endl;
    if (visible == true)
        std::cout << "visible" << std::endl;
}

void print_row_column_resources(ui::active *a, void *call, void *client)
{
    GLuint sx, sy, cx, cy, resize;
    float px, py;
    ui::row_column *r = dynamic_cast<ui::row_column *>(a);

    if (r == NULL)
        return;

    r->get(ui::element::size, ui::size::rows, &sx,
           ui::element::size, ui::size::columns, &sy,
           ui::element::child_spacing, ui::size::width, &cx,
           ui::element::child_spacing, ui::size::height, &cy,
           ui::element::resize, 0, &resize,
           ui::element::pixel_size, ui::size::width, &px,
           ui::element::pixel_size, ui::size::height, &py);
    std::cout << "grid size <" << sx << ", " << sy << ">" << std::endl;
    std::cout << "spacing <" << cx << ", " << cy << ">" << std::endl;
    std::cout << "pixel <" << px << ", " << py << ">" << std::endl;
    switch (resize)
    {
      case ui::resize::none:    std::cout << "none" << std::endl;    break;
      case ui::resize::shrink:  std::cout << "shrink" << std::endl;  break;
      case ui::resize::grow:    std::cout << "grow" << std::endl;    break;
      case ui::resize::all:     std::cout << "all" << std::endl;     break;
      default:                  std::cout << "??" << std::endl;      break;
    }
}

void print_radio(ui::active *a, void *call, void *client)
{
    ui::widget *w = NULL;
    ui::radio_box<ui::row_column> *r
        = dynamic_cast<ui::radio_box<ui::row_column> *>(
            reinterpret_cast<ui::widget *>(client)
        );

    if (r != NULL)
    {
        r->get(ui::element::child, ui::child::radio, &w);
        std::cout << "is a radio box, "
                  << std::hex << w << std::dec << " is the checked child"
                  << std::endl;
    }
    else
        std::cout << "is not a radio box" << std::endl;
}
