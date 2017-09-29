/* active.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 04 Dec 2016, 16:57:41 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
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
 * This file contains the callback list method definitions for the
 * CuddlyGL UI widget set.
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "active.h"

const ui::to_point zero_time(std::chrono::seconds(0));

std::list<ui::cb_list_elem>& ui::active::which_cb_list(GLuint which)
{
    switch (which)
    {
      case ui::callback::enter:     return this->enter_cb;
      case ui::callback::leave:     return this->leave_cb;
      default:
      case ui::callback::btn_down:  return this->btn_down_cb;
      case ui::callback::btn_up:    return this->btn_up_cb;
      case ui::callback::motion:    return this->motion_cb;
      case ui::callback::key_down:  return this->key_down_cb;
      case ui::callback::key_up:    return this->key_up_cb;
      case ui::callback::resize:    return this->resize_cb;
    }
}

ui::active::active(GLuint w, GLuint h)
    : ui::rect(w, h),
      enter_cb(), leave_cb(), motion_cb(), btn_down_cb(), btn_up_cb(),
      key_down_cb(), key_up_cb(), resize_cb()
{
    this->timeout = ui::zero_time;
    this->timeout_func = NULL;
    this->timeout_arg = NULL;
}

ui::active::~active()
{
}

void ui::active::add_callback(GLuint cb_list,
                                    ui::cb_fptr funcptr,
                                    void *client)
{
    std::list<ui::cb_list_elem>& l = this->which_cb_list(cb_list);
    cb_list_elem new_elem = {funcptr, client};

    l.push_back(new_elem);
}

void ui::active::remove_callback(GLuint cb_list,
                                       ui::cb_fptr funcptr,
                                       void *client)
{
    std::list<ui::cb_list_elem>& l = this->which_cb_list(cb_list);
    cb_list_elem old_elem = {funcptr, client};

    l.remove(old_elem);
}

void ui::active::call_callbacks(GLuint cb_list, void *call_data)
{
    std::list<ui::cb_list_elem>& l = this->which_cb_list(cb_list);
    std::list<ui::cb_list_elem>::iterator i;

    for (i = l.begin(); i != l.end(); ++i)
        (*i)(this, call_data);
}

void ui::active::add_timeout(ui::to_until until, ui::to_fptr fp, void *a)
{
    this->timeout = ui::to_time::now() + until;
    this->timeout_func = fp;
    this->timeout_arg = a;
}

void ui::active::remove_timeout(void)
{
    this->timeout = ui::zero_time;
    this->timeout_func = NULL;
    this->timeout_arg = NULL;
}

void ui::active::call_timeout(void)
{
    if (this->timeout != ui::zero_time
        && this->timeout <= ui::to_time::now())
    {
        if (this->timeout_func != NULL)
            (*this->timeout_func)(this, this->timeout_arg);
        this->remove_timeout();
    }
}

