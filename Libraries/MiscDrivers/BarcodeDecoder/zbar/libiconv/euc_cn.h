/*
 * Copyright (C) 1999-2001, 2016 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * EUC-CN
 */

static int euc_cn_mbtowc(conv_t conv, ucs4_t *pwc, const unsigned char *s, size_t n)
{
    unsigned char c = *s;
    /* Code set 0 (ASCII or GB 1988-89) */
    if (c < 0x80)
        return ascii_mbtowc(conv, pwc, s, n);
    /* Code set 1 (GB 2312-1980) */
    if (c >= 0xa1 && c < 0xff) {
        if (n < 2)
            return RET_TOOFEW(0);
        {
            unsigned char c2 = s[1];
            if (c2 >= 0xa1 && c2 < 0xff) {
                unsigned char buf[2];
                buf[0] = c - 0x80;
                buf[1] = c2 - 0x80;
                return gb2312_mbtowc(conv, pwc, buf, 2);
            } else
                return RET_ILSEQ;
        }
    }
    return RET_ILSEQ;
}

static int euc_cn_wctomb(conv_t conv, unsigned char *r, ucs4_t wc, size_t n)
{
    unsigned char buf[2];
    int ret;

    /* Code set 0 (ASCII or GB 1988-89) */
    ret = ascii_wctomb(conv, r, wc, n);
    if (ret != RET_ILUNI)
        return ret;

    /* Code set 1 (GB 2312-1980) */
    ret = gb2312_wctomb(conv, buf, wc, 2);
    if (ret != RET_ILUNI) {
        if (ret != 2)
            abort();
        if (n < 2)
            return RET_TOOSMALL;
        r[0] = buf[0] + 0x80;
        r[1] = buf[1] + 0x80;
        return 2;
    }

    return RET_ILUNI;
}
