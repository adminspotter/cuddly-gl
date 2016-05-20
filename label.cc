#include <stdexcept>

#include "label.h"

int ui::label::get_font(GLuint t, void *v)
{
    v = (void *)this->font;
    return 0;
}

void ui::label::set_font(GLuint t, void *v)
{
    this->font = (Font *)v;
}

int ui::label::get_string(GLuint t, void *v)
{
    *((std::string *)v) = this->str;
    return 0;
}

void ui::label::set_string(GLuint t, void *v)
{
    this->use_text = true;
    if (this->image != NULL)
    {
        delete[] this->image;
        this->image = NULL;
    }
    this->str = *((std::string *)v);
}

int ui::label::get_bgimage(GLuint t, void *v)
{
    v = (void *)this->image;
}

void ui::label::set_bgimage(GLuint t, void *v)
{
    if (this->width == 0 || this->height == 0)
        throw std::runtime_error(_("Width and height must be set before assigning a background image"));

    this->use_text = false;
    this->str = "";
    if (this->image != NULL)
        delete[] this->image;
    this->image = new unsigned char[this->width * this->height];
    memcpy(this->image, v, this->width * this->height);
}

void ui::label::populate_buffers(void)
{
    if (this->use_text)
    {
    }
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                 this->width, this->height, 0, GL_ALPHA,
                 GL_UNSIGNED_BYTE, this->image);
}

ui::label::label(ui::context *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h), str()
{
    this->use_text = true;
    this->image = NULL;
    this->font = NULL;
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
    if (this->image != NULL)
        delete[] this->image;
}

int ui::label::get(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:     return this->get_font(t, v);
      case ui::element::string:   return this->get_string(t, v);
      case ui::element::bgimage:  return this->get_bgimage(t, v);
      default:                    return ui::panel::get(e, t, v);
    }
}

void ui::label::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:
        this->set_font(t, v);
        this->populate_buffers();
        break;

      case ui::element::string:
        this->set_string(t, v);
        this->populate_buffers();
        break;

      case ui::element::bgimage:
        this->set_bgimage(t, v);
        this->populate_buffers();
        break;

      default:
        /* This already calls populate_buffers, so no need to call it again */
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
