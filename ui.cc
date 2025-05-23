/* ui.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2019  Trinity Annabelle Quirk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * This file contains the ui context method definitions for the
 * CuddlyGL UI widget set.
 *
 * Things to do
 *
 */

#include <string>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "ui.h"
#include "shader.h"

int ui::context::get_attribute(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::attribute::position:   *v = this->pos_attr;           return 0;
      case ui::attribute::color:      *v = this->color_attr;         return 0;
      case ui::attribute::texture:    *v = this->texture_attr;       return 0;
      case ui::attribute::text_bgnd:  *v = this->text_bgnd_uniform;  return 0;
      default:                                                       return 1;
    }
}

void ui::context::init(void)
{
    this->vert_shader = load_shader(GL_VERTEX_SHADER);
    this->frag_shader = load_shader(GL_FRAGMENT_SHADER);
    this->shader_pgm = create_program(vert_shader, 0, frag_shader, "fcolor");
    glUseProgram(this->shader_pgm);
    this->pos_attr = glGetAttribLocation(shader_pgm, "position");
    this->color_attr = glGetAttribLocation(shader_pgm, "color");
    this->texture_attr = glGetAttribLocation(shader_pgm, "texture_uv");
    this->text_bgnd_uniform = glGetUniformLocation(shader_pgm, "text_bgnd");
    this->translate_uniform = glGetUniformLocation(shader_pgm, "translate");
}

ui::context::context()
    : ui::composite::composite(NULL), ui::active::active(0, 0),
      ui::rect::rect(0, 0)
{
    this->init();
}

ui::context::~context()
{
    glDeleteShader(this->vert_shader);
    glDeleteShader(this->frag_shader);
    glUseProgram(0);
    glDeleteProgram(this->shader_pgm);
}

int ui::context::get(GLuint e, GLuint t, GLuint *v) const
{
    switch (e)
    {
      case ui::element::attribute:  return this->get_attribute(t, v);
      default:                      return this->composite::get(e, t, v);
    }
}

void ui::context::draw(void)
{
    glm::mat4 basic_trans(1.0f);

    glUseProgram(this->shader_pgm);
    for (auto& i : this->children)
        i->draw(this->translate_uniform, basic_trans);
}
