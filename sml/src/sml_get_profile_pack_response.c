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

#include <sml/sml_get_profile_pack_response.h>
#include <stdio.h>

static void *sml_prof_obj_header_entry_parse_(sml_buffer *buf);
static void *sml_value_entry_parse_(sml_buffer *buf);
static void sml_value_entry_free_(void *p);
static void sml_value_entry_write_(void *p, sml_buffer *buf);

static void sml_prof_obj_header_entry_free_(void *p) {
	sml_prof_obj_header_entry *entry = p;
	if (entry) {
		sml_octet_string_free(entry->obj_name);
		sml_unit_free(entry->unit);
		sml_number_free(entry->scaler);

		free(entry);
	}
}

static void *sml_prof_obj_period_entry_parse_(sml_buffer *buf) {
	sml_prof_obj_period_entry *entry = sml_prof_obj_period_entry_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 4) {
		buf->error = 1;
		goto error;
	}

	entry->val_time = sml_time_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;
	entry->status = sml_u64_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;
	entry->value_list = sml_sequence_parse(buf, sml_value_entry_parse_, sml_value_entry_free_);
	if (sml_buf_has_errors(buf))
		goto error;
	entry->period_signature = sml_signature_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return entry;

error:
	buf->error = 1;
	sml_prof_obj_period_entry_free(entry);
	return NULL;
}

static void sml_prof_obj_period_entry_free_(void *p) {
	sml_prof_obj_period_entry *entry = p;

	if (entry) {
		sml_time_free(entry->val_time);
		sml_number_free(entry->status);
		sml_sequence_free(entry->value_list);
		sml_signature_free(entry->period_signature);

		free(entry);
	}
}

static void sml_prof_obj_header_entry_write_(void *p, sml_buffer *buf) {
	sml_prof_obj_header_entry *entry = p;

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 3);

	sml_octet_string_write(entry->obj_name, buf);
	sml_unit_write(entry->unit, buf);
	sml_i8_write(entry->scaler, buf);
}

static void sml_prof_obj_period_entry_write_(void *p, sml_buffer *buf) {
	sml_prof_obj_period_entry *entry = p;

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 4);
	sml_time_write(entry->val_time, buf);
	sml_u64_write(entry->status, buf);
	sml_sequence_write(entry->value_list, buf, sml_value_entry_write_);
	sml_signature_write(entry->period_signature, buf);
}

static void *sml_value_entry_parse_(sml_buffer *buf) {
	sml_value_entry *entry = sml_value_entry_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 2) {
		buf->error = 1;
		goto error;
	}

	entry->value = sml_value_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;
	entry->value_signature = sml_signature_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return entry;

error:
	buf->error = 1;
	sml_value_entry_free(entry);
	return NULL;
}

static void sml_value_entry_write_(void *p, sml_buffer *buf) {
	sml_value_entry *entry = p;

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 2);
	sml_value_write(entry->value, buf);
	sml_signature_write(entry->value_signature, buf);
}

static void sml_value_entry_free_(void *p) {
	sml_value_entry *entry = p;

	if (entry) {
		sml_value_free(entry->value);
		sml_signature_free(entry->value_signature);

		free(entry);
	}
}

// sml_get_profile_pack_response;

sml_get_profile_pack_response *sml_get_profile_pack_response_init() {
	sml_get_profile_pack_response *msg =
		(sml_get_profile_pack_response *)malloc(sizeof(sml_get_profile_pack_response));
	*msg = (sml_get_profile_pack_response){.server_id = NULL,
										   .act_time = NULL,
										   .reg_period = NULL,
										   .parameter_tree_path = NULL,
										   .header_list = NULL,
										   .period_list = NULL,
										   .rawdata = NULL,
										   .profile_signature = NULL};

	return msg;
}

sml_get_profile_pack_response *sml_get_profile_pack_response_parse(sml_buffer *buf) {
	sml_get_profile_pack_response *msg = sml_get_profile_pack_response_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 8) {
		buf->error = 1;
		goto error;
	}

	msg->server_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->act_time = sml_time_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->reg_period = sml_u32_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->parameter_tree_path = sml_tree_path_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->header_list =
		sml_sequence_parse(buf, sml_prof_obj_header_entry_parse_, sml_prof_obj_header_entry_free_);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->period_list =
		sml_sequence_parse(buf, sml_prof_obj_period_entry_parse_, sml_prof_obj_period_entry_free_);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->rawdata = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->profile_signature = sml_signature_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return msg;

error:
	buf->error = 1;
	sml_get_profile_pack_response_free(msg);
	return NULL;
}

void sml_get_profile_pack_response_write(sml_get_profile_pack_response *msg, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 8);

	sml_octet_string_write(msg->server_id, buf);
	sml_time_write(msg->act_time, buf);
	sml_u32_write(msg->reg_period, buf);
	sml_tree_path_write(msg->parameter_tree_path, buf);
	sml_sequence_write(msg->header_list, buf, sml_prof_obj_header_entry_write_);
	sml_sequence_write(msg->period_list, buf, sml_prof_obj_period_entry_write_);
	sml_octet_string_write(msg->rawdata, buf);
	sml_signature_write(msg->profile_signature, buf);
}

void sml_get_profile_pack_response_free(sml_get_profile_pack_response *msg) {
	if (msg) {
		sml_octet_string_free(msg->server_id);
		sml_time_free(msg->act_time);
		sml_number_free(msg->reg_period);
		sml_tree_path_free(msg->parameter_tree_path);
		sml_sequence_free(msg->header_list);
		sml_sequence_free(msg->period_list);
		sml_octet_string_free(msg->rawdata);
		sml_signature_free(msg->profile_signature);

		free(msg);
	}
}

// sml_prof_obj_header_entry;

sml_prof_obj_header_entry *sml_prof_obj_header_entry_init() {
	sml_prof_obj_header_entry *entry =
		(sml_prof_obj_header_entry *)malloc(sizeof(sml_prof_obj_header_entry));
	*entry = (sml_prof_obj_header_entry){.obj_name = NULL, .unit = NULL, .scaler = NULL};
	return entry;
}

static void *sml_prof_obj_header_entry_parse_(sml_buffer *buf) {
	sml_prof_obj_header_entry *entry = sml_prof_obj_header_entry_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 3) {
		buf->error = 1;
		goto error;
	}

	entry->obj_name = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;
	entry->unit = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;
	entry->scaler = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return entry;
error:
	buf->error = 1;
	sml_prof_obj_header_entry_free(entry);
	return NULL;
}

sml_prof_obj_header_entry *sml_prof_obj_header_entry_parse(sml_buffer *buf) {
	return sml_prof_obj_header_entry_parse_(buf);
}

void sml_prof_obj_header_entry_write(sml_prof_obj_header_entry *entry, sml_buffer *buf) {
	sml_prof_obj_header_entry_write_(entry, buf);
}

void sml_prof_obj_header_entry_free(sml_prof_obj_header_entry *entry) {
	sml_prof_obj_header_entry_free_(entry);
}

// sml_prof_obj_period_entry;

sml_prof_obj_period_entry *sml_prof_obj_period_entry_init() {
	sml_prof_obj_period_entry *entry =
		(sml_prof_obj_period_entry *)malloc(sizeof(sml_prof_obj_period_entry));
	*entry = (sml_prof_obj_period_entry){
		.val_time = NULL, .status = NULL, .value_list = NULL, .period_signature = NULL};
	return entry;
}

sml_prof_obj_period_entry *sml_prof_obj_period_entry_parse(sml_buffer *buf) {
	return sml_prof_obj_period_entry_parse_(buf);
}

void sml_prof_obj_period_entry_write(sml_prof_obj_period_entry *entry, sml_buffer *buf) {
	sml_prof_obj_period_entry_write_(entry, buf);
}

void sml_prof_obj_period_entry_free(sml_prof_obj_period_entry *entry) {
	sml_prof_obj_period_entry_free_(entry);
}

// sml_value_entry;

sml_value_entry *sml_value_entry_init() {
	sml_value_entry *entry = (sml_value_entry *)malloc(sizeof(sml_value_entry));
	*entry = (sml_value_entry){.value = NULL, .value_signature = NULL};

	return entry;
}

sml_value_entry *sml_value_entry_parse(sml_buffer *buf) { return sml_value_entry_parse_(buf); }

void sml_value_entry_write(sml_value_entry *entry, sml_buffer *buf) {
	sml_value_entry_write_(entry, buf);
}

void sml_value_entry_free(sml_value_entry *entry) { sml_value_entry_free_(entry); }
