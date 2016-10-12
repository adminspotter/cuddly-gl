/* widget.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 12 Oct 2016, 08:27:17 tquirk
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
 * This file contains the basic widget object method definitions.
 *
 * Things to do
 *
 */

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "widget.h"
#include "../l10n.h"

int ui::widget::get_position(GLuint t, void *v)
{
}

void ui::widget::set_position(GLuint t, void *v)
{
}

int ui::widget::get_state(GLuint t, void *v)
{
}

void ui::widget::set_state(GLuint t, void *v)
{
}

void ui::widget::set_size(GLuint t, void *v)
{
}

void ui::widget::generate_points(void)
{
}

void ui::widget::populate_buffers(void)
{
}

ui::widget::widget(ui::composite *c, GLuint w, GLuint h)
    : ui::active(w, h), pos(0, 0), pos_transform()
{
}

ui::widget::~widget()
{
}

int ui::widget::get(GLuint e, GLuint t, void *v)
{
}

void ui::widget::set(GLuint e, GLuint t, void *v)
{
}

void ui::widget::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
}
