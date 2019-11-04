/* armable.h                                               -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 03 Nov 2019, 15:57:55 tquirk
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
 * This file contains the armable object declaration.  This is an
 * intermediate class between the label and the various buttons.  It
 * adds the concepts of armed and active, and adds some basic
 * callbacks to perform those actions.  It is a pure virtual, so it is
 * not instantiable; its subclasses must provide methods which define
 * what happens when armed or active states are set.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_ARMABLE_H__
#define __INC_CUDDLY_ARMABLE_H__

#include "label.h"

namespace ui
{
    class armable : public label
    {
      protected:
        bool activated, armed;
        int activations;

        virtual int get_state(GLuint, bool *) const override;
        virtual void set_state(GLuint, bool) override;

        int get_active_state(bool *) const;
        virtual void set_active_state(bool) = 0;
        int get_arm_state(bool *) const;
        virtual void set_arm_state(bool) = 0;

        void activate(void);
        void deactivate(void);
        void arm(void);
        void disarm(void);

        static void enter_callback(active *, void *, void *);
        static void leave_callback(active *, void *, void *);
        static void focus_callback(active *, void *, void *);
        static void mouse_down_callback(active *, void *, void *);
        static void mouse_up_callback(active *, void *, void *);
        static void key_down_callback(active *, void *, void *);
        static void key_up_callback(active *, void *, void *);

        void init(composite *);

      public:
        explicit armable(composite *);
        ~armable();
    };
}

#endif /* __INC_CUDDLY_ARMABLE_H__ */
