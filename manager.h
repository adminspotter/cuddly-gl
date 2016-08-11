/* manager.h                                          -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 11 Aug 2016, 07:44:02 tquirk
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
 * This file contains the declaration for the manager object.  It
 * combines the panel with the composite, so we can have dialog boxes
 * and menu bars and menus and whatnot.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_MANAGER_H__
#define __INC_R9_MANAGER_H__

#include "panel.h"
#include "composite.h"

namespace ui
{
    class manager : public panel, public composite
    {
      protected:
        static void motion_callback(panel *, void *, void *);
        static void button_callback(panel *, void *, void *);
        static void keypress_callback(panel *, void *, void *);

      public:
        manager(composite *, GLuint, GLuint);
        ~manager();
    };
}

#endif /* __INC_R9_MANAGER_H__ */
