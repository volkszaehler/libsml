// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed
// DAI-Labor, TU-Berlin
//
// This file is part of libSML.
//
// libSML is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libSML is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libSML.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SML_NUMBER_H_
#define SML_NUMBER_H_

#include "sml_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

void *sml_number_init(u64 number, unsigned char type, int size);

// Parses a number. Identified by type (SML_TYPE_INTEGER or SML_TYPE_UNSIGNED)
// and maximal number of bytes (SML_TYPE_NUMBER_8, SML_TYPE_NUMBER_16,
// SML_TYPE_NUMBER_32, SML_TYPE_NUMBER_64)
void *sml_number_parse(sml_buffer *buf, unsigned char type, int max_size);

void sml_number_write(void *np, unsigned char type, int size, sml_buffer *buf);

void sml_number_free(void *np);

#define sml_u8_init(n) (u8 *)sml_number_init(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_8)
#define sml_u16_init(n) (u16 *)sml_number_init(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_16)
#define sml_u32_init(n) (u32 *)sml_number_init(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_32)
#define sml_u64_init(n) (u64 *)sml_number_init(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_64)
#define sml_i8_init(n) (i8 *)sml_number_init(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_8)
#define sml_i16_init(n) (i16 *)sml_number_init(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_16)
#define sml_i32_init(n) (i32 *)sml_number_init(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_32)
#define sml_i64_init(n) (i64 *)sml_number_init(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_64)

#define sml_u8_parse(buf) (u8 *)sml_number_parse(buf, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_8)
#define sml_u16_parse(buf) (u16 *)sml_number_parse(buf, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_16)
#define sml_u32_parse(buf) (u32 *)sml_number_parse(buf, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_32)
#define sml_u64_parse(buf) (u64 *)sml_number_parse(buf, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_64)
#define sml_i8_parse(buf) (i8 *)sml_number_parse(buf, SML_TYPE_INTEGER, SML_TYPE_NUMBER_8)
#define sml_i16_parse(buf) (i16 *)sml_number_parse(buf, SML_TYPE_INTEGER, SML_TYPE_NUMBER_16)
#define sml_i32_parse(buf) (i32 *)sml_number_parse(buf, SML_TYPE_INTEGER, SML_TYPE_NUMBER_32)
#define sml_i64_parse(buf) (i64 *)sml_number_parse(buf, SML_TYPE_INTEGER, SML_TYPE_NUMBER_64)

#define sml_u8_write(n, buf) sml_number_write(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_8, buf)
#define sml_u16_write(n, buf) sml_number_write(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_16, buf)
#define sml_u32_write(n, buf) sml_number_write(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_32, buf)
#define sml_u64_write(n, buf) sml_number_write(n, SML_TYPE_UNSIGNED, SML_TYPE_NUMBER_64, buf)
#define sml_i8_write(n, buf) sml_number_write(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_8, buf)
#define sml_i16_write(n, buf) sml_number_write(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_16, buf)
#define sml_i32_write(n, buf) sml_number_write(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_32, buf)
#define sml_i64_write(n, buf) sml_number_write(n, SML_TYPE_INTEGER, SML_TYPE_NUMBER_64, buf)

#define sml_u8_free(n) sml_number_free(n)
#define sml_u16_free(n) sml_number_free(n)
#define sml_u32_free(n) sml_number_free(n)
#define sml_u64_free(n) sml_number_free(n)
#define sml_i8_free(n) sml_number_free(n)
#define sml_i16_free(n) sml_number_free(n)
#define sml_i32_free(n) sml_number_free(n)
#define sml_i64_free(n) sml_number_free(n)

typedef u8 sml_unit;
#define sml_unit_init(n) sml_u8_init(n)
#define sml_unit_parse(buf) sml_u8_parse(buf)
#define sml_unit_write(n, buf) sml_u8_write(n, buf)
#define sml_unit_free(np) sml_number_free(np)

#ifdef __cplusplus
}
#endif

#endif /* SML_NUMBER_H_ */
