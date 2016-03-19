#include "label.h"

int ui::label::get_font(GLuint t, void *v)
{
}

void ui::label::set_font(GLuint t, void *v)
{
}

int ui::label::get_bgimage(GLuint t, void *v)
{
}

void ui::label::set_bgimage(GLuint t, void *v)
{
}

void ui::label::populate_buffers(void)
{
}

ui::label::label(ui::context *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h)
{
    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D,
                     GL_TEXTURE_BORDER_COLOR,
                     glm::value_ptr(this->background));
    this->populate_buffers();
}

ui::label::~label()
{
    glDeleteTextures(1, &this->tex);
}

int ui::label::get(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::font)
        return this->get_font(t, v);
    return ui::panel::get(e, t, v);
}

void ui::label::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:
        this->set_font(t, v);
        this->populate_buffers();
        break;

      case ui::element::bgimage:
        this->set_bgimage(t, v);
        this->populate_buffers();
        break;

      default:
        ui::panel::set(e, t, v);
        break;
    }
}

void ui::label::draw(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    ui::panel::draw();
    glDisable(GL_TEXTURE_2D);
}
