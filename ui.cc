#include <algorithm>

#include "ui.h"
#include "panel.h"

ui::context::context(GLuint w, GLuint h)
    : children()
{
    this->width = w;
    this->height = h;

    this->vert_shader = load_shader(GL_VERTEX_SHADER,
                                    SHADER_SRC_PATH "/ui_vertex.glsl");
    this->frag_shader = load_shader(GL_FRAGMENT_SHADER,
                                    SHADER_SRC_PATH "/ui_fragment.glsl");
    this->shader_pgm = create_program(vert_shader, 0, frag_shader, "color");
    this->pos_attr = glGetAttribLocation(shader_pgm, "position");
    this->norm_attr = glGetAttribLocation(shader_pgm, "normal");
    this->color_attr = glGetAttribLocation(shader_pgm, "color");
}

ui::context::~context()
{
    glDeleteShader(this->vert_shader);
    glDeleteShader(this->frag_shader);
    glUseProgram(0);
    glDeleteProgram(this->shader_pgm);
}

GLuint ui::context::get(GLuint e, GLuint t)
{
    switch (e)
    {
      case ui::element::size:
        switch (t)
        {
          case ui::size::width:  return this->width;
          case ui::size::height: return this->height;
          default:               return -1;
        }

      case ui::element::attribute:
        switch (t)
        {
          case ui::attribute::position: return this->pos_attr;
          case ui::attribute::normal:   return this->norm_attr;
          case ui::attribute::color:    return this->color_attr;
          default:                      return -1;
        }
    }
    return -1;
}

void ui::context::draw(void)
{
    glUseProgram(this->shader_pgm);
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        (*i)->draw();
}

ui::context& ui::context::operator+=(ui::panel *p)
{
    auto found = std::find(this->children.begin(), this->children.end(), p);
    if (found == this->children.end())
        this->children.push_back(p);
    return *this;
}

ui::context& ui::context::operator-=(ui::panel *p)
{
    this->children.remove(p);
    return *this;
}

