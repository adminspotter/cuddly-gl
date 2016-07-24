/* button.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 24 Jul 2016, 09:33:59 tquirk
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
 * This file contains the basic button object declaration.  We derive
 * from the label, which will take care of the actual text/image.  In
 * this class, we'll actually implement some event callbacks.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_BUTTON_H__
#define __INC_R9_BUTTON_H__

#include <list>

#include "label.h"

namespace ui
{
    class button : public label
    {
      protected:
        bool active, armed;

        int get_active_state(GLuint, void *);
        void set_active_state(GLuint, void *);
        int get_arm_state(GLuint, void *);
        void set_arm_state(GLuint, void *);
        void set_margin(GLuint, void *);

        void grow_border(void);
        void shrink_border(void);

        static void activate(panel *, void *, void *);
        static void deactivate(panel *, void *, void *);
        static void arm(panel *, void *, void *);
        static void disarm(panel *, void *, void *);

      public:
        button(context *, GLuint = 0, GLuint = 0);
        virtual ~button();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);
    };
}

#endif /* __INC_R9_BUTTON_H__ */
