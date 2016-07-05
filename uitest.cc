#include <vector>
#include <iostream>
#include <algorithm>

#define GLFW_INCLUDE_GL_3
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui.h"
#include "font.h"
#include "label.h"

void error_callback(int, const char *);
void window_size_callback(GLFWwindow *w, int, int);
void mouse_position_callback(GLFWwindow *, double, double);
void mouse_button_callback(GLFWwindow *, int, int, int);
void key_callback(GLFWwindow *, int, int, int, int);
void create_image(int, int);
void clicky_callback(ui::panel *, void *, void *);

ui::context *ctx;
ui::label *l1, *l2;

std::string font_name("Times New Roman.ttf"), greeting("Howdy!");
std::vector<std::string> paths =
{
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
unsigned char img[72 * 48 * 4];

int main(int argc, char **argv)
{
    GLFWwindow *w;
    GLuint border = 1, wid = 72, hei = 48, xpos = 100, ypos = 100;
    glm::vec4 fg1 = {1.0, 1.0, 1.0, 1.0}, fg2 = {0.0, 1.0, 1.0, 1.0};

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
    glfwSetMouseButtonCallback(w, mouse_button_callback);
    glfwSetCursorPosCallback(w, mouse_position_callback);

    create_image(wid, hei);

    ctx = new ui::context(800, 600);
    l1 = new ui::label(ctx, 0, 0);
    l1->set_va(ui::element::size, ui::size::width, &wid,
               ui::element::size, ui::size::height, &hei,
               ui::element::bgimage, 0, img,
               ui::element::border, ui::side::all, &border,
               ui::element::color, ui::color::foreground, &fg1, 0);
    l2 = new ui::label(ctx, 0, 0);
    l2->set_va(ui::element::font, 0, new Font(font_name, 80, paths),
               ui::element::string, 0, &greeting,
               ui::element::border, ui::side::all, &border,
               ui::element::color, ui::color::foreground, &fg2,
               ui::element::position, ui::position::x, &xpos,
               ui::element::position, ui::position::y, &ypos, 0);
    l2->add_callback(ui::callback::down, clicky_callback, NULL);

    while (!glfwWindowShouldClose(w))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ctx->draw();
        glfwSwapBuffers(w);
        glfwPollEvents();
    }
    delete ctx;
    glfwTerminate();
    return 0;
}

void error_callback(int err, const char *desc)
{
    std::cout << "glfw error: " << desc << " (" << err << ')' << std::endl;
}

void window_size_callback(GLFWwindow *w, int width, int height)
{
    GLuint temp;

    temp = width;
    ctx->set(ui::element::size, ui::size::width, &temp);
    temp = height;
    ctx->set(ui::element::size, ui::size::height, &temp);
}

void key_callback(GLFWwindow *w, int key, int scan, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(w, GL_TRUE);
}

void mouse_position_callback(GLFWwindow *w, double xpos, double ypos)
{
    ctx->cursor_pos_callback((int)xpos, (int)ypos);
}

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods)
{
    int btn, act;

    switch (button)
    {
      default:
      case GLFW_MOUSE_BUTTON_1:  btn = ui::cursor::button0;  break;
      case GLFW_MOUSE_BUTTON_2:  btn = ui::cursor::button1;  break;
      case GLFW_MOUSE_BUTTON_3:  btn = ui::cursor::button2;  break;
      case GLFW_MOUSE_BUTTON_4:  btn = ui::cursor::button3;  break;
      case GLFW_MOUSE_BUTTON_5:  btn = ui::cursor::button4;  break;
      case GLFW_MOUSE_BUTTON_6:  btn = ui::cursor::button5;  break;
      case GLFW_MOUSE_BUTTON_7:  btn = ui::cursor::button6;  break;
      case GLFW_MOUSE_BUTTON_8:  btn = ui::cursor::button7;  break;
    }

    switch (action)
    {
      default:
      case GLFW_PRESS:    act = ui::cursor::down;  break;
      case GLFW_RELEASE:  act = ui::cursor::up;    break;
    }

    ctx->cursor_btn_callback(btn, act);
}

void create_image(int width, int height)
{
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < width; ++k)
                memcpy(&img[((height - 1 - (i * 8) - j) * width + k) * 4],
                       colors[i],
                       sizeof(unsigned char) * 4);
}

/* ARGSUSED */
void clicky_callback(ui::panel *p, void *client, void *call)
{
    std::cout << "clicky clicky!" << std::endl;
}
