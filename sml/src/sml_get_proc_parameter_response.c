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

#include <sml/sml_get_proc_parameter_response.h>
#include <stdio.h>

sml_get_proc_parameter_response *sml_get_proc_parameter_response_init() {
	sml_get_proc_parameter_response *msg =
		(sml_get_proc_parameter_response *)malloc(sizeof(sml_get_proc_parameter_response));
	*msg = (sml_get_proc_parameter_response){
		.server_id = NULL, .parameter_tree_path = NULL, .parameter_tree = NULL};

	return msg;
}

sml_get_proc_parameter_response *sml_get_proc_parameter_response_parse(sml_buffer *buf) {
	sml_get_proc_parameter_response *msg = sml_get_proc_parameter_response_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 3) {
		buf->error = 1;
		goto error;
	}

	msg->server_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->parameter_tree_path = sml_tree_path_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->parameter_tree = sml_tree_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return msg;

error:
	sml_get_proc_parameter_response_free(msg);
	return NULL;
}

void sml_get_proc_parameter_response_write(sml_get_proc_parameter_response *msg, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 3);
	sml_octet_string_write(msg->server_id, buf);
	sml_tree_path_write(msg->parameter_tree_path, buf);
	sml_tree_write(msg->parameter_tree, buf);
}

void sml_get_proc_parameter_response_free(sml_get_proc_parameter_response *msg) {
	if (msg) {
		sml_octet_string_free(msg->server_id);
		sml_tree_path_free(msg->parameter_tree_path);
		sml_tree_free(msg->parameter_tree);

		free(msg);
	}
}
