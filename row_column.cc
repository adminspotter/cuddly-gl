/* row_column.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 16 Aug 2016, 06:43:11 tquirk
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
 * This file contains the method definitions for the row-column
 * object.  It extends the manager to add grid layout of its children.
 *
 * Things to do
 *
 */

#include "row_column.h"

int ui::row_column::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::width:
      case ui::size::height:   return this->manager::get_size(t, v);

      case ui::size::rows:     *(GLuint *)v = this->grid_sz.x;  break;
      case ui::size::columns:  *(GLuint *)v = this->grid_sz.y;  break;
      default:                 ret = 1;                         break;
    }
    return ret;
}

void ui::row_column::set_size(GLuint t, void *v)
{
    switch (t)
    {
      case ui::size::width:
      case ui::size::height:   this->manager::set_size(t, v);   break;

      case ui::size::rows:     this->grid_sz.x = *(GLuint *)v;  break;
      case ui::size::columns:  this->grid_sz.y = *(GLuint *)v;  break;
    }
}

ui::row_column::row_column(ui::composite *c, GLuint w, GLuint h)
    : ui::manager::manager(c, w, h), grid_sz(0, 1)
{
}

ui::row_column::~row_column()
{
}
