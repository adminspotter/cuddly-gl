/* active.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2026  Trinity Annabelle Quirk
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
 * This file adds the callback and timeout roles to our rect class, to
 * try to reduce multiple inheritance.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_ACTIVE_H__
#define __INC_CUDDLY_ACTIVE_H__

#include <list>
#include <chrono>
#include <any>
#include <stdexcept>

#include "rect.h"

namespace ui
{
    class active;

    class cb_list_base
    {
      public:
        typedef void (*cb_fptr_base_t)(ui::active *, std::any *, void *);

        virtual void add(cb_fptr_base_t, void *) {};
        virtual void remove(cb_fptr_base_t, void *) {};
        virtual void call(ui::active *, std::any *) {};
    };

    template<typename call_data_t>
    class cb_list : public cb_list_base
    {
      public:
        typedef void (*cb_fptr_t)(ui::active *, call_data_t&, void *);

      protected:
        class cb_elem
        {
          public:
            cb_fptr_t ptr;
            void *client_data;

            cb_elem(cb_fptr_t ptr, void *client_data)
                {
                    this->ptr = ptr;
                    this->client_data = client_data;
                };
            ~cb_elem() {};
            bool operator==(const cb_elem& p) const
                {
                    return (this->ptr == p.ptr
                            && this->client_data == p.client_data);
                };
            void operator()(active *p, call_data_t& call_data)
                {
                    this->ptr(p, call_data, this->client_data);
                };
        };

        std::list<cb_elem> _list;

      public:
        cb_list<call_data_t>() : _list() {};
        ~cb_list<call_data_t>() {};

        int size(void) const
            {
                return this->_list.size();
            };

        virtual void add(cb_fptr_t funcptr, void *client_data)
            {
                this->_list.push_back({funcptr, client_data});
            };
        virtual void remove(cb_fptr_t funcptr, void *client_data)
            {
                this->_list.remove({funcptr, client_data});
            };
        virtual void call(ui::active *a, call_data_t& call_data)
            {
                for (auto& cb : this->_list)
                    cb(a, call_data);
            };
    };

    typedef void (*to_fptr)(active *, void *);

    typedef std::chrono::steady_clock to_time;
    typedef std::chrono::time_point<typename ui::to_time> to_point;
    typedef ui::to_time::duration to_until;

    static const to_point zero_time;

    class active : public virtual rect
    {
      protected:
        cb_list<mouse_call_data> enter_cb, leave_cb, motion_cb;
        cb_list<btn_call_data> btn_down_cb, btn_up_cb;
        cb_list<key_call_data> key_down_cb, key_up_cb;
        cb_list<resize_call_data> resize_cb;
        cb_list<focus_call_data> focus_cb;
        cb_list<visible_call_data> visible_cb;
        to_point timeout;
        to_fptr timeout_func;
        void *timeout_arg;

        virtual cb_list_base *which_cb_list(GLuint);

      public:
        active(GLuint, GLuint);
        ~active();

        template<typename call_data_t>
        void add_callback(GLuint cb_list_id,
                          void (*funcptr)(active *, call_data_t&, void *),
                          void *client_data)
            {
                auto list_ptr = dynamic_cast<cb_list<call_data_t> *>(
                    this->which_cb_list(cb_list_id)
                );
                if (list_ptr == NULL)
                    throw std::runtime_error("wrong call data type");
                list_ptr->add(funcptr, client_data);
            };
        template<typename call_data_t>
        void remove_callback(GLuint cb_list_id,
                             void (*funcptr)(active *, call_data_t&, void *),
                             void *client_data)
            {
                auto list_ptr = dynamic_cast<cb_list<call_data_t> *>(
                    this->which_cb_list(cb_list_id)
                );
                if (list_ptr == NULL)
                    throw std::runtime_error("wrong call data type");
                list_ptr->remove(funcptr, client_data);
            };
        template<typename call_data_t>
        void call_callbacks(GLuint cb_list_id, call_data_t& call_data)
            {
                auto list_ptr = dynamic_cast<cb_list<call_data_t> *>(
                    this->which_cb_list(cb_list_id)
                );
                if (list_ptr == NULL)
                    throw std::runtime_error("wrong call data type");
                list_ptr->call(this, call_data);
            };

        void add_timeout(to_until, to_fptr, void *);
        void remove_timeout(void);
        void call_timeout(void);
    };
}

#endif /* __INC_CUDDLY_ACTIVE_H__ */
