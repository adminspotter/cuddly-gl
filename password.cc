/* password.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 21 Aug 2016, 12:31:39 tquirk
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
 * This file contains the password field method definitions for the R9
 * UI widget set.
 *
 * Things to do
 *
 */

#include "password.h"

ui::password::password(ui::composite *c, GLuint w, GLuint h)
    : ui::text_field::text_field(c, w, h)
{
}

ui::password::~password()
{
}
