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
#ifndef __ZY_STRING_H_
#define __ZY_STRING_H_

#include <glib.h>
#include <stdarg.h>
#include <assert.h>
#include <string>

namespace ZY {

class String : public std::string {
public:
    String () : std::string () { }
    String (const gchar *str) : std::string (str) { }
    String (const std::string &str) : std::string (str) { }
    /* TODO: remove the following line later. */
    G_DEPRECATED String (gint len) : std::string () { reserve (len); }
    String (const gchar ch) : std::string (1, ch) { }

    String & printf (const gchar *fmt, ...)
    {
        gchar *str;
        va_list args;

        va_start (args, fmt);
        str = g_strdup_vprintf (fmt, args);
        va_end (args);

        assign (str);
        g_free (str);
        return *this;
    }

    String & appendPrintf (const gchar *fmt, ...)
    {
        gchar *str;
        va_list args;

        va_start (args, fmt);
        str = g_strdup_vprintf (fmt, args);
        va_end (args);

        append (str);
        g_free (str);

        return *this;
    }

    String & appendUnichar (gunichar ch)
    {
        gchar str[12];
        gint len;
        len = g_unichar_to_utf8 (ch, str);
        str[len] = 0;
        append (str);
        return *this;
    }

    String & insert (gint i, gchar ch)
    {
        std::string::insert (i, 1, ch);
        return *this;
    }

    String & truncate (guint len)
    {
        erase(len);
        return *this;
    }

    gsize utf8Length (void) const
    {
        return g_utf8_strlen (c_str(), -1);
    }

    String & operator<< (gint i)
    {
        return appendPrintf ("%d", i);
    }

    String & operator<< (guint i)
    {
        return appendPrintf ("%u", i);
    }

    String & operator<< (const gchar ch)
    {
        append (1, ch);
        return *this;
    }

    String & operator<< (const gchar *str)
    {
        append (str);
        return *this;
    }

    String & operator<< (const gunichar *wstr)
    {
        gchar *str;
        GError *error;
        str = g_ucs4_to_utf8 (wstr, -1, NULL, NULL, &error);
        if (str == NULL) {
            g_warning ("convert ucs4 to utf8 failed: %s", error->message);
            g_error_free (error);
        }
        else {
            append (str);
            g_free (str);
        }
        return *this;
    }

    gchar operator[] (gint i)
    {
        return std::string::operator[] (i);
    }

    String & operator<< (const std::string &str)
    {
        return operator<< (str.c_str ());
    }

    String & operator<< (const String &str)
    {
        return operator<< ((const gchar *)str);
    }

    String & operator= (const gchar * str)
    {
        assign (str);
        return *this;
    }

    String & operator= (const gchar ch)
    {
        assign (1, ch);
        return *this;
    }

    operator const gchar *(void) const
    {
        return this->c_str ();
    }

    operator gboolean (void) const
    {
        return ! empty ();
    }
};

};
#endif
