/* vim:set et ts=4 sts=4:
 *
 * ibus-libzhuyin - New Zhuyin engine based on libzhuyin for IBus
 *
 * Copyright (c) 2014 Peng Wu <alexepico@gmail.com>
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __ZY_LIB_ZHUYIN_ZHUYIN_EDITOR_H_
#define __ZY_LIB_ZHUYIN_ZHUYIN_EDITOR_H_

#include "ZYZPhoneticEditor.h"

namespace ZY {

class Config;

class ZhuyinEditor : public PhoneticEditor {
    using PhoneticEditor::InputState;

public:
    ZhuyinEditor (ZhuyinProperties & props, Config & config);
    virtual ~ZhuyinEditor (void);

protected:
    gboolean processKeyEvent (guint keyval, guint keycode, guint modifiers);

    virtual void updatePreeditText ();
    virtual void updateAuxiliaryText ();
    virtual void updateZhuyin (void);

    void commit ();
    void reset ();

    gboolean insert (guint keyval, guint keycode, guint modifiers);

protected:
    String m_preedit_text;
};

};

#endif
