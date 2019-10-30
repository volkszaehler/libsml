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

#include <sml/sml_number.h>
#include <sml/sml_open_request.h>
#include <stdio.h>

sml_open_request *sml_open_request_init() {
	sml_open_request *open_request = (sml_open_request *)malloc(sizeof(sml_open_request));
	*open_request = (sml_open_request){.codepage = NULL,
									   .client_id = NULL,
									   .req_file_id = NULL,
									   .server_id = NULL,
									   .username = NULL,
									   .password = NULL,
									   .sml_version = NULL};
	return open_request;
}

sml_open_request *sml_open_request_parse(sml_buffer *buf) {
	sml_open_request *msg = sml_open_request_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 7) {
		buf->error = 1;
		goto error;
	}

	msg->codepage = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->client_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->req_file_id = sml_octet_string_parse(buf);
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

	msg->sml_version = sml_u8_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return msg;

error:
	sml_open_request_free(msg);
	return NULL;
}

void sml_open_request_write(sml_open_request *msg, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 7);

	sml_octet_string_write(msg->codepage, buf);
	sml_octet_string_write(msg->client_id, buf);
	sml_octet_string_write(msg->req_file_id, buf);
	sml_octet_string_write(msg->server_id, buf);
	sml_octet_string_write(msg->username, buf);
	sml_octet_string_write(msg->password, buf);
	sml_u8_write(msg->sml_version, buf);
}

void sml_open_request_free(sml_open_request *msg) {
	if (msg) {
		sml_octet_string_free(msg->codepage);
		sml_octet_string_free(msg->client_id);
		sml_octet_string_free(msg->req_file_id);
		sml_octet_string_free(msg->server_id);
		sml_octet_string_free(msg->username);
		sml_octet_string_free(msg->password);
		sml_number_free(msg->sml_version);

		free(msg);
	}
}
