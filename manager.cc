/* manager.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 10 Aug 2016, 08:26:37 tquirk
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
 * This file contains the method definitions for the manager object,
 * which combines the panel with the composite.
 *
 * Things to do
 *
 */

#include "manager.h"

ui::manager::manager(ui::composite *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h), ui::composite::composite(c, w, h)
{
}

ui::manager::~manager()
{
}
