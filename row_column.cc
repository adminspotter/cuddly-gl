/* row_column.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2020  Trinity Annabelle Quirk
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
 * This file contains the method definitions for the row-column
 * object.  It extends the manager to add grid layout of its children.
 *
 * Things to do
 *
 */

#include <algorithm>

#include "row_column.h"

int ui::row_column::get_size(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::size::rows:     *v = this->grid_sz.x;  return 0;
      case ui::size::columns:  *v = this->grid_sz.y;  return 0;
      default:                 return this->manager::get_size(t, v);
    }
}

int ui::row_column::get_size(GLuint t, glm::ivec2 *v) const
{
    if (t == ui::size::grid)
    {
        *v = this->grid_sz;
        return 0;
    }
    return this->manager::get_size(t, v);
}

void ui::row_column::set_size(GLuint t, GLuint v)
{
    switch (t)
    {
      case ui::size::rows:     this->grid_sz.y = v;  break;
      case ui::size::columns:  this->grid_sz.x = v;  break;
      default:                 this->manager::set_size(t, v);
    }
}

void ui::row_column::set_size(GLuint t, const glm::ivec2& v)
{
    if (t == ui::size::grid)
        this->grid_sz = v;
    else
        this->manager::set_size(t, v);
}

int ui::row_column::get_order(GLuint t, GLuint *v) const
{
    *v = this->pack_order;
    return 0;
}

void ui::row_column::set_order(GLuint t, GLuint v)
{
    if (v == ui::order::row || v == ui::order::column)
        this->pack_order = v;
}

glm::ivec2 ui::row_column::calculate_cell_size(void)
{
    glm::ivec2 cell_size(0, 0), child_sz;

    for (auto& i : this->children)
    {
        i->get(ui::element::size, ui::size::all, &child_sz);
        cell_size.x = std::max(cell_size.x, child_sz.x);
        cell_size.y = std::max(cell_size.y, child_sz.y);
    }
    return cell_size;
}

/* Either of the grid_sz elements can be 0, which means that we don't
 * care how many of them there are.  Also, we can have both elements
 * nonzero, but too many children to fit into the desired grid, so
 * we'll have to increase the size of the grid to compensate.
 */
glm::ivec2 ui::row_column::calculate_grid_size(void)
{
    int num_children = this->children.size();
    glm::ivec2 actual(this->grid_sz);

    /* Easy case: we have a full grid size setting, and not too many
     * children to fit into it, so we'll just use what we have.  We
     * also handle the no-grid-size-setting case here.
     */
    if ((actual.x == 0 && actual.y == 0)
        || (actual.x != 0 && actual.y != 0
            && actual.x * actual.y >= num_children))
        return actual;

    /* Check if we only have a prescribed number of columns. */
    if (actual.x == 0)
        actual.x = (num_children / actual.y)
            + (num_children % actual.y > 0 ? 1 : 0);
    else
        actual.y = (num_children / actual.x)
            + (num_children % actual.x > 0 ? 1 : 0);

    return actual;
}

void ui::row_column::set_desired_size(void)
{
    glm::ivec2 old_sz(this->dim);
    glm::ivec2 cell_size(0, 0), grid_size(0, 0);

    this->composite::set_desired_size();

    cell_size = this->calculate_cell_size();
    grid_size = this->calculate_grid_size();
    this->dim.x = ((cell_size.x + this->child_spacing.x) * grid_size.x)
        + this->child_spacing.x
        + this->margin[1] + this->margin[2]
        + this->border[1] + this->border[2];
    this->dim.y = ((cell_size.y + this->child_spacing.y) * grid_size.y)
        + this->child_spacing.y
        + this->margin[0] + this->margin[3]
        + this->border[0] + this->border[3];
    if (this->dim != old_sz)
    {
        ui::resize_call_data call_data = { this->dim };
        this->call_callbacks(ui::callback::resize, &call_data);
    }
    this->composite::parent->move_child(this);
    this->populate_buffers();

    if (this->pack_order == ui::order::row)
        this->insert_row_major(grid_size, cell_size);
    else
        this->insert_column_major(grid_size, cell_size);
    this->dirty = false;
}

void ui::row_column::insert_row_major(glm::ivec2& grid, glm::ivec2& cell)
{
    glm::ivec2 pos(this->margin[1] + this->border[1] + this->child_spacing.x,
                   this->margin[0] + this->border[0] + this->child_spacing.y);
    glm::ivec2 cur_pos = pos;
    auto c = this->children.begin();

    for (int i = 0; i < grid.y; ++i)
    {
        cur_pos.y = pos.y + ((cell.y + this->child_spacing.y) * i);
        for (int j = 0; j < grid.x; ++j)
        {
            cur_pos.x = pos.x + ((cell.x + this->child_spacing.x) * j);
            (*c)->set(ui::element::position, ui::position::all, cur_pos);

            if (++c == this->children.end())
                return;
        }
    }
}

void ui::row_column::insert_column_major(glm::ivec2& grid, glm::ivec2& cell)
{
    glm::ivec2 pos(this->margin[1] + this->border[1] + this->child_spacing.x,
                   this->margin[0] + this->border[0] + this->child_spacing.y);
    glm::ivec2 cur_pos = pos;
    auto c = this->children.begin();

    for (int i = 0; i < grid.x; ++i)
    {
        cur_pos.x = pos.x + ((cell.x + this->child_spacing.x) * i);
        for (int j = 0; j < grid.y; ++j)
        {
            cur_pos.y = pos.y + ((cell.y + this->child_spacing.y) * j);
            (*c)->set(ui::element::position, ui::position::all, cur_pos);

            if (++c == this->children.end())
                return;
        }
    }
}

void ui::row_column::init(ui::composite *c)
{
    this->pack_order = ui::order::row;
}

ui::row_column::row_column(ui::composite *c)
    : ui::manager::manager(c), ui::active::active(0, 0),
      ui::rect::rect(0, 0), grid_sz(1, 0)
{
    this->init(c);
}

ui::row_column::~row_column()
{
}

int ui::row_column::get(GLuint e, GLuint t, GLuint *v) const
{
    if (e == ui::element::order)
        return this->get_order(t, v);
    return this->manager::get(e, t, v);
}

void ui::row_column::set(GLuint e, GLuint t, GLuint v)
{
    if (e == ui::element::order)
        this->set_order(t, v);
    else
        this->manager::set(e, t, v);
}
