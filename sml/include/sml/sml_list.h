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

#ifndef SML_LIST_H_
#define SML_LIST_H_

#include "sml_number.h"
#include "sml_octet_string.h"
#include "sml_status.h"
#include "sml_time.h"
#include "sml_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void **elems;
	int elems_len;
	void (*elem_free)(void *elem);
} sml_sequence;

sml_sequence *sml_sequence_init(void (*elem_free)(void *elem));
sml_sequence *sml_sequence_parse(sml_buffer *buf, void *(*elem_parse)(sml_buffer *buf),
								 void (*elem_free)(void *elem));
void sml_sequence_write(sml_sequence *seq, sml_buffer *buf,
						void (*elem_write)(void *elem, sml_buffer *buf));
void sml_sequence_free(sml_sequence *seq);
void sml_sequence_add(sml_sequence *list, void *new_entry);

typedef struct sml_list_entry {
	octet_string *obj_name;
	sml_status *status; // optional
	sml_time *val_time; // optional
	sml_unit *unit;     // optional
	i8 *scaler;         // optional
	sml_value *value;
	sml_signature *value_signature; // optional

	// list specific
	struct sml_list_entry *next;
} sml_list;

sml_list *sml_list_init();
sml_list *sml_list_parse(sml_buffer *buf);
void sml_list_write(sml_list *list, sml_buffer *buf);
void sml_list_add(sml_list *list, sml_list *new_entry);
void sml_list_free(sml_list *list);

#ifdef __cplusplus
}
#endif

#endif /* SML_LIST_H_ */
