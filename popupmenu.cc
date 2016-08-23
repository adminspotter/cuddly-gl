/* popupmenu.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 23 Aug 2016, 16:35:42 tquirk
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
 * This file contains the method definitions for the popup menu.  Our
 * long-desired pie menus are finally implemented!
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "ui.h"
#include "popupmenu.h"

int ui::popupmenu::get_popup(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::popup::visible:  *(bool *)v = this->visible;      break;
      case ui::popup::button:   *(int *)v = this->popup_button;  break;
      default:                  ret = 1;                         break;
    }
    return ret;
}

void ui::popupmenu::set_popup(GLuint t, void *v)
{
    switch (t)
    {
      case ui::popup::visible:  this->visible = *(bool *)v;      break;
      case ui::popup::button:   this->popup_button = *(int *)v;  break;
    }
}

void ui::popupmenu::show(ui::panel *p, void *call, void *client)
{
    ui::popupmenu *pm = (ui::popupmenu *)client;
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;
    bool t = true;

    if (bcd->button == pm->popup_button && bcd->state == ui::mouse::down)
    {
        glm::ivec2 tmp_vec;

        pm->get(ui::element::size, ui::size::all, &tmp_vec);
        tmp_vec = bcd->location - (tmp_vec / 2);
        pm->set_va(ui::element::position, ui::position::all, &tmp_vec,
                   ui::element::popup, ui::popup::visible, &t, 0);
    }
}

void ui::popupmenu::hide(ui::panel *p, void *call, void *client)
{
    ui::popupmenu *pm = (ui::popupmenu *)client;
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;
    bool f = false;

    if (bcd->button == pm->popup_button && bcd->state == ui::mouse::up)
        pm->set(ui::element::popup, ui::popup::visible, &f);
}


ui::popupmenu::popupmenu(composite *c, GLuint w, GLuint h)
    : ui::manager::manager(c, w, h)
{
    ui::context *ctx = dynamic_cast<ui::context *>(c);
    ui::manager *mgr = dynamic_cast<ui::manager *>(c);
    this->popup_button = ui::mouse::button2;

    /* Our parent could be either a UI context, or a regular manager */
    if (ctx != NULL)
        ctx->set(ui::element::popup, ui::popup::menu, this);
    else if (mgr != NULL)
    {
        mgr->add_callback(ui::callback::btn_down, ui::popupmenu::show, this);
        mgr->add_callback(ui::callback::btn_up, ui::popupmenu::hide, this);
    }
}

ui::popupmenu::~popupmenu()
{
    ui::context *ctx = dynamic_cast<ui::context *>(this->composite::parent);
    ui::manager *mgr = dynamic_cast<ui::manager *>(this->composite::parent);

    /* Our parent could be either a UI context, or a regular manager */
    if (ctx != NULL)
        ctx->set(ui::element::popup, ui::popup::menu, NULL);
    else if (mgr != NULL)
    {
        mgr->remove_callback(ui::callback::btn_down, ui::popupmenu::show, this);
        mgr->remove_callback(ui::callback::btn_up, ui::popupmenu::hide, this);
    }
}

int ui::popupmenu::get(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::popup)
        return this->get_popup(t, v);
    return this->manager::get(e, t, v);
}

void ui::popupmenu::set(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::popup)
        this->set_popup(t, v);
    else
        this->manager::set(e, t, v);
}
