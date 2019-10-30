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

#ifndef SML_MESSAGE_H_
#define SML_MESSAGE_H_

#include "sml_list.h"
#include "sml_octet_string.h"
#include "sml_time.h"
#include "sml_tree.h"
#include <stdlib.h>

#include "sml_attention_response.h"
#include "sml_close_request.h"
#include "sml_close_response.h"
#include "sml_get_list_request.h"
#include "sml_get_list_response.h"
#include "sml_get_proc_parameter_request.h"
#include "sml_get_proc_parameter_response.h"
#include "sml_get_profile_list_request.h"
#include "sml_get_profile_list_response.h"
#include "sml_get_profile_pack_request.h"
#include "sml_get_profile_pack_response.h"
#include "sml_open_request.h"
#include "sml_open_response.h"
#include "sml_set_proc_parameter_request.h"

// clang-format off
#define SML_MESSAGE_OPEN_REQUEST                0x00000100
#define SML_MESSAGE_OPEN_RESPONSE               0x00000101
#define SML_MESSAGE_CLOSE_REQUEST               0x00000200
#define SML_MESSAGE_CLOSE_RESPONSE              0x00000201
#define SML_MESSAGE_GET_PROFILE_PACK_REQUEST    0x00000300
#define SML_MESSAGE_GET_PROFILE_PACK_RESPONSE   0x00000301
#define SML_MESSAGE_GET_PROFILE_LIST_REQUEST    0x00000400
#define SML_MESSAGE_GET_PROFILE_LIST_RESPONSE   0x00000401
#define SML_MESSAGE_GET_PROC_PARAMETER_REQUEST  0x00000500
#define SML_MESSAGE_GET_PROC_PARAMETER_RESPONSE 0x00000501
#define SML_MESSAGE_SET_PROC_PARAMETER_REQUEST  0x00000600
#define SML_MESSAGE_SET_PROC_PARAMETER_RESPONSE 0x00000601 // This doesn't exist in the spec
#define SML_MESSAGE_GET_LIST_REQUEST            0x00000700
#define SML_MESSAGE_GET_LIST_RESPONSE           0x00000701
#define SML_MESSAGE_ATTENTION_RESPONSE          0x0000FF01
// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u32 *tag;
	void *data;
} sml_message_body;

typedef struct {
	octet_string *transaction_id;
	u8 *group_id;
	u8 *abort_on_error;
	sml_message_body *message_body;
	u16 *crc;
	/* end of message */
} sml_message;

// SML MESSAGE
sml_message *sml_message_parse(sml_buffer *buf);
sml_message *sml_message_init(); // Sets a transaction id.
void sml_message_free(sml_message *msg);
void sml_message_write(sml_message *msg, sml_buffer *buf);

// SML_MESSAGE_BODY
sml_message_body *sml_message_body_parse(sml_buffer *buf);
sml_message_body *sml_message_body_init(u32 tag, void *data);
void sml_message_body_free(sml_message_body *message_body);
void sml_message_body_write(sml_message_body *message_body, sml_buffer *buf);

#ifdef __cplusplus
}
#endif

#endif /* SML_MESSAGE_H_ */
