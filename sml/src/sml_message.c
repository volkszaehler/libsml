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

#include <sml/sml_crc16.h>
#include <sml/sml_list.h>
#include <sml/sml_message.h>
#include <sml/sml_number.h>
#include <sml/sml_octet_string.h>
#include <sml/sml_shared.h>
#include <sml/sml_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// sml_message;

sml_message *sml_message_parse(sml_buffer *buf) {
	sml_message *msg = (sml_message *)malloc(sizeof(sml_message));
	*msg = (sml_message){.transaction_id = NULL,
						 .group_id = NULL,
						 .abort_on_error = NULL,
						 .message_body = NULL,
						 .crc = NULL};
	int msg_start = buf->cursor;
	int len;

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		fprintf(stderr, "not a list\n");
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 6) {
		fprintf(stderr, "length != 6\n");
		buf->error = 1;
		goto error;
	}

	msg->transaction_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) {
		goto error;
	}

	msg->group_id = sml_u8_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->abort_on_error = sml_u8_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	msg->message_body = sml_message_body_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	len = buf->cursor - msg_start;
	if ((buf->buffer_len - buf->cursor) < 3) {
		// libFuzzer ASAN found read out of bound.
		// at least the 2 bytes for the crc and 1 for the sml_message_end there?
		buf->error = 1;
		goto error;
	}

	msg->crc = sml_u16_parse(buf);
	if (sml_buf_has_errors(buf) || !(msg->crc)) {
		buf->error = 1;
		goto error;
	}

	if (
		*msg->crc != sml_crc16_calculate(&(buf->buffer[msg_start]), len) &&
		// Workaround for Holley DTZ541 uses CRC-16/Kermit
		*msg->crc != sml_crc16kermit_calculate(&(buf->buffer[msg_start]), len)
	){
		fprintf(stderr, "libsml: sml_message_parse(): crc mismatch, dropping message\n");
		goto error;
	}

	if (buf->cursor >= buf->buffer_len) {
		fprintf(stderr, "buf->cursor >= buf->buffer_len\n");
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_current_byte(buf) == SML_MESSAGE_END) {
		sml_buf_update_bytes_read(buf, 1);
	}

	return msg;

error:
	sml_message_free(msg);
	return NULL;
}

sml_message *sml_message_init() {
	sml_message *msg = (sml_message *)malloc(sizeof(sml_message));
	*msg = (sml_message){.transaction_id = NULL,
						 .group_id = NULL,
						 .abort_on_error = NULL,
						 .message_body = NULL,
						 .crc = NULL};
	msg->transaction_id = sml_octet_string_generate_uuid();
	return msg;
}

void sml_message_free(sml_message *msg) {
	if (msg) {
		sml_octet_string_free(msg->transaction_id);
		sml_number_free(msg->group_id);
		sml_number_free(msg->abort_on_error);
		sml_message_body_free(msg->message_body);
		sml_number_free(msg->crc);
		free(msg);
	}
}

void sml_message_write(sml_message *msg, sml_buffer *buf) {
	int msg_start = buf->cursor;

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 6);
	sml_octet_string_write(msg->transaction_id, buf);
	sml_u8_write(msg->group_id, buf);
	sml_u8_write(msg->abort_on_error, buf);
	sml_message_body_write(msg->message_body, buf);

	msg->crc =
		sml_u16_init(sml_crc16_calculate(&(buf->buffer[msg_start]), buf->cursor - msg_start));
	sml_u16_write(msg->crc, buf);

	// end of message
	buf->buffer[buf->cursor] = 0x0;
	buf->cursor++;
}

// sml_message_body;

sml_message_body *sml_message_body_parse(sml_buffer *buf) {
	sml_message_body *msg_body = (sml_message_body *)malloc(sizeof(sml_message_body));
	*msg_body = (sml_message_body){.tag = NULL, .data = NULL};

	if ((buf->cursor + 1) > buf->buffer_len) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 2) {
		buf->error = 1;
		goto error;
	}

	msg_body->tag = sml_u32_parse(buf);
	if (sml_buf_has_errors(buf) || msg_body->tag == NULL)
		goto error;

	switch (*(msg_body->tag)) {
	case SML_MESSAGE_OPEN_REQUEST:
		msg_body->data = sml_open_request_parse(buf);
		break;
	case SML_MESSAGE_OPEN_RESPONSE:
		msg_body->data = sml_open_response_parse(buf);
		break;
	case SML_MESSAGE_CLOSE_REQUEST:
		msg_body->data = sml_close_request_parse(buf);
		break;
	case SML_MESSAGE_CLOSE_RESPONSE:
		msg_body->data = sml_close_response_parse(buf);
		break;
	case SML_MESSAGE_GET_PROFILE_PACK_REQUEST:
		msg_body->data = sml_get_profile_pack_request_parse(buf);
		break;
	case SML_MESSAGE_GET_PROFILE_PACK_RESPONSE:
		msg_body->data = sml_get_profile_pack_response_parse(buf);
		break;
	case SML_MESSAGE_GET_PROFILE_LIST_REQUEST:
		msg_body->data = sml_get_profile_list_request_parse(buf);
		break;
	case SML_MESSAGE_GET_PROFILE_LIST_RESPONSE:
		msg_body->data = sml_get_profile_list_response_parse(buf);
		break;
	case SML_MESSAGE_GET_PROC_PARAMETER_REQUEST:
		msg_body->data = sml_get_proc_parameter_request_parse(buf);
		break;
	case SML_MESSAGE_GET_PROC_PARAMETER_RESPONSE:
		msg_body->data = sml_get_proc_parameter_response_parse(buf);
		break;
	case SML_MESSAGE_SET_PROC_PARAMETER_REQUEST:
		msg_body->data = sml_set_proc_parameter_request_parse(buf);
		break;
	case SML_MESSAGE_GET_LIST_REQUEST:
		msg_body->data = sml_get_list_request_parse(buf);
		break;
	case SML_MESSAGE_GET_LIST_RESPONSE:
		msg_body->data = sml_get_list_response_parse(buf);
		break;
	case SML_MESSAGE_ATTENTION_RESPONSE:
		msg_body->data = sml_attention_response_parse(buf);
		break;
	default:
		fprintf(stderr, "libsml: error: message type %04X not yet implemented\n", *(msg_body->tag));
		break;
	}

	return msg_body;

error:
	free(msg_body);
	return NULL;
}

sml_message_body *sml_message_body_init(u32 tag, void *data) {
	sml_message_body *message_body = (sml_message_body *)malloc(sizeof(sml_message_body));
	*message_body = (sml_message_body){.tag = sml_u32_init(tag), .data = data};
	return message_body;
}

void sml_message_body_write(sml_message_body *message_body, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 2);
	sml_u32_write(message_body->tag, buf);

	switch (*(message_body->tag)) {
	case SML_MESSAGE_OPEN_REQUEST:
		sml_open_request_write((sml_open_request *)message_body->data, buf);
		break;
	case SML_MESSAGE_OPEN_RESPONSE:
		sml_open_response_write((sml_open_response *)message_body->data, buf);
		break;
	case SML_MESSAGE_CLOSE_REQUEST:
		sml_close_request_write((sml_close_request *)message_body->data, buf);
		break;
	case SML_MESSAGE_CLOSE_RESPONSE:
		sml_close_response_write((sml_close_response *)message_body->data, buf);
		break;
	case SML_MESSAGE_GET_PROFILE_PACK_REQUEST:
		sml_get_profile_pack_request_write((sml_get_profile_pack_request *)message_body->data, buf);
		break;
	case SML_MESSAGE_GET_PROFILE_PACK_RESPONSE:
		sml_get_profile_pack_response_write((sml_get_profile_pack_response *)message_body->data,
											buf);
		break;
	case SML_MESSAGE_GET_PROFILE_LIST_REQUEST:
		sml_get_profile_list_request_write((sml_get_profile_list_request *)message_body->data, buf);
		break;
	case SML_MESSAGE_GET_PROFILE_LIST_RESPONSE:
		sml_get_profile_list_response_write((sml_get_profile_list_response *)message_body->data,
											buf);
		break;
	case SML_MESSAGE_GET_PROC_PARAMETER_REQUEST:
		sml_get_proc_parameter_request_write((sml_get_proc_parameter_request *)message_body->data,
											 buf);
		break;
	case SML_MESSAGE_GET_PROC_PARAMETER_RESPONSE:
		sml_get_proc_parameter_response_write((sml_get_proc_parameter_response *)message_body->data,
											  buf);
		break;
	case SML_MESSAGE_SET_PROC_PARAMETER_REQUEST:
		sml_set_proc_parameter_request_write((sml_set_proc_parameter_request *)message_body->data,
											 buf);
		break;
	case SML_MESSAGE_GET_LIST_REQUEST:
		sml_get_list_request_write((sml_get_list_request *)message_body->data, buf);
		break;
	case SML_MESSAGE_GET_LIST_RESPONSE:
		sml_get_list_response_write((sml_get_list_response *)message_body->data, buf);
		break;
	case SML_MESSAGE_ATTENTION_RESPONSE:
		sml_attention_response_write((sml_attention_response *)message_body->data, buf);
		break;
	default:
		fprintf(stderr, "libsml: error: message type %04X not yet implemented\n",
				*(message_body->tag));
		break;
	}
}

void sml_message_body_free(sml_message_body *message_body) {
	if (message_body) {
		switch (*(message_body->tag)) {
		case SML_MESSAGE_OPEN_REQUEST:
			sml_open_request_free((sml_open_request *)message_body->data);
			break;
		case SML_MESSAGE_OPEN_RESPONSE:
			sml_open_response_free((sml_open_response *)message_body->data);
			break;
		case SML_MESSAGE_CLOSE_REQUEST:
			sml_close_request_free((sml_close_request *)message_body->data);
			break;
		case SML_MESSAGE_CLOSE_RESPONSE:
			sml_close_response_free((sml_close_response *)message_body->data);
			break;
		case SML_MESSAGE_GET_PROFILE_PACK_REQUEST:
			sml_get_profile_pack_request_free((sml_get_profile_pack_request *)message_body->data);
			break;
		case SML_MESSAGE_GET_PROFILE_PACK_RESPONSE:
			sml_get_profile_pack_response_free((sml_get_profile_pack_response *)message_body->data);
			break;
		case SML_MESSAGE_GET_PROFILE_LIST_REQUEST:
			sml_get_profile_list_request_free((sml_get_profile_list_request *)message_body->data);
			break;
		case SML_MESSAGE_GET_PROFILE_LIST_RESPONSE:
			sml_get_profile_list_response_free((sml_get_profile_list_response *)message_body->data);
			break;
		case SML_MESSAGE_GET_PROC_PARAMETER_REQUEST:
			sml_get_proc_parameter_request_free(
				(sml_get_proc_parameter_request *)message_body->data);
			break;
		case SML_MESSAGE_GET_PROC_PARAMETER_RESPONSE:
			sml_get_proc_parameter_response_free(
				(sml_get_proc_parameter_response *)message_body->data);
			break;
		case SML_MESSAGE_SET_PROC_PARAMETER_REQUEST:
			sml_set_proc_parameter_request_free(
				(sml_set_proc_parameter_request *)message_body->data);
			break;
		case SML_MESSAGE_GET_LIST_REQUEST:
			sml_get_list_request_free((sml_get_list_request *)message_body->data);
			break;
		case SML_MESSAGE_GET_LIST_RESPONSE:
			sml_get_list_response_free((sml_get_list_response *)message_body->data);
			break;
		case SML_MESSAGE_ATTENTION_RESPONSE:
			sml_attention_response_free((sml_attention_response *)message_body->data);
			break;
		default:
			fprintf(stderr, "libsml: NYI: %s for message type %04X\n", __func__,
					*(message_body->tag));
			break;
		}
		sml_number_free(message_body->tag);
		free(message_body);
	}
}
