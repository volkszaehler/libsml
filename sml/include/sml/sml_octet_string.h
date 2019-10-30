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

#ifndef SML_OCTET_STRING_H_
#define SML_OCTET_STRING_H_

#include "sml_shared.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned char *str;
	int len;
} octet_string;

octet_string *sml_octet_string_init(unsigned char *str, int length);
octet_string *sml_octet_string_init_from_hex(char *str);
// Parses a octet string, updates the buffer accordingly, memory must be free'd elsewhere.
octet_string *sml_octet_string_parse(sml_buffer *buf);
octet_string *sml_octet_string_generate_uuid();
void sml_octet_string_write(octet_string *str, sml_buffer *buf);
void sml_octet_string_free(octet_string *str);
int sml_octet_string_cmp(octet_string *s1, octet_string *s2);
int sml_octet_string_cmp_with_hex(octet_string *str, char *hex);

// sml signature
typedef octet_string sml_signature;
#define sml_signature_parse(buf) sml_octet_string_parse(buf)
#define sml_signature_write(s, buf) sml_octet_string_write(s, buf)
#define sml_signature_free(s) sml_octet_string_free(s)

#ifdef __cplusplus
}
#endif

#endif /* SML_OCTET_STRING_H_ */
