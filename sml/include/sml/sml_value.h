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

#ifndef SML_VALUE_H_
#define SML_VALUE_H_

#include "sml_boolean.h"
#include "sml_number.h"
#include "sml_octet_string.h"
#include "sml_shared.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u8 type;
	union {
		sml_boolean *boolean;
		octet_string *bytes; // can have zero length
		i8 *int8;
		i16 *int16;
		i32 *int32;
		i64 *int64;
		u8 *uint8;
		u16 *uint16;
		u32 *uint32;
		u64 *uint64;
	} data;
} sml_value;

sml_value *sml_value_init();
sml_value *sml_value_parse(sml_buffer *buf);
void sml_value_write(sml_value *value, sml_buffer *buf);
void sml_value_free(sml_value *value);

// Cast arbitrary sized sml_value to double
double sml_value_to_double(sml_value *value);
// Converts SML octet string to a printable hex string
char *sml_value_to_strhex(sml_value *value, char **result, bool mixed);

#ifdef __cplusplus
}
#endif

#endif /* SML_VALUE_H_ */
