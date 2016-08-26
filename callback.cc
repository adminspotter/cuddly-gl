/* callback.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Aug 2016, 00:24:03 tquirk
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

ui::event_target::event_target()
    : enter_cb(), leave_cb(), motion_cb(), btn_down_cb(), btn_up_cb(),
      key_down_cb(), key_up_cb()
{
}

ui::event_target::~event_target()
{
}
