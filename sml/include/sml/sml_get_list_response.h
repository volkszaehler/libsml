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

#ifndef SML_GET_LIST_RESPONSE_H_
#define SML_GET_LIST_RESPONSE_H_

#include "sml_list.h"
#include "sml_octet_string.h"
#include "sml_shared.h"
#include "sml_time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	octet_string *client_id; // optional
	octet_string *server_id;
	octet_string *list_name;   // optional
	sml_time *act_sensor_time; // optional
	sml_list *val_list;
	sml_signature *list_signature; // optional
	sml_time *act_gateway_time;    // optional
} sml_get_list_response;

sml_get_list_response *sml_get_list_response_init();
sml_get_list_response *sml_get_list_response_parse(sml_buffer *buf);
void sml_get_list_response_write(sml_get_list_response *msg, sml_buffer *buf);
void sml_get_list_response_free(sml_get_list_response *msg);

#ifdef __cplusplus
}
#endif

#endif /* SML_GET_LIST_RESPONSE_H_ */
