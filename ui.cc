/* ui.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 16 Jul 2016, 17:07:53 tquirk
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

#include "ui.h"
#include "panel.h"
#include "shader.h"

/* We'll use this for our quadtree default max depth */
const int ui::context::tree_max_depth = 4;

int ui::context::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::width:   *((GLuint *)v) = this->width;  break;
      case ui::size::height:  *((GLuint *)v) = this->height; break;
      default:                ret = 1;                       break;
    }
    return ret;
}

void ui::context::set_size(GLuint d, void *v)
{
    GLuint new_v = *((GLuint *)v);

    switch (d)
    {
      case ui::size::width:  this->width = new_v;  break;
      case ui::size::height: this->height = new_v; break;
    }

    /* Regenerate our search tree */
    glm::ivec2 ul = {0, 0}, lr = {this->width, this->height};
    delete this->tree;
    this->tree = new ui::quadtree(NULL, ul, lr, ui::context::tree_max_depth);
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        this->tree->insert(*i);
}

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
      default: ret = 1; break;
    }
    return ret;
}

ui::context::context(GLuint w, GLuint h)
    : children(), old_cursor(0, 0)
{
    glm::ivec2 ul = {0, 0}, lr = {w, h};

    this->width = w;
    this->height = h;

    this->tree = new quadtree(NULL, ul, lr, ui::context::tree_max_depth);
    this->old_child = NULL;

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
}

ui::context::~context()
{
    glDeleteShader(this->vert_shader);
    glDeleteShader(this->frag_shader);
    glUseProgram(0);
    glDeleteProgram(this->shader_pgm);

    delete this->tree;
}

int ui::context::get(GLuint e, GLuint t, void *v)
{
    int ret;

    switch (e)
    {
      case ui::element::size:       ret = this->get_size(t, v);      break;
      case ui::element::attribute:  ret = this->get_attribute(t, v); break;
      default:                      ret = 1;                         break;
    }
    return ret;
}

void ui::context::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::size: this->set_size(t, v); break;
    }
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        (*i)->populate_buffers();
}

void ui::context::draw(void)
{
    glUseProgram(this->shader_pgm);
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        (*i)->draw();
}

ui::context& ui::context::add_child(ui::panel *p)
{
    auto found = std::find(this->children.begin(), this->children.end(), p);
    if (found == this->children.end())
    {
        this->children.push_back(p);
        this->tree->insert(p);
    }
    return *this;
}

ui::context& ui::context::remove_child(ui::panel *p)
{
    this->children.remove(p);
    this->tree->remove(p);
    return *this;
}

ui::context& ui::context::move_child(ui::panel *p)
{
    /* This is pretty brute-force, but it's at least simple to understand */
    this->tree->remove(p);
    this->tree->insert(p);
    return *this;
}

void ui::context::cursor_pos_callback(int x, int y)
{
    glm::ivec2 pos = {x, y};
    ui::panel *p = this->tree->search(pos);

    if (p != NULL)
    {
        if (this->old_child == NULL)
            p->call_callbacks(ui::callback::enter, NULL);
        p->call_callbacks(ui::callback::motion, NULL);
    }
    else if (this->old_child != NULL)
        this->old_child->call_callbacks(ui::callback::leave, NULL);

    this->old_child = p;
    this->old_cursor = pos;
}

void ui::context::cursor_btn_callback(int btn, int state)
{
    glm::ivec2 pos = this->old_cursor;
    ui::panel *p = this->tree->search(pos);

    if (p != NULL)
    {
        GLuint x, y;
        struct ui::btn_callback_call call_data;

        p->get_va(ui::element::position, ui::position::x, &x,
                  ui::element::position, ui::position::y, &y, 0);
        call_data.location.x = pos.x - x;
        call_data.location.y = pos.y - y;
        call_data.button = btn;
        call_data.state = (state == ui::cursor::up
                           ? ui::callback::btn_up
                           : ui::callback::btn_down);
        p->call_callbacks(call_data.state, &call_data);
    }

    this->old_child = p;
}
