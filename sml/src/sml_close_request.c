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

#include <sml/sml_close_request.h>
#include <stdio.h>

sml_close_request *sml_close_request_init() {
	sml_close_request *close_request = (sml_close_request *)malloc(sizeof(sml_close_request));
	*close_request = (sml_close_request){.global_signature = NULL};

	return close_request;
}

sml_close_request *sml_close_request_parse(sml_buffer *buf) {
	sml_close_request *msg = sml_close_request_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 1) {
		buf->error = 1;
		goto error;
	}

	msg->global_signature = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return msg;

error:
	sml_close_request_free(msg);
	return NULL;
}

void sml_close_request_write(sml_close_request *msg, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 1);
	sml_octet_string_write(msg->global_signature, buf);
}

void sml_close_request_free(sml_close_request *msg) {
	if (msg) {
		sml_octet_string_free(msg->global_signature);
		free(msg);
	}
}
