/* active.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Oct 2016, 08:32:49 tquirk
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
 * This file adds the callback role to our rect class, to try to
 * reduce multiple inheritance.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_ACTIVE_H__
#define __INC_R9_ACTIVE_H__

#include <list>

#include "rect.h"

namespace ui
{
    /* A forward declaration, to solve multi-include problems */
    class active;

    /* Callback function pointer */
    typedef void (*cb_fptr)(active *, void *, void *);

    typedef struct cb_list_tag
    {
        cb_fptr ptr;
        void *client_data;

        bool operator==(const struct cb_list_tag& p) const
            {
                return (this->ptr == p.ptr
                        && this->client_data == p.client_data);
            };
        void operator()(active *p, void *call_data)
            {
                this->ptr(p, call_data, this->client_data);
            };
    }
    cb_list_elem;

    class active : public virtual rect
    {
      protected:
        std::list<cb_list_elem> enter_cb, leave_cb, motion_cb;
        std::list<cb_list_elem> btn_down_cb, btn_up_cb;
        std::list<cb_list_elem> key_down_cb, key_up_cb;

        std::list<cb_list_elem>& which_cb_list(GLuint);

      public:
        active(GLuint, GLuint);
        ~active();

        virtual void add_callback(GLuint, cb_fptr, void *);
        virtual void remove_callback(GLuint, cb_fptr, void *);
        virtual void call_callbacks(GLuint, void *);
    };
}

#endif /* __INC_R9_ACTIVE_H__ */
