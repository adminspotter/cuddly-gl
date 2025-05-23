/* button.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2018  Trinity Annabelle Quirk
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
 * from the armable, which, via the label, takes care of the actual
 * text/image.  In this class, we'll implement the arm/active logic
 * for the armable's callbacks.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_BUTTON_H__
#define __INC_CUDDLY_BUTTON_H__

#include <list>

#include "armable.h"

namespace ui
{
    class button : public armable
    {
      protected:
        virtual void set_margin(GLuint, GLuint) override;

        virtual void set_active_state(bool) override;
        virtual void set_arm_state(bool) override;
        void grow_border(void);
        void shrink_border(void);

        void init(composite *);

      public:
        explicit button(composite *);
        template<typename... Args>
        button(composite *c, Args... args)
            : rect(0, 0), active(0, 0), armable(c)
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~button();
    };
}

#endif /* __INC_CUDDLY_BUTTON_H__ */
