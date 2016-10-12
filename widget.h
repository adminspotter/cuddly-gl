/* widget.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 10 Oct 2016, 07:51:09 tquirk
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
 * This file contains the widget object declaration.  The widget adds
 * a parent, the draw method, some GL entity handling, and is the
 * simplest thing that will actually draw on the screen.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_WIDGET_H__
#define __INC_R9_WIDGET_H__

#include "active.h"
#include "composite.h"

namespace ui
{
    class composite;

    class widget : public active
    {
      protected:
        composite *parent;
        glm::ivec2 pos;

      public:
        widget(composite *, GLuint, GLuint);
        virtual ~widget();

    };
}

#endif /* __INC_R9_WIDGET_H__ */
