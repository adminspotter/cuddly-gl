/* text_field.h                                            -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 14 Jul 2016, 19:00:52 tquirk
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
 * This file contains the text field declaration.  We're subclassing
 * the label, since it already has font handling built in.  We'll be
 * adding keyboard handling and a moving cursor.
 *
 * Things to do
 *   - Determine if we want to add a 'password' flag here, to display
 *     things as asterisks, or have that in a subclass.
 *
 */

#ifndef __INC_R9_TEXT_FIELD_H__
#define __INC_R9_TEXT_FIELD_H__

#include "label.h"

namespace ui
{
    class text_field : public label
    {
    };
}

#endif /* __INC_R9_TEXT_FIELD_H__ */
