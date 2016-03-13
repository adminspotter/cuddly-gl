#include <iostream>
#include <algorithm>

#define GLFW_INCLUDE_GL_3
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui.h"
#include "panel.h"

void error_callback(int, const char *);
void window_size_callback(GLFWwindow *w, int, int);
void key_callback(GLFWwindow *, int, int, int, int);
void mouse_button_callback(GLFWwindow *, int, int, int);

ui::context *ctx;
ui::panel *p;

int main(int argc, char **argv)
{
    GLFWwindow *w;
    GLuint tb, m;

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

    ctx = new ui::context(800, 600);
    p = new ui::panel(ctx, 40, 40);
    tb = 1;
    m = 0;
    p->set_va(ui::element::border, ui::side::all, &tb,
              ui::element::margin, ui::side::all, &m, 0);

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

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        GLuint new_w, new_h;
        glm::vec4 new_color;

        p->get(ui::element::size, ui::size::width, &new_w);
        new_w = std::max((new_w + 10) % 100, (GLuint)40);
        p->get(ui::element::size, ui::size::height, &new_h);
        new_h = std::max((new_h + 10) % 80, (GLuint)40);
        p->get(ui::element::color, ui::color::background,
               glm::value_ptr(new_color));
        new_color[2] += 0.1;
        if (new_color[2] > 1.0)
            new_color[2] = 0.0;
        p->set_va(ui::element::size, ui::size::width, &new_w,
                  ui::element::size, ui::size::height, &new_h,
                  ui::element::color, ui::color::background, glm::value_ptr(new_color), 0);
    }
}
