/* row_column.h                                            -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Jul 2018, 08:04:54 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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
 * This file contains the declaration for the row-column object.  It
 * extends the manager to add grid layout of its children.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_ROW_COLUMN_H__
#define __INC_CUDDLY_ROW_COLUMN_H__

#include "manager.h"

namespace ui
{
    class row_column : public manager
    {
      protected:
        glm::ivec2 grid_sz;
        GLuint pack_order;

        virtual int get_size(GLuint, void *) const override;
        virtual void set_size(GLuint, const void *) override;
        virtual int get_order(GLuint, void *) const;
        virtual void set_order(GLuint, const void *);

        glm::ivec2 calculate_cell_size(void);
        glm::ivec2 calculate_grid_size(void);
        virtual void set_desired_size(void) override;
        void insert_row_major(glm::ivec2&, glm::ivec2&);
        void insert_column_major(glm::ivec2&, glm::ivec2&);

      public:
        row_column(composite *, GLuint, GLuint);
        virtual ~row_column();

        virtual int get(GLuint, GLuint, void *) const override;
        virtual void set(GLuint, GLuint, const void *) override;
    };
}

#endif /* __INC_CUDDLY_ROW_COLUMN_H__ */
