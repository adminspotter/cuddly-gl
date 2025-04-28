/* util.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
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
 * This file contains the declarations for some string conversion functions.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_UTIL_H__
#define __INC_CUDDLY_UTIL_H__

#include <string>

namespace ui
{
    std::u32string utf8tou32str(const std::string&);
    std::string u32strtoutf8(const std::u32string&);
}

#endif /* __INC_CUDDLY_UTIL_H__ */
