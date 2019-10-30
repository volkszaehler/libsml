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

#include <sml/sml_get_profile_list_response.h>

// sml_get_profile_list_response;

sml_get_profile_list_response *sml_get_profile_list_response_init() {
	sml_get_profile_list_response *msg =
		(sml_get_profile_list_response *)malloc(sizeof(sml_get_profile_list_response));
	*msg = (sml_get_profile_list_response){.server_id = NULL,
										   .act_time = NULL,
										   .reg_period = NULL,
										   .parameter_tree_path = NULL,
										   .val_time = NULL,
										   .status = NULL,
										   .period_list = NULL,
										   .rawdata = NULL,
										   .period_signature = NULL};
	return msg;
}

static void *sml_period_entry_parse_(sml_buffer *buf) { return sml_period_entry_parse(buf); }

static void sml_period_entry_free_(void *p) { sml_period_entry_free(p); }

sml_get_profile_list_response *sml_get_profile_list_response_parse(sml_buffer *buf) {
	sml_get_profile_list_response *msg = sml_get_profile_list_response_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 9) {
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

	msg->val_time = sml_time_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->status = sml_u64_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->period_list = sml_sequence_parse(buf, sml_period_entry_parse_, sml_period_entry_free_);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->rawdata = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->period_signature = sml_signature_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return msg;

error:
	buf->error = 1;
	sml_get_profile_list_response_free(msg);
	return NULL;
}

static void sml_period_entry_write_(void *p, sml_buffer *buf) { sml_period_entry_write(p, buf); }

void sml_get_profile_list_response_write(sml_get_profile_list_response *msg, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 9);

	sml_octet_string_write(msg->server_id, buf);
	sml_time_write(msg->act_time, buf);
	sml_u32_write(msg->reg_period, buf);
	sml_tree_path_write(msg->parameter_tree_path, buf);
	sml_time_write(msg->val_time, buf);
	sml_u64_write(msg->status, buf);
	sml_sequence_write(msg->period_list, buf, sml_period_entry_write_);
	sml_octet_string_write(msg->rawdata, buf);
	sml_signature_write(msg->period_signature, buf);
}

void sml_get_profile_list_response_free(sml_get_profile_list_response *msg) {
	if (msg) {
		sml_octet_string_free(msg->server_id);
		sml_time_free(msg->act_time);
		sml_number_free(msg->reg_period);
		sml_tree_path_free(msg->parameter_tree_path);
		sml_time_free(msg->val_time);
		sml_number_free(msg->status);
		sml_sequence_free(msg->period_list);
		sml_octet_string_free(msg->rawdata);
		sml_signature_free(msg->period_signature);

		free(msg);
	}
}
