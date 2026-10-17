/* label.h                                                 -*- C++ -*-
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
 * This file contains the basic label object declaration.  We derive
 * from the text_widget, which will take care of the text handling.
 * In this class, we'll add the ability to set an image, as well as
 * use text.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_LABEL_H__
#define __INC_CUDDLY_LABEL_H__

#include "text_widget.h"

namespace ui
{
    class label : public text_widget
    {
      protected:
        int get_image(GLuint, ui::image *) const;
        virtual void set_image(GLuint, const ui::image&);

      public:
        explicit label(ui::composite *);
        template<typename... Args>
        label(composite *c, Args... args)
            : ui::text_widget::text_widget(c), ui::active::active(0, 0),
              ui::rect::rect(0, 0)
            {
                this->set(args...);
            };
        virtual ~label();

        using ui::text_widget::get;
        int get(GLuint, GLuint, ui::image *) const;
        using ui::text_widget::set;
        virtual void set(GLuint, GLuint, const ui::image&);

        GET_VA;
        SET_VA;
    };
}

#endif /* __INC_CUDDLY_LABEL_H__ */
