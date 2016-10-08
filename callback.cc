/* callback.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 07 Oct 2016, 08:07:27 tquirk
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
 * This file contains the callback list method definitions for the R9
 * UI widget set.
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "callback.h"

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
    }
}

ui::active::active(GLuint w, GLuint h)
    : ui::rect(w, h),
      enter_cb(), leave_cb(), motion_cb(), btn_down_cb(), btn_up_cb(),
      key_down_cb(), key_up_cb()
{
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
