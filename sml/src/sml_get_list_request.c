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

#include <sml/sml_get_list_request.h>
#include <stdio.h>

sml_get_list_request *sml_get_list_request_init() {
	sml_get_list_request *msg = (sml_get_list_request *)malloc(sizeof(sml_get_list_request));
	*msg = (sml_get_list_request){.client_id = NULL,
								  .server_id = NULL,
								  .username = NULL,
								  .password = NULL,
								  .list_name = NULL};

	return msg;
}

void sml_get_list_request_write(sml_get_list_request *msg, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 5);

	sml_octet_string_write(msg->client_id, buf);
	sml_octet_string_write(msg->server_id, buf);
	sml_octet_string_write(msg->username, buf);
	sml_octet_string_write(msg->password, buf);
	sml_octet_string_write(msg->list_name, buf);
}

sml_get_list_request *sml_get_list_request_parse(sml_buffer *buf) {
	sml_get_list_request *msg = (sml_get_list_request *)malloc(sizeof(sml_get_list_request));
	*msg = (sml_get_list_request){.client_id = NULL,
								  .server_id = NULL,
								  .username = NULL,
								  .password = NULL,
								  .list_name = NULL};

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 5) {
		buf->error = 1;
		goto error;
	}

	msg->client_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->server_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->username = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->password = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->list_name = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return msg;

error:
	sml_get_list_request_free(msg);
	return NULL;
}

void sml_get_list_request_free(sml_get_list_request *msg) {
	if (msg) {
		sml_octet_string_free(msg->client_id);
		sml_octet_string_free(msg->server_id);
		sml_octet_string_free(msg->list_name);
		sml_octet_string_free(msg->username);
		sml_octet_string_free(msg->password);
		free(msg);
	}
}
