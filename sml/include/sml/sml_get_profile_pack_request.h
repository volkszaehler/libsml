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

#ifndef SML_GET_PROFILE_PACK_REQUEST_H_
#define SML_GET_PROFILE_PACK_REQUEST_H_

#include "sml_list.h"
#include "sml_octet_string.h"
#include "sml_shared.h"
#include "sml_time.h"
#include "sml_tree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef octet_string sml_obj_req_entry;
#define sml_obj_req_entry_parse(buf) sml_octet_string_parse(buf)
#define sml_obj_req_entry_write(p, buf) sml_octet_string_write(p, buf)
#define sml_obj_req_entry_free(p) sml_octet_string_free(p)

typedef struct sml_obj_req_entry_list_entry {
	sml_obj_req_entry *object_list_entry;

	// list specific
	struct sml_obj_req_entry_list_entry *next;
} sml_obj_req_entry_list;

typedef struct {
	octet_string *server_id;   // optional
	octet_string *username;    // optional
	octet_string *password;    // optional
	sml_boolean *with_rawdata; // optional
	sml_time *begin_time;      // optional
	sml_time *end_time;        // optional
	sml_tree_path *parameter_tree_path;
	sml_obj_req_entry_list *object_list; // optional
	sml_tree *das_details;               // optional
} sml_get_profile_pack_request;

sml_get_profile_pack_request *sml_get_profile_pack_request_parse(sml_buffer *buf);
sml_get_profile_pack_request *sml_get_profile_pack_request_init();
void sml_get_profile_pack_request_write(sml_get_profile_pack_request *msg, sml_buffer *buf);
void sml_get_profile_pack_request_free(sml_get_profile_pack_request *msg);

#ifdef __cplusplus
}
#endif

#endif /* SML_GET_PROFILE_PACK_REQUEST_H_ */
