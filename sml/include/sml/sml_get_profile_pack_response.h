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

#ifndef SML_GET_PROFILE_PACK_RESPONSE_H_
#define SML_GET_PROFILE_PACK_RESPONSE_H_

#include "sml_list.h"
#include "sml_octet_string.h"
#include "sml_shared.h"
#include "sml_time.h"
#include "sml_tree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	octet_string *server_id;
	sml_time *act_time; // specified by germans (current time was meant) ;)
	u32 *reg_period;
	sml_tree_path *parameter_tree_path;
	sml_sequence *header_list;        // list of sml_prof_obj_header_entry
	sml_sequence *period_list;        // list of sml_prof_obj_period_entry
	octet_string *rawdata;            // optional
	sml_signature *profile_signature; // optional

} sml_get_profile_pack_response;

sml_get_profile_pack_response *sml_get_profile_pack_response_init();
sml_get_profile_pack_response *sml_get_profile_pack_response_parse(sml_buffer *buf);
void sml_get_profile_pack_response_write(sml_get_profile_pack_response *msg, sml_buffer *buf);
void sml_get_profile_pack_response_free(sml_get_profile_pack_response *msg);

typedef struct {
	octet_string *obj_name;
	sml_unit *unit;
	i8 *scaler;
} sml_prof_obj_header_entry;

sml_prof_obj_header_entry *sml_prof_obj_header_entry_init();
sml_prof_obj_header_entry *sml_prof_obj_header_entry_parse(sml_buffer *buf);
void sml_prof_obj_header_entry_write(sml_prof_obj_header_entry *entry, sml_buffer *buf);
void sml_prof_obj_header_entry_free(sml_prof_obj_header_entry *entry);

typedef struct {
	sml_time *val_time;
	u64 *status;
	sml_sequence *value_list;
	sml_signature *period_signature;
} sml_prof_obj_period_entry;

sml_prof_obj_period_entry *sml_prof_obj_period_entry_init();
sml_prof_obj_period_entry *sml_prof_obj_period_entry_parse(sml_buffer *buf);
void sml_prof_obj_period_entry_write(sml_prof_obj_period_entry *entry, sml_buffer *buf);
void sml_prof_obj_period_entry_free(sml_prof_obj_period_entry *entry);

typedef struct {
	sml_value *value;
	sml_signature *value_signature;
} sml_value_entry;

sml_value_entry *sml_value_entry_init();
sml_value_entry *sml_value_entry_parse(sml_buffer *buf);
void sml_value_entry_write(sml_value_entry *entry, sml_buffer *buf);
void sml_value_entry_free(sml_value_entry *entry);

#ifdef __cplusplus
}
#endif

#endif /* SML_GET_PROFILE_PACK_RESPONSE_H_ */
