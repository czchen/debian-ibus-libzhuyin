/* vim:set et ts=4 sts=4:
 *
 * ibus-libzhuyin - New Zhuyin engine based on libzhuyin for IBus
 *
 * Copyright (c) 2008-2010 Peng Huang <shawn.p.huang@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef __ZY_BUS_H_
#define __ZY_BUS_H_

#include <ibus.h>
#include "ZYObject.h"

namespace ZY {

class Bus : Object {
public:
    Bus (void) : Object (ibus_bus_new ()) { }

    bool isConnected (void)
    {
        return ibus_bus_is_connected (*this);
    }

    operator IBusBus * (void) const
    {
        return get<IBusBus> ();
    }
};

};
#endif
