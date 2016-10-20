/* ui.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 20 Oct 2016, 07:36:39 tquirk
 *
 * Revision IX game client
 * Copyright (C) 2016  Trinity Annabelle Quirk
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
 * This file contains the ui context method definitions for the R9 UI
 * widget set.
 *
 * Things to do
 *
 */

#include <string>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "ui.h"
#include "shader.h"

int ui::context::get_attribute(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::attribute::position:
        *((GLuint *)v) = this->pos_attr;
        break;
      case ui::attribute::normal:
        *((GLuint *)v) = this->norm_attr;
        break;
      case ui::attribute::color:
        *((GLuint *)v) = this->color_attr;
        break;
      case ui::attribute::texture:
        *((GLuint *)v) = this->texture_attr;
        break;
      case ui::attribute::use_text:
        *((GLuint *)v) = this->use_text_uniform;
        break;
      case ui::attribute::text_bgnd:
        *((GLuint *)v) = this->text_bgnd_uniform;
        break;
      case ui::attribute::translate:
        *((GLuint *)v) = this->translate_uniform;
        break;
      default: ret = 1; break;
    }
    return ret;
}

ui::context::context(GLuint w, GLuint h)
    : ui::composite::composite(NULL, w, h), ui::rect::rect(w, h)
{
    this->vert_shader = load_shader(GL_VERTEX_SHADER,
                                    SHADER_SRC_PATH "/ui_vertex.glsl");
    this->frag_shader = load_shader(GL_FRAGMENT_SHADER,
                                    SHADER_SRC_PATH "/ui_fragment.glsl");
    this->shader_pgm = create_program(vert_shader, 0, frag_shader, "fcolor");
    glUseProgram(this->shader_pgm);
    this->pos_attr = glGetAttribLocation(shader_pgm, "position");
    this->norm_attr = glGetAttribLocation(shader_pgm, "normal");
    this->color_attr = glGetAttribLocation(shader_pgm, "color");
    this->texture_attr = glGetAttribLocation(shader_pgm, "texture_uv");
    this->use_text_uniform = glGetUniformLocation(shader_pgm, "use_text");
    this->text_bgnd_uniform = glGetUniformLocation(shader_pgm, "text_bgnd");
    this->translate_uniform = glGetUniformLocation(shader_pgm, "translate");
}

ui::context::~context()
{
    glDeleteShader(this->vert_shader);
    glDeleteShader(this->frag_shader);
    glUseProgram(0);
    glDeleteProgram(this->shader_pgm);
}

int ui::context::get(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::attribute:  return this->get_attribute(t, v);
      default:                      return this->composite::get(e, t, v);
    }
}

void ui::context::draw(void)
{
    glm::mat4 basic_trans;

    glUseProgram(this->shader_pgm);
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        (*i)->draw(this->translate_uniform, basic_trans);
}
