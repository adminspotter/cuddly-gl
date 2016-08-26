/* callback.h                                              -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Aug 2016, 23:05:19 tquirk
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
 * This file contains the basic callback role.  We need to factor the
 * callback lists out of everything, so they can be a role in
 * anything.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_CALLBACK_H__
#define __INC_R9_CALLBACK_H__

#include <list>

#include "panel.h"

namespace ui
{
    /* A forward declaration, to solve multi-include problems */
    class panel;

    /* Callback function pointer */
    typedef void (*cb_fptr)(panel *, void *, void *);

    typedef struct cb_list_tag
    {
        cb_fptr ptr;
        void *client_data;

        bool operator==(const struct cb_list_tag& p) const
            {
                return (this->ptr == p.ptr
                        && this->client_data == p.client_data);
            };
        void operator()(panel *p, void *call_data)
            {
                this->ptr(p, call_data, this->client_data);
            };
    }
    cb_list_elem;

    class event_handler
    {
      protected:
        std::list<cb_list_elem> enter_cb, leave_cb, motion_cb;
        std::list<cb_list_elem> btn_down_cb, btn_up_cb;
        std::list<cb_list_elem> key_down_cb, key_up_cb;

        std::list<cb_list_elem>& which_cb_list(GLuint);

      public:
        event_handler();
        ~event_handler();

        virtual void add_callback(GLuint, cb_fptr, void *);
        virtual void remove_callback(GLuint, cb_fptr, void *);
        virtual void call_callbacks(GLuint, void *);
    };
}

#endif /* __INC_R9_CALLBACK_H__ */
