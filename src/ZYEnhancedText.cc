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

#include "ZYEnhancedText.h"
#include <assert.h>


namespace ZY{

const char symbol_lead_char = '`';
const int symbol_parts_num = 3;

typedef String::const_iterator iterator_t;

#if 0
/* template method, to be moved to PhoneticEditor class. */
bool
isPhonetic (const char key)
{
    /* use zhuyin_in_chewing_keyboard here. */
    return PHONETIC_SECTION;
}
#endif


String
escape_brace (const String & text)
{
    String braced_text = "{";

    iterator_t iter;
    for (iter = text.begin (); iter != text.end (); ++iter) {
        braced_text += *iter;
        if ('{' == *iter || '}' == *iter) {
            braced_text += *iter;
        }
    }

    braced_text += "}";
    return braced_text;
}

String
get_next_brace(const String & enhanced_text, size_t start_pos, size_t & end_pos)
{
    String text;

    iterator_t iter = enhanced_text.begin () + start_pos;

    /* eat the first '{'. */
    assert ('{' == *iter);
    ++iter;

    for (; iter != enhanced_text.end (); ++iter ) {
        switch(*iter) {
        case '{':
            ++iter;
            /* eat the next '{'. */
            assert ('{' == *iter);
            text += *iter;
            break;
        case '}':
            ++iter;
            if ('}' == *iter) {
                text += *iter;
            } else {
                /* exit loop here. */
                end_pos = iter - enhanced_text.begin ();
                return text;
            }
            break;
        default:
            text += *iter;
            break;
        }
    }

    /* can't reach the end, will end at the last '}'. */
    assert(iter != enhanced_text.end ());
    return text;
}

/* implementation */
size_t
get_enhanced_text_length (const String & enhanced_text)
{
    size_t length = 0;
    size_t start_pos = 0, end_pos = 0;

    iterator_t iter;
    for (iter = enhanced_text.begin (); iter != enhanced_text.end ();) {
        /* handle symbol section. */
        if (symbol_lead_char == *iter) {
            ++iter;

            start_pos = iter - enhanced_text.begin ();
            /* eat 3 braces. */
            for (int i = 0; i < symbol_parts_num; ++i) {
                get_next_brace (enhanced_text, start_pos, end_pos);
                start_pos = end_pos;
            }

            iter = enhanced_text.begin () + start_pos;
            ++length;
            continue;
        }

        /* only phonetic character allows here. */
        /* assert(PHONETIC_SECTION == isPhonetic(*iter)); */
        ++iter;
        ++length;
    }

    return length;
}

section_t
probe_section (const String & enhanced_text, size_t offset, size_t & pos)
{
    size_t start_pos = 0, end_pos = 0;

    /* loop from start to offset. */
    iterator_t iter;
    for (iter = enhanced_text.begin (); iter != enhanced_text.end (); ) {
        /* only one character left. */
        if (0 == offset)
            break;

        /* Note: some duplicated code here... */
        /* handle symbol section. */
        if (symbol_lead_char == *iter) {
            ++iter;

            start_pos = iter - enhanced_text.begin ();
            /* eat 3 braces. */
            for (int i = 0; i < symbol_parts_num; ++i) {
                get_next_brace (enhanced_text, start_pos, end_pos);
                start_pos = end_pos;
            }

            iter = enhanced_text.begin () + start_pos;
            --offset;
            continue;
        }

        /* only phonetic character allows here. */
        /* assert(PHONETIC_SECTION == isPhonetic(*iter)); */
        ++iter;
        --offset;
    }

    pos = iter - enhanced_text.begin ();

    if (symbol_lead_char == *iter)
        return SYMBOL_SECTION;
    else
        return PHONETIC_SECTION;
}

section_t
probe_section_quick (const String & enhanced_text, size_t pos)
{
    iterator_t iter = enhanced_text.begin () + pos;

    if (symbol_lead_char == *iter)
        return SYMBOL_SECTION;
    else
        return PHONETIC_SECTION;
}

bool
get_phonetic_section (const String & enhanced_text,
                      size_t start_pos, size_t & end_pos, String & section)
{
    section.clear();
    iterator_t iter = enhanced_text.begin () + start_pos;

    /* safe check here. */
    if (symbol_lead_char == *iter)
        return false;

    /* gather consecutive characters. */
    for (; iter != enhanced_text.end (); ++iter) {
        if (symbol_lead_char == *iter)
            break;

        /* assert(PHONETIC_SECTION == isPhonetic(*iter)); */
        section += *iter;
    }

    end_pos = iter - enhanced_text.begin ();
    return true;
}

bool
get_symbol_section (const String & enhanced_text,
                    size_t start_pos, size_t & end_pos,
                    String & type, String & lookup, String & choice)
{
    iterator_t iter = enhanced_text.begin () + start_pos;

    if (symbol_lead_char != *iter)
        return false;

    assert (symbol_lead_char == *iter);
    ++iter;

    /* eat the type. */
    start_pos = iter - enhanced_text.begin ();
    type = get_next_brace (enhanced_text, start_pos, end_pos);
    start_pos = end_pos;

    /* eat the lookup. */
    lookup = get_next_brace (enhanced_text, start_pos, end_pos);
    start_pos = end_pos;

    /* eat the choice. */
    choice = get_next_brace (enhanced_text, start_pos, end_pos);
    start_pos = end_pos;

    return true;
}

bool
probe_section_start(const String & enhanced_text,
                    guint offset, guint & inner_offset,
                    size_t & index, size_t & start_pos)
{
    /* decrement the cursor variable to calculate the zhuyin cursor. */
    guint cursor = offset;
    inner_offset = 0;

    index = 0;
    start_pos = 0; size_t end_pos = 0;

    while (end_pos != enhanced_text.size ()) {
        if (0 == cursor)
            break;

        start_pos = end_pos;
        section_t type = probe_section_quick (enhanced_text, start_pos);

        if (PHONETIC_SECTION == type) {
            String section;
            get_phonetic_section (enhanced_text, start_pos, end_pos, section);

            size_t section_len = end_pos - start_pos;

            if (cursor < section_len)
                break;

            cursor -= section_len;

            if (0 == cursor) {
                start_pos = end_pos;
                break;
            }

            ++index;
        }

        if (SYMBOL_SECTION == type) {
            String type, lookup, choice;
            get_symbol_section (enhanced_text, start_pos, end_pos,
                                type, lookup, choice);
            --cursor;
        }
    }

    inner_offset = cursor;
    return true;
}

bool
insert_section (String & enhanced_text, size_t offset, const String & section)
{
    size_t pos = 0;
    section_t section_type = probe_section (enhanced_text, offset, pos);

    enhanced_text = enhanced_text.substr (0, pos) + section +
        enhanced_text.substr (pos);

    return true;
}

bool
insert_phonetic(String & enhanced_text, size_t offset, const char key)
{
    String section;
    section += key;
    return insert_section(enhanced_text, offset, section);
}

bool
insert_symbol(String & enhanced_text, size_t offset,
              const String & type, const String & lookup,
              const String & choice)
{
    String section;
    section += symbol_lead_char;
    section += escape_brace (type) + escape_brace (lookup) +
        escape_brace(choice);

    return insert_section(enhanced_text, offset, section);
}

bool
erase_input_sequence(String & enhanced_text, size_t offset, size_t length)
{
    size_t pos = 0, start_pos = 0, end_pos = 0;
    section_t section_type = probe_section(enhanced_text, offset, pos);
    start_pos = pos; end_pos = pos;
    for (size_t i = 0; i < length; ++i) {
        switch (section_type) {
        case PHONETIC_SECTION:
            ++ end_pos;
            break;
        case SYMBOL_SECTION:
            {
                String type, lookup, choice;
                assert (get_symbol_section (enhanced_text, pos, end_pos,
                                            type, lookup, choice));
            }
            break;
        }
        pos = end_pos;

        /* reach the end of the enhanced text. */
        if (enhanced_text.length () == pos)
            break;

        section_type = probe_section_quick (enhanced_text, pos);
    }

    enhanced_text.erase (start_pos, end_pos - start_pos);
    return true;
}

size_t
get_number_of_phonetic_sections (String & enhanced_text)
{
    size_t num = 0;
    size_t start_pos = 0, end_pos = 0;

    while (end_pos != enhanced_text.size ()) {
        section_t type = probe_section_quick (enhanced_text, start_pos);

        if (PHONETIC_SECTION == type) {
            String section;
            get_phonetic_section (enhanced_text, start_pos, end_pos, section);
            ++num;
        }

        if (SYMBOL_SECTION == type) {
            String type, lookup, choice;
            get_symbol_section (enhanced_text, start_pos, end_pos,
                                type, lookup, choice);
        }

        start_pos = end_pos;
    }

    return num;
}

};
