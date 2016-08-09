/* composite.h                                             -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 08 Aug 2016, 22:27:17 tquirk
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
 * This file contains the declaration for the composite object.  It's
 * a panel which can have children, which will then be subclassable
 * into more interesting manager widgets.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_COMPOSITE_H__
#define __INC_R9_COMPOSITE_H__

#include "panel.h"

namespace ui
{
    class composite : public panel
    {
      public:
        composite(context *, GLuint, GLuint);
        virtual ~composite();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);

        virtual void draw(void);
    };
}

#endif /* __INC_R9_COMPOSITE_H__ */
