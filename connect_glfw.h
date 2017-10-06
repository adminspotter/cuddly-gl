#ifndef __INC_CONNECT_GLFW_H__
#define __INC_CONNECT_GLFW_H__

#ifndef GLFW_INCLUDE_GL_3
#define GLFW_INCLUDE_GL_3
#endif
#include <GLFW/glfw3.h>

#include "ui.h"

void ui_connect_glfw(ui::context *, GLFWwindow *);

#endif /* __INC_CONNECT_GLFW_H__ */
