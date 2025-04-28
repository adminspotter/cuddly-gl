/* toggle.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2019  Trinity Annabelle Quirk
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
 * This file contains the toggle button object declaration.  We derive
 * from the basic button, which will take care of the arm/active
 * states.  In this class, we'll add the check box and mark.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_TOGGLE_H__
#define __INC_CUDDLY_TOGGLE_H__

#include "armable.h"

namespace ui
{
    class toggle : public armable
    {
      protected:
        bool checked;

        virtual int get_state(GLuint, bool *) const override;
        virtual void set_state(GLuint, bool) override;

        virtual void set_active_state(bool) override;
        virtual void set_arm_state(bool) override;
        int get_checked_state(bool *) const;
        void set_checked_state(bool);

        void empty_checkbox(void);
        void fill_checkbox(void);

        virtual void calculate_widget_size(void) override;
        virtual vertex_buffer *generate_points(void) override;

        static void check(active *, void *, void *);

        void init(composite *);

      public:
        explicit toggle(composite *);
        template<typename... Args>
        toggle(composite *c, Args... args)
            : rect(0, 0), active(0, 0), armable(c)
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~toggle();
    };
}

#endif /* __INC_CUDDLY_TOGGLE_H__ */
