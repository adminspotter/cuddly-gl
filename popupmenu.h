/* popupmenu.h                                             -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 23 Aug 2016, 21:04:46 tquirk
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
 * This file contains the declaration for the popup menu.  It is a
 * composite subclass which will draw a round or oval popup window,
 * chopped into several segments.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_POPUPMENU_H__
#define __INC_R9_POPUPMENU_H__

#include "manager.h"

namespace ui
{
    class popupmenu : public manager
    {
      protected:
        bool visible;
        int popup_button;

        int get_popup(GLuint, void *);
        void set_popup(GLuint, void *);

        static void show(panel *, void *, void *);
        static void hide(panel *, void *, void *);

        void populate_buffers(void);

      public:
        popupmenu(composite *, GLuint, GLuint);
        virtual ~popupmenu();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);
    };
}

#endif /* __INC_R9_POPUPMENU_H__ */
