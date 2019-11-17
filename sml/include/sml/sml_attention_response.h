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

#ifndef SML_ATTENTION_RESPONSE_H_
#define SML_ATTENTION_RESPONSE_H_

#include "sml_octet_string.h"
#include "sml_shared.h"
#include "sml_tree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	octet_string *server_id;
	octet_string *attention_number;
	octet_string *attention_message; // optional
	sml_tree *attention_details;     // optional
} sml_attention_response;

sml_attention_response *sml_attention_response_init();
sml_attention_response *sml_attention_response_parse(sml_buffer *buf);
void sml_attention_response_write(sml_attention_response *msg, sml_buffer *buf);
void sml_attention_response_free(sml_attention_response *msg);

#ifdef __cplusplus
}
#endif

#endif /* SML_ATTENTION_RESPONSE_H_ */
